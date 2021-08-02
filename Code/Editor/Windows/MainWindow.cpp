#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorFlowScene.h>
#include <Editor/Core/EditorFlowView.h>
#include <Editor/Core/EditorUtils.h>
#include <Editor/Core/Models/SSBehaviorTreeNodeDataModel.h>
#include <Editor/Core/Models/SubtreeNodeModel.h>
#include <Editor/Windows/MainWindow.h>

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes.h>
#include <SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h>

#include <QDir>
#include <QDomDocument>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QShortcut>
#include <QSizePolicy>
#include <QStatusBar>
#include <QVBoxLayout>

namespace SparkyStudios::AI::BehaviorTree::Editor::Windows
{
    MainWindow::MainWindow(QWidget* parent, const AZ::IO::PathView& projectPath)
        : AzQtComponents::DockMainWindow(parent)
        , _current_layout(QtNodes::PortLayout::Vertical)
    {
        setWindowTitle(tr("BehaviorTree Editor"));
        setMinimumSize(800, 600);

        _model_registry = createDataModelRegistry();

        m_sidePanel = new Widgets::NodesSidePanel(_model_registry, _treenode_models, this);
        m_sidePanel->updateTreeView();

        m_nodeProperties = new Widgets::NodeProperties(this);

        m_blackboardProperties = new Widgets::BlackboardProperties(_blackboard_models, this);
        m_blackboardProperties->UpdateProperties();

        m_tabWidget = new AzQtComponents::TabWidget();
        AzQtComponents::TabWidget::applySecondaryStyle(m_tabWidget, false);

        m_tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_tabWidget->tabBar(), &QTabBar::customContextMenuRequested, this, &MainWindow::OnTabCustomContextMenuRequested);

        m_fancyDocking = new AzQtComponents::FancyDocking(this);

        auto nodesLibrary = new AzQtComponents::StyledDockWidget(tr("Nodes Library"), this);
        nodesLibrary->setObjectName(nodesLibrary->windowTitle());
        nodesLibrary->setWidget(m_sidePanel);
        addDockWidget(Qt::LeftDockWidgetArea, nodesLibrary);

        auto blackboardProperties = new AzQtComponents::StyledDockWidget(tr("Blackboard Properties"), this);
        blackboardProperties->setObjectName(blackboardProperties->windowTitle());
        blackboardProperties->setWidget(m_blackboardProperties);
        addDockWidget(Qt::RightDockWidgetArea, blackboardProperties);

        auto nodeProperties = new AzQtComponents::StyledDockWidget(tr("Node Properties"), this);
        nodeProperties->setObjectName(nodeProperties->windowTitle());
        nodeProperties->setWidget(m_nodeProperties);
        addDockWidget(Qt::RightDockWidgetArea, nodeProperties);

        m_statusBar = new Widgets::StatusBar(this);
        statusBar()->addWidget(m_statusBar);

        if (!projectPath.empty())
        {
            _project_path = QString::fromUtf8(projectPath.Native().data(), aznumeric_cast<int>(projectPath.Native().size()));
        }

        resize(800, 600);
        setCentralWidget(m_tabWidget);

        setupShortcuts();

        createTab("UnsavedTree");
        OnTabSetMainTree(0);
        OnSceneChanged();

        _current_state = saveCurrentState();
    }

    Widgets::GraphicContainer* MainWindow::CurrentTabInfo()
    {
        int index = m_tabWidget->currentIndex();
        QString tab_name = m_tabWidget->tabText(index);
        return GetTabByName(tab_name);
    }

    Widgets::GraphicContainer* MainWindow::GetTabByName(const QString& tab_name)
    {
        auto it = _tab_info.find(tab_name);
        return (it != _tab_info.end()) ? (it->second) : nullptr;
    }

    void MainWindow::LockEditor(const bool locked)
    {
        for (auto& tab_it : _tab_info)
        {
            tab_it.second->lockEditing(locked);
        }
    }

    void MainWindow::LoadFromXML(const QString& xml)
    {
        QDomDocument document;

        try
        {
            QString errorMsg;
            int errorLine;
            if (!document.setContent(xml, &errorMsg, &errorLine))
            {
                throw std::runtime_error(tr("Error parsing XML (line %1): %2").arg(errorLine).arg(errorMsg).toStdString());
            }
            //---------------
            std::vector<QString> registeredId;
            for (const auto& it : _treenode_models)
            {
                registeredId.push_back(it.first);
            }
            std::vector<QString> error_messages;
            bool done = Core::VerifyXML(document, registeredId, error_messages);

            if (!done)
            {
                QString merged_error;
                for (const auto& err : error_messages)
                {
                    merged_error += err + "\n";
                }
                throw std::runtime_error(merged_error.toStdString());
            }
        } catch (std::runtime_error& err)
        {
            QMessageBox messageBox;
            messageBox.critical(this, "Error parsing the XML", err.what());
            messageBox.show();
            return;
        }

        //---------------
        bool error = false;
        QString err_message;
        auto saved_state = _current_state;
        auto prev_tree_model = _treenode_models;

        try
        {
            auto document_root = document.documentElement();

            if (document_root.hasAttribute("main_tree_to_execute"))
            {
                _main_tree = document_root.attribute("main_tree_to_execute");
            }

            auto custom_models = Core::ReadTreeNodesModel(document_root);

            for (const auto& model : custom_models)
            {
                OnAddToModelRegistry(model.second);
            }

            m_sidePanel->updateTreeView();

            auto bb_properties = Core::ReadBlackboardPropertiesModel(document_root);
            _blackboard_models.clear();

            for (auto&& property : bb_properties)
            {
                _blackboard_models.insert({ property.second.name, property.second });
            }

            m_blackboardProperties->UpdateProperties();

            OnActionClearTriggered(false);

            const QSignalBlocker blocker(CurrentTabInfo());

            for (auto bt_root = document_root.firstChildElement("BehaviorTree"); !bt_root.isNull();
                 bt_root = bt_root.nextSiblingElement("BehaviorTree"))
            {
                auto tree = Core::BuildTreeFromXML(bt_root, _treenode_models);
                QString tree_name("BehaviorTree");

                if (bt_root.hasAttribute("ID"))
                {
                    tree_name = bt_root.attribute("ID");
                    if (_main_tree.isEmpty()) // valid when there is only one
                    {
                        _main_tree = tree_name;
                    }
                }

                OnCreateAbsBehaviorTree(tree, tree_name);
            }

            if (!_main_tree.isEmpty())
            {
                for (int i = 0; i < m_tabWidget->count(); i++)
                {
                    if (m_tabWidget->tabText(i) == _main_tree)
                    {
                        m_tabWidget->tabBar()->moveTab(i, 0);
                        m_tabWidget->setCurrentIndex(0);
                        m_tabWidget->tabBar()->setTabIcon(0, QIcon(":/icons/green_star.png"));
                        break;
                    }
                }
            }

            if (CurrentTabInfo() == nullptr)
            {
                createTab("BehaviorTree");
                _main_tree = "BehaviorTree";
            }
            else
            {
                CurrentTabInfo()->nodeReorder();
            }

            // TODO
            // auto models_to_remove = GetModelsToRemove(this, _treenode_models, custom_models);

            // for (QString model_name : models_to_remove)
            // {
            //     OnModelRemoveRequested(model_name);
            // }
        } catch (std::exception& err)
        {
            error = true;
            err_message = err.what();
        }

        if (error)
        {
            _treenode_models = prev_tree_model;
            loadSavedStateFromJson(saved_state);
            qDebug() << "R: Undo size: " << _undo_stack.size() << " Redo size: " << _redo_stack.size();
            QMessageBox::warning(
                this, tr("Exception!"), tr("It was not possible to parse the file. Error:\n\n%1").arg(err_message), QMessageBox::Ok);
        }
        else
        {
            OnSceneChanged();
            OnPushUndo();
        }
    }

    void MainWindow::SetCurrentNode(QtNodes::Node* node)
    {
        m_nodeProperties->SetNode(node);
    }

    void MainWindow::OnAutoArrange()
    {
        CurrentTabInfo()->nodeReorder();
    }

    void MainWindow::OnSceneChanged()
    {
        const bool isValidTree = CurrentTabInfo()->containsValidTree();

        // TODO: Enable/Disable toolbox buttons
        // TODO: Enable/Disable save button

        if (isValidTree)
        {
            // TODO: Update status bar to mark tree as valid
            emit ValidTree();
        }
        else
        {
            // TODO: Update status bar to mark tree as invalid
            emit InvalidTree();
        }

        LockEditor(false);
    }

    void MainWindow::OnUndoInvoked()
    {
        if (_undo_stack.size() > 0)
        {
            _redo_stack.push_back(std::move(_current_state));
            _current_state = _undo_stack.back();
            _undo_stack.pop_back();

            loadSavedStateFromJson(_current_state);
        }
    }

    void MainWindow::OnRedoInvoked()
    {
        if (_redo_stack.size() > 0)
        {
            _undo_stack.push_back(_current_state);
            _current_state = std::move(_redo_stack.back());
            _redo_stack.pop_back();

            loadSavedStateFromJson(_current_state);
        }
    }

    void MainWindow::OnTabSetMainTree(int tab_index)
    {
        for (int i = 0; i < m_tabWidget->count(); i++)
        {
            if (i == tab_index)
            {
                // TODO: Mark this tab as active (maybe with an icon?)
                _main_tree = m_tabWidget->tabBar()->tabText(i);
            }
            else
            {
                m_tabWidget->tabBar()->setTabIcon(i, QIcon());
            }
        }

        m_tabWidget->setCurrentIndex(tab_index);
    }

    void MainWindow::OnTabCustomContextMenuRequested(const QPoint& pos)
    {
        int index = m_tabWidget->tabBar()->tabAt(pos);
        QString name = m_tabWidget->tabText(index);
        bool isMainTree = _main_tree == name;

        QMenu menu(this);

        // ------------
        QAction* save = menu.addAction("Save");
        connect(save, &QAction::triggered, this, &MainWindow::OnActionSaveTriggered);
        // ------------
        QAction* setAsEntryPoint = menu.addAction("Set as Entry Point");
        setAsEntryPoint->setEnabled(!isMainTree);
        connect(
            setAsEntryPoint, &QAction::triggered, this,
            [this, index]()
            {
                OnTabSetMainTree(index);
            });
        // ------------
        QAction* rename = menu.addAction("Rename");
        connect(
            rename, &QAction::triggered, this,
            [this, index]()
            {
                OnTabRenameRequested(index);
            });
        // ------------

        QPoint globalPos = m_tabWidget->tabBar()->mapToGlobal(pos);
        menu.exec(globalPos);
    }

    void MainWindow::OnActionClearTriggered(bool createNew)
    {
        for (auto& it : _tab_info)
        {
            it.second->clearScene();
            it.second->deleteLater();
        }

        _tab_info.clear();
        _opened_file.clear();
        m_statusBar->SetOpenedFileName("No Open File");

        m_tabWidget->clear();
        m_sidePanel->clear();

        if (createNew)
        {
            createTab("BehaviorTree");
        }
    }

    void MainWindow::OnPushUndo()
    {
        SavedState saved = saveCurrentState();

        if (_undo_stack.empty() || (saved != _current_state && _undo_stack.back() != _current_state))
        {
            _undo_stack.push_back(std::move(_current_state));
            _redo_stack.clear();
        }
        _current_state = saved;
    }

    void MainWindow::OnCreateAbsBehaviorTree(const Core::AbstractBehaviorTree& tree, const QString& bt_name, bool secondary_tabs)
    {
        auto container = GetTabByName(bt_name);
        if (!container)
        {
            container = createTab(bt_name);
        }

        const QSignalBlocker blocker(container);
        container->loadSceneFromTree(tree);
        container->nodeReorder();

        if (secondary_tabs)
        {
            for (const auto& node : tree.Nodes())
            {
                if (node.model.type == Core::NodeType::SUBTREE && GetTabByName(node.model.registrationId) == nullptr)
                {
                    createTab(node.model.registrationId);
                }
            }
        }

        clearUndoStacks();
    }

    void MainWindow::OnActionSaveTriggered()
    {
        saveFile(false);
    }

    void MainWindow::OnActionSaveAsTriggered()
    {
        saveFile(true);
    }

    void MainWindow::OnActionLoadTriggered()
    {
        QSettings settings;
        QString directory_path = settings.value("MainWindow.lastLoadDirectory", _project_path).toString();

        QString fileName =
            QFileDialog::getOpenFileName(this, tr("Load BehaviorTree from file"), directory_path, tr("SS BehaviorTree files (*.ssbt)"));
        if (!QFileInfo::exists(fileName))
        {
            return;
        }

        if (_opened_file == fileName)
        {
            return;
        }

        QFileInfo fileInfo(fileName);
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        directory_path = fileInfo.absolutePath();
        settings.setValue("MainWindow.lastLoadDirectory", directory_path);
        settings.sync();

        QString xml_text;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            xml_text += in.readLine();
        }

        createTab(fileInfo.baseName());
        LoadFromXML(xml_text);
        _opened_file = fileName;
        m_statusBar->SetOpenedFileName(fileInfo.baseName());
    }

    void MainWindow::OnAddToModelRegistry(const Core::NodeModel& model)
    {
        namespace util = QtNodes::detail;
        const auto& ID = model.registrationId;

        QtNodes::DataModelRegistry::RegistryItemCreator node_creator = [model]() -> QtNodes::DataModelRegistry::RegistryItemPtr
        {
            if (model.type == Core::NodeType::SUBTREE)
            {
                return util::make_unique<Core::Models::SubtreeNodeModel>(model);
            }
            return util::make_unique<Core::Models::SSBehaviorTreeNodeDataModel>(model);
        };

        _model_registry->registerModel(QString::fromStdString(BT::toStr(model.type)), node_creator, ID);

        _treenode_models.insert({ ID, model });
        m_sidePanel->updateTreeView();
    }

    void MainWindow::OnTabRenameRequested(int tab_index, QString new_name)
    {
        QString old_name = m_tabWidget->tabText(tab_index);

        if (new_name.isEmpty())
        {
            bool ok = false;
            new_name = QInputDialog::getText(
                this, tr("Change BehaviorTree Name"), tr("Insert the new name of this BehaviorTree"), QLineEdit::Normal, old_name, &ok);
            if (!ok)
            {
                return;
            }
        }

        if (new_name == old_name)
        {
            return;
        }

        if (GetTabByName(new_name))
        {
            QMessageBox::warning(
                this, "Tab name already is use",
                tr("There is already a BehaviorTree called [%1].\n"
                   "Use another name.")
                    .arg(new_name),
                QMessageBox::Ok);
            return;
        }

        m_tabWidget->setTabText(tab_index, new_name);
        auto it = _tab_info.find(old_name);
        auto container = it->second;
        _tab_info.insert({ new_name, container });
        _tab_info.erase(it);
        if (_main_tree == old_name)
        {
            _main_tree = new_name;
        }

        // if a subtree SUBTREE already
        if (_model_registry->registeredModelsCategoryAssociation().count(old_name) != 0)
        {
            // _model_registry->unregisterModel(old_name);
            _treenode_models.erase(old_name);
            Core::NodeModel model = { Core::NodeType::SUBTREE, new_name, {} };
            OnAddToModelRegistry(model);
            _treenode_models.insert({ new_name, model });
            m_sidePanel->updateTreeView();
            OnTreeNodeEdited(old_name, new_name);
        }

        // TODO: this is a work around until we find a better solution
        clearUndoStacks();
    }

    void MainWindow::OnTreeNodeEdited(QString prev_ID, QString new_ID)
    {
        for (auto& it : _tab_info)
        {
            auto container = it.second;
            std::vector<QtNodes::Node*> nodes_to_rename;

            for (const auto& node_it : container->scene()->nodes())
            {
                QtNodes::Node* graphic_node = node_it.second.get();
                if (!graphic_node)
                {
                    continue;
                }
                auto bt_node = dynamic_cast<Core::Models::SSBehaviorTreeNodeDataModel*>(graphic_node->nodeDataModel());
                if (!bt_node)
                {
                    continue;
                }

                if (bt_node->model().registrationId == prev_ID)
                {
                    nodes_to_rename.push_back(graphic_node);
                }
            }

            for (auto& graphic_node : nodes_to_rename)
            {
                auto bt_node = dynamic_cast<Core::Models::SSBehaviorTreeNodeDataModel*>(graphic_node->nodeDataModel());
                bool is_expanded_subtree = false;

                if (bt_node->model().type == Core::NodeType::SUBTREE)
                {
                    auto subtree_model = dynamic_cast<Core::Models::SubtreeNodeModel*>(bt_node);
                    if (subtree_model && subtree_model->expanded())
                    {
                        is_expanded_subtree = true;
                        subTreeExpand(*container, *graphic_node, SUBTREE_COLLAPSE);
                    }
                }

                auto new_node = container->substituteNode(graphic_node, new_ID);

                if (is_expanded_subtree)
                {
                    subTreeExpand(*container, *new_node, SUBTREE_EXPAND);
                };
            }
        }
    }

    void MainWindow::setupShortcuts()
    {
        // ------------
        auto* arrange_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G, Qt::Key_A), this);
        connect(arrange_shortcut, &QShortcut::activated, this, &MainWindow::OnAutoArrange);
        // ------------
        auto* undo_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
        connect(undo_shortcut, &QShortcut::activated, this, &MainWindow::OnUndoInvoked);
        // ------------
        auto* redo_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z), this);
        connect(redo_shortcut, &QShortcut::activated, this, &MainWindow::OnRedoInvoked);
        // ------------
        auto* save_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
        connect(save_shortcut, &QShortcut::activated, this, &MainWindow::OnActionSaveTriggered);
        // ------------
        auto* save_as_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this);
        connect(save_as_shortcut, &QShortcut::activated, this, &MainWindow::OnActionSaveAsTriggered);
        // ------------
        auto* open_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
        connect(open_shortcut, &QShortcut::activated, this, &MainWindow::OnActionLoadTriggered);
    }

    Widgets::GraphicContainer* MainWindow::createTab(const QString& name, bool setActive)
    {
        if (_tab_info.count(name) > 0)
        {
            AZ_Warning("SSBehaviorTreeEditor", false, "There is already a Tab named %s", name.toStdString().c_str());
            return _tab_info[name];
        }

        Widgets::GraphicContainer* ti = new Widgets::GraphicContainer(_model_registry, this);
        ti->setObjectName(name);
        _tab_info.insert({ name, ti });

        ti->scene()->setLayout(_current_layout);

        int index = m_tabWidget->addTab(ti->view(), name);

        if (setActive)
        {
            m_tabWidget->setCurrentIndex(index);
        }

        ti->scene()->CreateNodeAtPosition("Root", "Root", QPointF(0, 0));
        ti->zoomHomeView();
        ti->lockEditing(false);

        //--------------------------------

        connect(ti, &Widgets::GraphicContainer::undoableChange, this, &MainWindow::OnPushUndo);

        connect(ti, &Widgets::GraphicContainer::undoableChange, this, &MainWindow::OnSceneChanged);

        // connect(ti, &Widgets::GraphicContainer::requestSubTreeExpand, this, &MainWindow::OnRequestSubTreeExpand);

        connect(
            ti, &Widgets::GraphicContainer::requestSubTreeCreate, this,
            [this](const Core::AbstractBehaviorTree& tree, const QString& bt_name)
            {
                OnCreateAbsBehaviorTree(tree, bt_name, false);
            });

        connect(ti, &Widgets::GraphicContainer::addNewModel, this, &MainWindow::OnAddToModelRegistry);

        return ti;
    }

    std::shared_ptr<QtNodes::DataModelRegistry> MainWindow::createDataModelRegistry()
    {
        EBUS_EVENT_RESULT(m_factory, SSBehaviorTreeRequestBus, GetFactory);

        Nodes::RegisterDefaultNodes(m_factory.GetRegistry());
        Nodes::RegisterDefaultProperties(m_factory.GetRegistry());

        // Enable all registered nodes
        m_factory.GetRegistry()->EnableNodes();

        auto ret = std::make_shared<QtNodes::DataModelRegistry>();

        auto registerModel = [&ret](const QString& ID, const Core::NodeModel& model)
        {
            QString category = QString::fromStdString(BT::toStr(model.type));

            if (ID == "Root")
            {
                category = "Root";
            }

            QtNodes::DataModelRegistry::RegistryItemCreator creator;
            creator = [model]() -> QtNodes::DataModelRegistry::RegistryItemPtr
            {
                return std::make_unique<Core::Models::SSBehaviorTreeNodeDataModel>(model);
            };

            ret->registerModel(category, creator, ID);
        };

        for (const auto& model : Core::BuiltInNodeModels(m_factory))
        {
            registerModel(model.first, model.second);
            _treenode_models.insert({ model.first, model.second });
        }

        return ret;
    }

    void MainWindow::loadSavedStateFromJson(SavedState saved_state)
    {
        // TODO crash if the name of the container (tab) changed
        for (auto& it : _tab_info)
        {
            it.second->clearScene();
            it.second->deleteLater();
        }

        _tab_info.clear();
        m_tabWidget->clear();

        _main_tree = saved_state.main_tree;

        for (const auto& it : saved_state.json_states)
        {
            QString tab_name = it.first;
            _tab_info.insert({ tab_name, createTab(tab_name) });
        }
        for (const auto& it : saved_state.json_states)
        {
            QString name = it.first;
            auto* container = GetTabByName(name);
            container->loadFromJson(it.second);
            container->view()->setTransform(saved_state.view_transform);
            container->view()->setSceneRect(saved_state.view_area);
        }

        for (int i = 0; i < m_tabWidget->count(); i++)
        {
            if (m_tabWidget->tabText(i) == saved_state.current_tab_name)
            {
                m_tabWidget->setCurrentIndex(i);
                m_tabWidget->widget(i)->setFocus();
            }
            if (m_tabWidget->tabText(i) == _main_tree)
            {
                OnTabSetMainTree(i);
            }
        }
        if (m_tabWidget->count() == 1)
        {
            OnTabSetMainTree(0);
        }

        OnSceneChanged();
    }

    MainWindow::SavedState MainWindow::saveCurrentState()
    {
        SavedState saved;
        int index = m_tabWidget->currentIndex();
        saved.main_tree = _main_tree;
        saved.current_tab_name = m_tabWidget->tabText(index);
        auto current_view = GetTabByName(saved.current_tab_name)->view();
        saved.view_transform = current_view->transform();
        saved.view_area = current_view->sceneRect();

        for (auto& it : _tab_info)
        {
            saved.json_states[it.first] = it.second->scene()->saveToMemory();
        }

        return saved;
    }

    void MainWindow::clearUndoStacks()
    {
        _undo_stack.clear();
        _redo_stack.clear();
        OnSceneChanged();
        OnPushUndo();
    }

    QString MainWindow::saveToXML() const
    {
        QDomDocument doc;

        const char* COMMENT_SEPARATOR = " ////////// ";

        QDomElement root = doc.createElement("root");
        doc.appendChild(root);

        if (_main_tree.isEmpty() == false)
        {
            root.setAttribute("main_tree_to_execute", _main_tree.toStdString().c_str());
        }

        for (auto& it : _tab_info)
        {
            auto& container = it.second;
            auto scene = container->scene();

            auto abs_tree = Core::BuildTreeFromScene(container->scene());
            auto abs_root = abs_tree.RootNode();

            if (abs_root->childrenIndex.size() == 1 && abs_root->model.registrationId == "Root")
            {
                // Move to the child of ROOT
                abs_root = abs_tree.Node(abs_root->childrenIndex.front());
            }

            QtNodes::Node* root_node = abs_root->graphicNode;

            root.appendChild(doc.createComment(COMMENT_SEPARATOR));
            QDomElement root_element = doc.createElement("BehaviorTree");

            root_element.setAttribute("ID", it.first.toStdString().c_str());
            root.appendChild(root_element);

            Core::RecursivelyCreateXml(*scene, doc, root_element, root_node, m_factory);
        }

        QDomElement root_models = doc.createElement("TreeNodesModel");

        for (const auto& tree_it : _treenode_models)
        {
            const auto& ID = tree_it.first;
            const auto& model = tree_it.second;

            if (Core::BuiltInNodeModels(m_factory).count(ID) != 0)
            {
                continue;
            }

            QDomElement node = doc.createElement(QString::fromStdString(toStr(model.type)));

            if (!node.isNull())
            {
                node.setAttribute("ID", ID);

                for (const auto& port_it : model.ports)
                {
                    const auto& port_name = port_it.first;
                    const auto& port = port_it.second;

                    QDomElement port_element = writePortModel(port_name, port, doc);
                    node.appendChild(port_element);
                }
            }

            root_models.appendChild(node);
        }

        QDomElement root_properties = doc.createElement("Blackboard");

        for (auto&& property : _blackboard_models)
        {
            QDomElement property_element = doc.createElement("Property");

            if (!property_element.isNull())
            {
                property_element.setAttribute("name", property.second.name);
                property_element.setAttribute("description", property.second.description);
                property_element.setAttribute("type", property.second.type);
                property_element.setAttribute("suffix", property.second.suffix);
                property_element.setAttribute("private", property.second.isPrivate ? "true" : "false");
            }

            root_properties.appendChild(property_element);
        }

        root.appendChild(doc.createComment(COMMENT_SEPARATOR));
        root.appendChild(root_models);
        root.appendChild(doc.createComment(COMMENT_SEPARATOR));
        root.appendChild(root_properties);
        root.appendChild(doc.createComment(COMMENT_SEPARATOR));

        return xmlDocumentToString(doc);
    }

    QString MainWindow::xmlDocumentToString(const QDomDocument& document) const
    {
        QString output_string;

        QXmlStreamWriter stream(&output_string);

        stream.setAutoFormatting(true);
        stream.setAutoFormattingIndent(4);

        stream.writeStartDocument();

        auto root_element = document.documentElement();

        stream.writeStartElement(root_element.tagName());

        streamElementAttributes(stream, root_element);

        auto next_node = root_element.firstChild();

        while (!next_node.isNull())
        {
            recursivelySaveNodeCanonically(stream, next_node);

            if (stream.hasError())
            {
                break;
            }
            next_node = next_node.nextSibling();
        }

        stream.writeEndElement();
        stream.writeEndDocument();

        return output_string;
    }

    void MainWindow::streamElementAttributes(QXmlStreamWriter& stream, const QDomElement& element) const
    {
        if (element.hasAttributes())
        {
            QMap<QString, QString> attributes;
            const QDomNamedNodeMap attributes_map = element.attributes();

            for (int i = 0; i < attributes_map.count(); ++i)
            {
                auto attribute = attributes_map.item(i);
                attributes.insert(attribute.nodeName(), attribute.nodeValue());
            }

            auto i = attributes.constBegin();
            while (i != attributes.constEnd())
            {
                stream.writeAttribute(i.key(), i.value());
                ++i;
            }
        }
    }

    void MainWindow::recursivelySaveNodeCanonically(QXmlStreamWriter& stream, const QDomNode& parent_node) const
    {
        if (stream.hasError())
        {
            return;
        }

        if (parent_node.isElement())
        {
            const QDomElement parent_element = parent_node.toElement();

            if (!parent_element.isNull())
            {
                stream.writeStartElement(parent_element.tagName());

                streamElementAttributes(stream, parent_element);

                if (parent_element.hasChildNodes())
                {
                    auto child = parent_element.firstChild();
                    while (!child.isNull())
                    {
                        recursivelySaveNodeCanonically(stream, child);
                        child = child.nextSibling();
                    }
                }

                stream.writeEndElement();
            }
        }
        else if (parent_node.isComment())
        {
            stream.writeComment(parent_node.nodeValue());
        }
        else if (parent_node.isText())
        {
            stream.writeCharacters(parent_node.nodeValue());
        }
    }

    bool MainWindow::SavedState::operator==(const MainWindow::SavedState& other) const
    {
        if (current_tab_name != other.current_tab_name || json_states.size() != other.json_states.size())
        {
            return false;
        }
        for (auto& it : json_states)
        {
            auto other_it = other.json_states.find(it.first);
            if (other_it == other.json_states.end() || it.second != other_it->second)
            {
                return false;
            }
        }
        if (view_area != other.view_area || view_transform != other.view_transform)
        {
            return false;
        }
        return true;
    }

    QtNodes::Node* MainWindow::subTreeExpand(
        Widgets::GraphicContainer& container, QtNodes::Node& node, MainWindow::SubtreeExpandOption option)
    {
        bool is_editor_mode = true;
        const QSignalBlocker blocker(this);
        auto subtree_model = dynamic_cast<Core::Models::SubtreeNodeModel*>(node.nodeDataModel());
        const QString& subtree_name = subtree_model->registrationName();

        if (option == SUBTREE_EXPAND && subtree_model->expanded() == false)
        {
            auto subtree_container = GetTabByName(subtree_name);

            // Prevent expansion of invalid subtree
            if (!subtree_container->containsValidTree())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Invalid SubTree. Can not expand SubTree."), QMessageBox::Cancel);
                return &node;
            }

            auto abs_subtree = BuildTreeFromScene(subtree_container->scene());

            subtree_model->setExpanded(true);
            node.nodeState().getEntries(QtNodes::PortType::Out).resize(1);
            container.appendTreeToNode(node, abs_subtree);
            container.lockSubtreeEditing(node, true, is_editor_mode);

            if (abs_subtree.Nodes().size() > 1)
            {
                container.nodeReorder();
            }

            return &node;
        }

        if (option == SUBTREE_COLLAPSE && subtree_model->expanded() == true)
        {
            bool need_reorder = true;
            const auto& conn_out = node.nodeState().connections(QtNodes::PortType::Out, 0);
            QtNodes::Node* child_node = nullptr;
            if (conn_out.size() == 1)
            {
                child_node = conn_out.begin()->second->getNode(QtNodes::PortType::In);
            }

            const QSignalBlocker b2(container);
            if (child_node)
            {
                container.deleteSubTreeRecursively(*child_node);
            }
            else
            {
                need_reorder = false;
            }

            subtree_model->setExpanded(false);
            node.nodeState().getEntries(QtNodes::PortType::Out).resize(0);
            container.lockSubtreeEditing(node, false, is_editor_mode);
            if (need_reorder)
            {
                container.nodeReorder();
            }

            return &node;
        }

        if (option == SUBTREE_REFRESH && subtree_model->expanded() == true)
        {
            const auto& conn_out = node.nodeState().connections(QtNodes::PortType::Out, 0);
            if (conn_out.size() != 1)
            {
                throw std::logic_error("subTreeExpand with SUBTREE_REFRESH, but not an expanded SubTree");
            }

            QtNodes::Node* child_node = conn_out.begin()->second->getNode(QtNodes::PortType::In);

            auto subtree_container = GetTabByName(subtree_name);
            auto subtree = Core::BuildTreeFromScene(subtree_container->scene());

            container.deleteSubTreeRecursively(*child_node);
            container.appendTreeToNode(node, subtree);
            container.nodeReorder();
            container.lockSubtreeEditing(node, true, is_editor_mode);

            return &node;
        }

        return nullptr;
    }

    void MainWindow::saveFile(bool overwrite)
    {
        for (auto& it : _tab_info)
        {
            auto& container = it.second;
            if (!container->containsValidTree())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Malformed behavior tree. File can not be saved."), QMessageBox::Cancel);
                return;
            }
        }

        if (_tab_info.size() == 1)
        {
            _main_tree = _tab_info.begin()->first;
        }

        QSettings settings;
        QString directory_path = settings.value("MainWindow.lastSaveDirectory", _project_path).toString();

        QString fileName;

        if (overwrite || _opened_file.isNull())
        {
            fileName = QFileDialog::getSaveFileName(this, "Save BehaviorTree to file", directory_path, "SS BehaviorTree files (*.ssbt)");
            if (fileName.isEmpty())
            {
                return;
            }
            if (!fileName.endsWith(".ssbt"))
            {
                fileName += ".ssbt";
            }
        }
        else
        {
            fileName = _opened_file;
        }

        QString xml_text = saveToXML();

        QFileInfo fileInfo(fileName);
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << xml_text << Qt::endl;
        }

        directory_path = fileInfo.absolutePath();
        settings.setValue("MainWindow.lastSaveDirectory", directory_path);

        _opened_file = fileName;
        m_statusBar->SetOpenedFileName(fileInfo.baseName());
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Windows

#include <Editor/Windows/moc_MainWindow.cpp>

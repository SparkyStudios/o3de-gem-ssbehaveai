// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorFlowScene.h>
#include <Editor/Core/EditorFlowView.h>
#include <Editor/Core/EditorUtils.h>
#include <Editor/Core/Models/SSBehaviorTreeNodeDataModel.h>
#include <Editor/Core/Models/SubtreeNodeModel.h>
#include <Editor/Windows/MainWindow.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/BehaveBehaviorTreeBus.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes.h>

#include <QDomDocument>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
#include <QStatusBar>

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Windows
{
    MainWindow::MainWindow(QWidget* parent, const AZ::IO::PathView& projectPath, const AZ::IO::PathView& filePath)
        : DockMainWindow(parent)
        , _currentLayout(QtNodes::PortLayout::Vertical)
        , _dirtyFile(false)
    {
        setWindowTitle(tr("Behave AI - Behavior Tree Editor"));
        setMinimumSize(800, 600);

        _modelRegistry = CreateDataModelRegistry();

        _sidePanel = new Widgets::NodesSidePanel(_modelRegistry, _treeNodeModels, this);
        _sidePanel->updateTreeView();

        _nodeProperties = new Widgets::NodeProperties(this);

        _blackboardProperties = new Widgets::BlackboardProperties(_blackboardModels, this);
        _blackboardProperties->UpdateProperties();

        _tabWidget = new AzQtComponents::TabWidget();
        AzQtComponents::TabWidget::applySecondaryStyle(_tabWidget, false);

        _tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(_tabWidget->tabBar(), &QTabBar::customContextMenuRequested, this, &MainWindow::OnTabCustomContextMenuRequested);

        _fancyDocking = new AzQtComponents::FancyDocking(this);

        auto* const nodesLibrary = new AzQtComponents::StyledDockWidget(tr("Nodes Library"), this);
        nodesLibrary->setObjectName(nodesLibrary->windowTitle());
        nodesLibrary->setWidget(_sidePanel);
        addDockWidget(Qt::LeftDockWidgetArea, nodesLibrary);

        auto* const blackboardProperties = new AzQtComponents::StyledDockWidget(tr("Blackboard Properties"), this);
        blackboardProperties->setObjectName(blackboardProperties->windowTitle());
        blackboardProperties->setWidget(_blackboardProperties);
        addDockWidget(Qt::RightDockWidgetArea, blackboardProperties);

        auto* const nodeProperties = new AzQtComponents::StyledDockWidget(tr("Node Properties"), this);
        nodeProperties->setObjectName(nodeProperties->windowTitle());
        nodeProperties->setWidget(_nodeProperties);
        addDockWidget(Qt::RightDockWidgetArea, nodeProperties);

        _statusBar = new Widgets::StatusBar(this);
        statusBar()->addWidget(_statusBar);

        resize(800, 600);
        setCentralWidget(_tabWidget);

        SetupMenu();

        if (!projectPath.empty())
        {
            _projectPath = QString::fromUtf8(projectPath.Native().data(), aznumeric_cast<int>(projectPath.Native().size()));
        }

        if (!filePath.empty())
        {
            OpenFile(QString::fromUtf8(filePath.Native().data(), aznumeric_cast<int>(filePath.Native().size())));
        }
        else
        {
            OnActionNewTriggered();
        }

        _currentState = SaveCurrentState();
    }

    Widgets::GraphicContainer* MainWindow::CurrentTabInfo()
    {
        const int index = _tabWidget->currentIndex();
        const QString tabName = _tabWidget->tabText(index);
        return GetTabByName(tabName);
    }

    Widgets::GraphicContainer* MainWindow::GetTabByName(const QString& name)
    {
        const auto it = _tabInfo.find(name);
        return it != _tabInfo.end() ? it->second : nullptr;
    }

    void MainWindow::LockEditor(const bool locked) const
    {
        for (const auto& tab : _tabInfo)
        {
            tab.second->lockEditing(locked);
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
            for (const auto& it : _treeNodeModels)
            {
                registeredId.push_back(it.first);
            }

            std::vector<QString> errorMessages;
            if (bool done = Core::VerifyXML(document, registeredId, errorMessages); !done)
            {
                QString mergedError;
                for (const auto& err : errorMessages)
                {
                    mergedError += err + "\n";
                }
                throw std::runtime_error(mergedError.toStdString());
            }
        } catch (std::runtime_error& err)
        {
            QMessageBox::critical(this, "Error parsing the XML", err.what());
            return;
        }

        //---------------
        bool error = false;
        QString errMessage;
        auto savedState = _currentState;
        auto prevTreeModel = _treeNodeModels;

        try
        {
            auto documentRoot = document.documentElement();

            if (documentRoot.hasAttribute("main_tree_to_execute"))
            {
                _mainTree = documentRoot.attribute("main_tree_to_execute");
            }

            auto customModels = Core::ReadTreeNodesModel(documentRoot);

            for (const auto& model : customModels)
            {
                OnAddToModelRegistry(model.second);
            }

            _sidePanel->updateTreeView();

            auto bbProperties = Core::ReadBlackboardPropertiesModel(documentRoot);
            _blackboardModels.clear();

            for (const auto& property : bbProperties)
            {
                _blackboardModels.insert({ property.second.mName, property.second });
            }

            _blackboardProperties->UpdateProperties();

            OnActionClearTriggered(false);

            const QSignalBlocker blocker(CurrentTabInfo());

            for (auto btRoot = documentRoot.firstChildElement("BehaviorTree"); !btRoot.isNull();
                 btRoot = btRoot.nextSiblingElement("BehaviorTree"))
            {
                auto tree = Core::BuildTreeFromXML(btRoot, _treeNodeModels);
                QString treeName("BehaviorTree");

                if (btRoot.hasAttribute("ID"))
                {
                    treeName = btRoot.attribute("ID");
                    if (_mainTree.isEmpty()) // valid when there is only one
                    {
                        _mainTree = treeName;
                    }
                }

                OnCreateAbsBehaviorTree(tree, treeName);
            }

            if (!_mainTree.isEmpty())
            {
                for (int i = 0; i < _tabWidget->count(); i++)
                {
                    if (_tabWidget->tabText(i) == _mainTree)
                    {
                        _tabWidget->tabBar()->moveTab(i, 0);
                        _tabWidget->setCurrentIndex(0);
                        _tabWidget->tabBar()->setTabIcon(0, QIcon(":/icons/green_star.png"));
                        break;
                    }
                }
            }

            if (CurrentTabInfo() == nullptr)
            {
                CreateTab("BehaviorTree");
                _mainTree = "BehaviorTree";
            }
            else
            {
                CurrentTabInfo()->nodeReorder();
            }

            // TODO
            // auto models_to_remove = GetModelsToRemove(this, _treeNodeModels, custom_models);

            // for (QString model_name : models_to_remove)
            // {
            //     OnModelRemoveRequested(model_name);
            // }
        } catch (std::exception& err)
        {
            error = true;
            errMessage = err.what();
        }

        if (error)
        {
            _treeNodeModels = prevTreeModel;
            LoadSavedStateFromJson(savedState);
            qDebug() << "R: Undo size: " << _undoStack.size() << " Redo size: " << _redoStack.size();
            QMessageBox::warning(
                this, tr("Exception!"), tr("It was not possible to parse the file. Error:\n\n%1").arg(errMessage), QMessageBox::Ok);
        }
        else
        {
            OnSceneChanged();
            OnPushUndo();
        }
    }

    void MainWindow::SetCurrentNode(QtNodes::Node* node)
    {
        _nodeProperties->SetNode(node);
    }

    void MainWindow::OnAutoArrange()
    {
        CurrentTabInfo()->nodeReorder();
    }

    void MainWindow::OnSceneChanged()
    {
        // TODO: Enable/Disable toolbox buttons
        // TODO: Enable/Disable save button

        if (const bool isValidTree = CurrentTabInfo()->containsValidTree())
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
        if (!_undoStack.empty())
        {
            _redoStack.push_back(std::move(_currentState));
            _currentState = _undoStack.back();
            _undoStack.pop_back();

            LoadSavedStateFromJson(_currentState);
        }
    }

    void MainWindow::OnRedoInvoked()
    {
        if (!_redoStack.empty())
        {
            _undoStack.push_back(_currentState);
            _currentState = std::move(_redoStack.back());
            _redoStack.pop_back();

            LoadSavedStateFromJson(_currentState);
        }
    }

    void MainWindow::OnTabSetMainTree(int index)
    {
        for (int i = 0; i < _tabWidget->count(); i++)
        {
            if (i == index)
            {
                // TODO: Mark this tab as active (maybe with an icon?)
                _mainTree = _tabWidget->tabBar()->tabText(i);
            }
            else
            {
                _tabWidget->tabBar()->setTabIcon(i, QIcon());
            }
        }

        _tabWidget->setCurrentIndex(index);
    }

    void MainWindow::OnTabCustomContextMenuRequested(const QPoint& pos)
    {
        int index = _tabWidget->tabBar()->tabAt(pos);
        const QString name = _tabWidget->tabText(index);
        const bool isMainTree = _mainTree == name;

        QMenu menu(this);

        // ------------
        const QAction* save = menu.addAction("Save");
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
        const QAction* rename = menu.addAction("Rename");
        connect(
            rename, &QAction::triggered, this,
            [this, index]()
            {
                OnTabRenameRequested(index);
            });
        // ------------

        const QPoint globalPos = _tabWidget->tabBar()->mapToGlobal(pos);
        menu.exec(globalPos);
    }

    void MainWindow::OnActionClearTriggered(bool createNew)
    {
        for (const auto& it : _tabInfo)
        {
            it.second->clearScene();
            it.second->deleteLater();
        }

        _tabInfo.clear();
        _openedFile.clear();
        _statusBar->SetOpenedFileName("Unsaved File");

        _tabWidget->clear();
        _sidePanel->clear();

        if (createNew)
        {
            CreateTab("BehaviorTree");
            OnTabSetMainTree(0);
            ClearUndoStacks();
        }
    }

    void MainWindow::OnPushUndo()
    {
        const SavedState saved = SaveCurrentState();

        if (_undoStack.empty() || saved != _currentState && _undoStack.back() != _currentState)
        {
            _undoStack.push_back(std::move(_currentState));
            _redoStack.clear();
        }

        _currentState = saved;
    }

    void MainWindow::OnCreateAbsBehaviorTree(const Core::AbstractBehaviorTree& tree, const QString& btName, bool secondaryTabs)
    {
        auto container = GetTabByName(btName);
        if (!container)
        {
            container = CreateTab(btName);
        }

        const QSignalBlocker blocker(container);
        container->loadSceneFromTree(tree);
        container->nodeReorder();

        if (secondaryTabs)
        {
            for (const auto& node : tree.Nodes())
            {
                if (node.model.type == Core::NodeType::SUBTREE && GetTabByName(node.model.registrationId) == nullptr)
                {
                    CreateTab(node.model.registrationId);
                }
            }
        }

        ClearUndoStacks();
    }

    void MainWindow::OnActionNewTriggered()
    {
        if (CheckDirty(tr("You will lost your changes if you create a new file, do you want to save the current one?")))
        {
            OnActionClearTriggered(true);
        }
    }

    bool MainWindow::OnActionSaveTriggered()
    {
        return SaveFile(false);
    }

    bool MainWindow::OnActionSaveAsTriggered()
    {
        return SaveFile(true);
    }

    void MainWindow::OnActionLoadTriggered()
    {
        const QSettings settings;
        const QString directoryPath = settings.value("MainWindow.lastLoadDirectory", _projectPath).toString();

        const QString fileName =
            QFileDialog::getOpenFileName(this, tr("Load Behavior Tree from file"), directoryPath, tr("Behavior Tree files (*.bhbtree)"));
        if (!QFileInfo::exists(fileName))
        {
            return;
        }

        if (_openedFile == fileName)
        {
            return;
        }

        OpenFile(fileName);
    }

    void MainWindow::OnActionQuitTriggered()
    {
        if (CheckDirty(tr("Do you want to save the current file before closing?")))
        {
            close();
        }
    }

    void MainWindow::OnAddToModelRegistry(const Core::NodeModel& model)
    {
        namespace Util = QtNodes::detail;
        const auto& id = model.registrationId;

        const QtNodes::DataModelRegistry::RegistryItemCreator nodeCreator = [model]() -> QtNodes::DataModelRegistry::RegistryItemPtr
        {
            if (model.type == Core::NodeType::SUBTREE)
            {
                return Util::make_unique<Core::Models::SubtreeNodeModel>(model);
            }
            return Util::make_unique<Core::Models::SSBehaviorTreeNodeDataModel>(model);
        };

        _modelRegistry->registerModel(QString::fromStdString(BT::toStr(model.type)), nodeCreator, id);

        _treeNodeModels.insert({ id, model });
        _sidePanel->updateTreeView();
    }

    void MainWindow::OnTabRenameRequested(int tabIndex, QString newName)
    {
        const QString oldName = _tabWidget->tabText(tabIndex);

        if (newName.isEmpty())
        {
            bool ok = false;
            newName = QInputDialog::getText(
                this, tr("Change Behavior Tree Name"), tr("Insert the new name of this Behavior Tree"), QLineEdit::Normal, oldName, &ok);

            if (!ok)
            {
                return;
            }
        }

        if (newName == oldName)
        {
            return;
        }

        if (GetTabByName(newName))
        {
            QMessageBox::warning(
                this, "Tab name already is use",
                tr("There is already a Behavior Tree called [%1].\n"
                   "Use another name.")
                    .arg(newName),
                QMessageBox::Ok);
            return;
        }

        _tabWidget->setTabText(tabIndex, newName);
        const auto it = _tabInfo.find(oldName);
        auto container = it->second;
        _tabInfo.insert({ newName, container });
        _tabInfo.erase(it);
        if (_mainTree == oldName)
        {
            _mainTree = newName;
        }

        // if a subtree SUBTREE already
        if (_modelRegistry->registeredModelsCategoryAssociation().count(oldName) != 0)
        {
            // _model_registry->unregisterModel(old_name);
            _treeNodeModels.erase(oldName);
            Core::NodeModel model = { Core::NodeType::SUBTREE, newName, {} };
            OnAddToModelRegistry(model);
            _treeNodeModels.insert({ newName, model });
            _sidePanel->updateTreeView();
            OnTreeNodeEdited(oldName, newName);
        }

        // TODO: this is a work around until we find a better solution
        ClearUndoStacks();
    }

    void MainWindow::OnTreeNodeEdited(const QString& prevId, const QString& newId)
    {
        for (const auto& it : _tabInfo)
        {
            const auto container = it.second;
            std::vector<QtNodes::Node*> nodesToRename;

            for (const auto& nodeIt : container->scene()->nodes())
            {
                QtNodes::Node* graphicNode = nodeIt.second.get();
                if (!graphicNode)
                {
                    continue;
                }

                const auto btNode = dynamic_cast<Core::Models::SSBehaviorTreeNodeDataModel*>(graphicNode->nodeDataModel());
                if (!btNode)
                {
                    continue;
                }

                if (btNode->model().registrationId == prevId)
                {
                    nodesToRename.push_back(graphicNode);
                }
            }

            for (const auto& graphicNode : nodesToRename)
            {
                const auto btNode = dynamic_cast<Core::Models::SSBehaviorTreeNodeDataModel*>(graphicNode->nodeDataModel());
                bool isExpandedSubtree = false;

                if (btNode->model().type == Core::NodeType::SUBTREE)
                {
                    if (const auto subtreeModel = dynamic_cast<Core::Models::SubtreeNodeModel*>(btNode);
                        subtreeModel && subtreeModel->expanded())
                    {
                        isExpandedSubtree = true;
                        SubTreeExpand(*container, *graphicNode, SUBTREE_COLLAPSE);
                    }
                }

                const auto newNode = container->substituteNode(graphicNode, newId);

                if (isExpandedSubtree)
                {
                    SubTreeExpand(*container, *newNode, SUBTREE_EXPAND);
                }
            }
        }
    }

    void MainWindow::SetupMenu()
    {
        QMenuBar* mb = menuBar();

        QMenu* fileMenu = mb->addMenu(tr("&File"));
        QMenu* editMenu = mb->addMenu(tr("&Edit"));
        // QMenu* helpMenu = mb->addMenu(tr("&Help"));

        fileMenu->addAction(tr("&New"), this, &MainWindow::OnActionNewTriggered, QKeySequence(Qt::CTRL + Qt::Key_N));
        fileMenu->addAction(tr("&Open..."), this, &MainWindow::OnActionLoadTriggered, QKeySequence(Qt::CTRL + Qt::Key_O));
        fileMenu->addAction(tr("&Save"), this, &MainWindow::OnActionSaveTriggered, QKeySequence(Qt::CTRL + Qt::Key_S));
        fileMenu->addAction(tr("Save &As..."), this, &MainWindow::OnActionSaveAsTriggered, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
        fileMenu->addSeparator();
        fileMenu->addAction(tr("&Quit"), this, &MainWindow::OnActionQuitTriggered, QKeySequence(Qt::CTRL + Qt::Key_Q));

        editMenu->addAction(tr("&Undo"), this, &MainWindow::OnUndoInvoked, QKeySequence(Qt::CTRL + Qt::Key_Z));
        editMenu->addAction(tr("&Redo"), this, &MainWindow::OnRedoInvoked, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
        editMenu->addSeparator();
        editMenu->addAction(tr("Rearrange &Graph"), this, &MainWindow::OnAutoArrange, QKeySequence(Qt::CTRL + Qt::Key_G, Qt::Key_A));
    }

    Widgets::GraphicContainer* MainWindow::CreateTab(const QString& name, bool setActive)
    {
        if (_tabInfo.count(name) > 0)
        {
            AZ_Warning("SSBehaviorTreeEditor", false, "There is already a Tab named %s", name.toStdString().c_str());
            return _tabInfo[name];
        }

        auto* ti = new Widgets::GraphicContainer(_modelRegistry, this);
        ti->setObjectName(name);
        _tabInfo.insert({ name, ti });

        ti->scene()->setLayout(_currentLayout);

        const int index = _tabWidget->addTab(ti->view(), name);

        if (setActive)
        {
            _tabWidget->setCurrentIndex(index);
        }

        ti->scene()->CreateNodeAtPosition("Root", "Root", QPointF(0, 0));
        ti->zoomHomeView();
        ti->lockEditing(false);

        //--------------------------------

        connect(ti, &Widgets::GraphicContainer::undoableChange, this, &MainWindow::OnPushUndo);
        connect(ti, &Widgets::GraphicContainer::undoableChange, this, &MainWindow::OnSceneChanged);
        // connect(ti, &Widgets::GraphicContainer::requestSubTreeExpand, this, &MainWindow::OnRequestSubTreeExpand);
        connect(ti, &Widgets::GraphicContainer::addNewModel, this, &MainWindow::OnAddToModelRegistry);
        connect(
            ti, &Widgets::GraphicContainer::requestSubTreeCreate, this,
            [this](const Core::AbstractBehaviorTree& tree, const QString& bt_name)
            {
                OnCreateAbsBehaviorTree(tree, bt_name, false);
            });

        return ti;
    }

    std::shared_ptr<QtNodes::DataModelRegistry> MainWindow::CreateDataModelRegistry()
    {
        EBUS_EVENT_RESULT(m_factory, BehaveBehaviorTreeRequestBus, GetFactory);

        Nodes::RegisterDefaultNodes(m_factory.GetRegistry());
        Nodes::RegisterDefaultProperties(m_factory.GetRegistry());

        // Enable all registered nodes
        m_factory.GetRegistry()->EnableNodes();

        auto ret = std::make_shared<QtNodes::DataModelRegistry>();

        auto registerModel = [&ret](const QString& id, const Core::NodeModel& model)
        {
            QString category = QString::fromStdString(BT::toStr(model.type));

            if (id == "Root")
            {
                category = "Root";
            }

            const QtNodes::DataModelRegistry::RegistryItemCreator creator = [model]() -> QtNodes::DataModelRegistry::RegistryItemPtr
            {
                return std::make_unique<Core::Models::SSBehaviorTreeNodeDataModel>(model);
            };

            ret->registerModel(category, creator, id);
        };

        for (const auto& model : Core::BuiltInNodeModels(m_factory))
        {
            registerModel(model.first, model.second);
            _treeNodeModels.insert({ model.first, model.second });
        }

        return ret;
    }

    void MainWindow::LoadSavedStateFromJson(SavedState state)
    {
        // TODO crash if the name of the container (tab) changed
        for (const auto& it : _tabInfo)
        {
            it.second->clearScene();
            it.second->deleteLater();
        }

        _tabInfo.clear();
        _tabWidget->clear();

        _mainTree = state.mMainTree;

        for (const auto& it : state.mJsonStates)
        {
            QString tabName = it.first;
            _tabInfo.insert({ tabName, CreateTab(tabName) });
        }
        for (const auto& it : state.mJsonStates)
        {
            QString name = it.first;
            auto* container = GetTabByName(name);
            container->loadFromJson(it.second);
            container->view()->setTransform(state.mViewTransform);
            container->view()->setSceneRect(state.mViewArea);
        }

        for (int i = 0; i < _tabWidget->count(); i++)
        {
            if (_tabWidget->tabText(i) == state.mCurrentTabName)
            {
                _tabWidget->setCurrentIndex(i);
                _tabWidget->widget(i)->setFocus();
            }
            if (_tabWidget->tabText(i) == _mainTree)
            {
                OnTabSetMainTree(i);
            }
        }

        if (_tabWidget->count() == 1)
        {
            OnTabSetMainTree(0);
        }

        OnSceneChanged();
    }

    MainWindow::SavedState MainWindow::SaveCurrentState()
    {
        SavedState saved;
        const int index = _tabWidget->currentIndex();
        saved.mMainTree = _mainTree;
        saved.mCurrentTabName = _tabWidget->tabText(index);
        const auto currentView = GetTabByName(saved.mCurrentTabName)->view();
        saved.mViewTransform = currentView->transform();
        saved.mViewArea = currentView->sceneRect();

        for (const auto& it : _tabInfo)
        {
            saved.mJsonStates[it.first] = it.second->scene()->saveToMemory();
        }

        _dirtyFile = true;

        return saved;
    }

    void MainWindow::ClearUndoStacks()
    {
        _undoStack.clear();
        _redoStack.clear();
        OnSceneChanged();
        OnPushUndo();
    }

    QString MainWindow::SaveToXml() const
    {
        QDomDocument doc;

        static constexpr auto COMMENT_SEPARATOR = " ////////// ";

        QDomElement root = doc.createElement("root");
        doc.appendChild(root);

        if (_mainTree.isEmpty() == false)
        {
            root.setAttribute("main_tree_to_execute", _mainTree.toStdString().c_str());
        }

        for (const auto& it : _tabInfo)
        {
            auto& container = it.second;
            const auto scene = container->scene();

            auto absTree = Core::BuildTreeFromScene(container->scene());
            auto absRoot = absTree.RootNode();

            if (absRoot->childrenIndex.size() == 1 && absRoot->model.registrationId == "Root")
            {
                // Move to the child of ROOT
                absRoot = absTree.Node(absRoot->childrenIndex.front());
            }

            const QtNodes::Node* rootNode = absRoot->graphicNode;

            root.appendChild(doc.createComment(COMMENT_SEPARATOR));
            QDomElement rootElement = doc.createElement("BehaviorTree");

            rootElement.setAttribute("ID", it.first.toStdString().c_str());
            root.appendChild(rootElement);

            Core::RecursivelyCreateXml(*scene, doc, rootElement, rootNode, m_factory);
        }

        QDomElement rootModels = doc.createElement("TreeNodesModel");

        for (const auto& treeIt : _treeNodeModels)
        {
            const auto& id = treeIt.first;
            const auto& model = treeIt.second;

            if (Core::BuiltInNodeModels(m_factory).count(id) != 0)
            {
                continue;
            }

            QDomElement node = doc.createElement(QString::fromStdString(toStr(model.type)));

            if (!node.isNull())
            {
                node.setAttribute("ID", id);

                for (const auto& portIt : model.ports)
                {
                    const auto& portName = portIt.first;
                    const auto& port = portIt.second;

                    QDomElement portElement = writePortModel(portName, port, doc);
                    node.appendChild(portElement);
                }
            }

            rootModels.appendChild(node);
        }

        QDomElement rootProperties = doc.createElement("Blackboard");

        for (const auto& property : _blackboardModels)
        {
            QDomElement propertyElement = doc.createElement("Property");

            if (!propertyElement.isNull())
            {
                propertyElement.setAttribute("name", property.second.mName);
                propertyElement.setAttribute("description", property.second.mDescription);
                propertyElement.setAttribute("type", property.second.mType);
                propertyElement.setAttribute("suffix", property.second.mSuffix);
                propertyElement.setAttribute("private", property.second.mIsPrivate ? "true" : "false");
                propertyElement.setAttribute("order", property.second.mOrder);
            }

            rootProperties.appendChild(propertyElement);
        }

        root.appendChild(doc.createComment(COMMENT_SEPARATOR));
        root.appendChild(rootModels);
        root.appendChild(doc.createComment(COMMENT_SEPARATOR));
        root.appendChild(rootProperties);
        root.appendChild(doc.createComment(COMMENT_SEPARATOR));

        return XmlDocumentToString(doc);
    }

    QString MainWindow::XmlDocumentToString(const QDomDocument& document) const
    {
        QString outputString;

        QXmlStreamWriter stream(&outputString);

        stream.setAutoFormatting(true);
        stream.setAutoFormattingIndent(4);

        stream.writeStartDocument();

        const auto rootElement = document.documentElement();

        stream.writeStartElement(rootElement.tagName());

        StreamElementAttributes(stream, rootElement);

        auto nextNode = rootElement.firstChild();

        while (!nextNode.isNull())
        {
            RecursivelySaveNodeCanonically(stream, nextNode);

            if (stream.hasError())
            {
                break;
            }

            nextNode = nextNode.nextSibling();
        }

        stream.writeEndElement();
        stream.writeEndDocument();

        return outputString;
    }

    void MainWindow::StreamElementAttributes(QXmlStreamWriter& stream, const QDomElement& element) const
    {
        if (element.hasAttributes())
        {
            QMap<QString, QString> attributes;
            const QDomNamedNodeMap attributesMap = element.attributes();

            for (int i = 0; i < attributesMap.count(); ++i)
            {
                auto attribute = attributesMap.item(i);
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

    void MainWindow::RecursivelySaveNodeCanonically(QXmlStreamWriter& stream, const QDomNode& parentNode) const
    {
        if (stream.hasError())
        {
            return;
        }

        if (parentNode.isElement())
        {
            if (const QDomElement parentElement = parentNode.toElement(); !parentElement.isNull())
            {
                stream.writeStartElement(parentElement.tagName());

                StreamElementAttributes(stream, parentElement);

                if (parentElement.hasChildNodes())
                {
                    auto child = parentElement.firstChild();
                    while (!child.isNull())
                    {
                        RecursivelySaveNodeCanonically(stream, child);
                        child = child.nextSibling();
                    }
                }

                stream.writeEndElement();
            }
        }
        else if (parentNode.isComment())
        {
            stream.writeComment(parentNode.nodeValue());
        }
        else if (parentNode.isText())
        {
            stream.writeCharacters(parentNode.nodeValue());
        }
    }

    bool MainWindow::SavedState::operator==(const MainWindow::SavedState& other) const
    {
        if (mCurrentTabName != other.mCurrentTabName || mJsonStates.size() != other.mJsonStates.size())
        {
            return false;
        }

        for (const auto& it : mJsonStates)
        {
            if (auto otherIt = other.mJsonStates.find(it.first); otherIt == other.mJsonStates.end() || it.second != otherIt->second)
            {
                return false;
            }
        }

        if (mViewArea != other.mViewArea || mViewTransform != other.mViewTransform)
        {
            return false;
        }

        return true;
    }

    QtNodes::Node* MainWindow::SubTreeExpand(
        Widgets::GraphicContainer& container, QtNodes::Node& node, MainWindow::SubtreeExpandOption option)
    {
        constexpr bool isEditorMode = true;
        const QSignalBlocker blocker(this);
        const auto subtreeModel = dynamic_cast<Core::Models::SubtreeNodeModel*>(node.nodeDataModel());
        const QString& subtreeName = subtreeModel->registrationName();

        if (option == SUBTREE_EXPAND && subtreeModel->expanded() == false)
        {
            const auto subtreeContainer = GetTabByName(subtreeName);

            // Prevent expansion of invalid subtree
            if (!subtreeContainer->containsValidTree())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Invalid SubTree. Can not expand SubTree."), QMessageBox::Cancel);
                return &node;
            }

            auto abstractBehaviorTree = BuildTreeFromScene(subtreeContainer->scene());

            subtreeModel->setExpanded(true);
            node.nodeState().getEntries(QtNodes::PortType::Out).resize(1);
            container.appendTreeToNode(node, abstractBehaviorTree);
            container.lockSubtreeEditing(node, true, isEditorMode);

            if (abstractBehaviorTree.Nodes().size() > 1)
            {
                container.nodeReorder();
            }

            return &node;
        }

        if (option == SUBTREE_COLLAPSE && subtreeModel->expanded() == true)
        {
            bool needReorder = true;
            const auto& connOut = node.nodeState().connections(QtNodes::PortType::Out, 0);
            QtNodes::Node* childNode = nullptr;
            if (connOut.size() == 1)
            {
                childNode = connOut.begin()->second->getNode(QtNodes::PortType::In);
            }

            const QSignalBlocker b2(container);
            if (childNode)
            {
                container.deleteSubTreeRecursively(*childNode);
            }
            else
            {
                needReorder = false;
            }

            subtreeModel->setExpanded(false);
            node.nodeState().getEntries(QtNodes::PortType::Out).resize(0);
            container.lockSubtreeEditing(node, false, isEditorMode);
            if (needReorder)
            {
                container.nodeReorder();
            }

            return &node;
        }

        if (option == SUBTREE_REFRESH && subtreeModel->expanded() == true)
        {
            const auto& connOut = node.nodeState().connections(QtNodes::PortType::Out, 0);
            if (connOut.size() != 1)
            {
                throw std::logic_error("subTreeExpand with SUBTREE_REFRESH, but not an expanded SubTree");
            }

            QtNodes::Node* childNode = connOut.begin()->second->getNode(QtNodes::PortType::In);

            const auto subtreeContainer = GetTabByName(subtreeName);
            auto subtree = Core::BuildTreeFromScene(subtreeContainer->scene());

            container.deleteSubTreeRecursively(*childNode);
            container.appendTreeToNode(node, subtree);
            container.nodeReorder();
            container.lockSubtreeEditing(node, true, isEditorMode);

            return &node;
        }

        return nullptr;
    }

    bool MainWindow::SaveFile(bool overwrite)
    {
        for (const auto& it : _tabInfo)
        {
            if (auto& container = it.second; !container->containsValidTree())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Malformed behavior tree. File can not be saved."), QMessageBox::Cancel);
                return false;
            }
        }

        if (_tabInfo.size() == 1)
        {
            _mainTree = _tabInfo.begin()->first;
        }

        QSettings settings;
        QString directoryPath = settings.value("MainWindow.lastSaveDirectory", _projectPath).toString();

        QString fileName;

        if (overwrite || _openedFile.isNull())
        {
            fileName = QFileDialog::getSaveFileName(this, "Save Behavior Tree to file", directoryPath, "Behavior Tree files (*.bhbtree)");
            if (fileName.isEmpty())
            {
                return false;
            }
            if (!fileName.endsWith(".bhbtree"))
            {
                fileName += ".bhbtree";
            }
        }
        else
        {
            fileName = _openedFile;
        }

        const QString xmlText = SaveToXml();

        const QFileInfo fileInfo(fileName);
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << xmlText << Qt::endl;
        }

        directoryPath = fileInfo.absolutePath();
        settings.setValue("MainWindow.lastSaveDirectory", directoryPath);

        _openedFile = fileName;
        _statusBar->SetOpenedFileName(fileInfo.baseName());

        _dirtyFile = false;

        return true;
    }

    bool MainWindow::OpenFile(const QString& filename)
    {
        const QFileInfo fileInfo(filename);
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QSettings settings;
        const QString directoryPath = fileInfo.absolutePath();
        settings.setValue("MainWindow.lastLoadDirectory", directoryPath);
        settings.sync();

        QString xmlText;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            xmlText += in.readLine();
        }

        CreateTab(fileInfo.baseName());
        LoadFromXML(xmlText);

        _openedFile = filename;
        _statusBar->SetOpenedFileName(fileInfo.baseName());

        return true;
    }

    bool MainWindow::CheckDirty(const QString& message)
    {
        if (_dirtyFile)
        {
            if (const QMessageBox::StandardButton result = QMessageBox::warning(
                    this, "The current file have unsaved changes", message, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                    QMessageBox::Save);
                result == QMessageBox::Save)
            {
                return OnActionSaveTriggered();
            }
            else if (result == QMessageBox::Cancel)
            {
                return false;
            }
        }

        return true;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Windows

#include <Editor/Windows/moc_MainWindow.cpp>

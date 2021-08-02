#include <Editor/Application/precompiled.h>

#include <QHeaderView>
#include <QMenu>
#include <QSettings>
#include <QTableWidgetItem>

#include <AzQtComponents/Components/Widgets/LineEdit.h>

#include <Editor/Widgets/NodesSidePanel.h>
#include <Editor/Widgets/ui_NodesSidePanel.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    NodesSidePanel::NodesSidePanel(const std::shared_ptr<QtNodes::DataModelRegistry>& registry, NodeModels& models, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NodesSidePanel)
        , _tree_nodes_model(models)
        , _model_registry(registry)
    {
        ui->setupUi(this);

        ui->nodeParamsFrame->setHidden(true);
        ui->nodesListTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        auto table_header = ui->nodeParamsTableWidget->horizontalHeader();

        table_header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        table_header->setSectionResizeMode(1, QHeaderView::Interactive);
        table_header->setSectionResizeMode(2, QHeaderView::Stretch);

        QSettings settings;
        table_header->restoreState(settings.value("SparkyStudios/AI/BehaviorTree/Editor/NodesSidePanel/header").toByteArray());

        AzQtComponents::LineEdit::applySearchStyle(ui->nodesListFilter);

        connect(
            ui->nodesListTreeWidget, &QTreeWidget::itemSelectionChanged, this,
            &NodesSidePanel::on_nodesListTreeWidget_itemSelectionChanged);
        connect(ui->nodesListFilter, &QLineEdit::textChanged, this, &NodesSidePanel::on_nodesListFilter_textChanged);
    }

    NodesSidePanel::~NodesSidePanel()
    {
        QSettings settings;
        settings.setValue(
            "SparkyStudios/AI/BehaviorTree/Editor/NodesSidePanel/header", ui->nodeParamsTableWidget->horizontalHeader()->saveState());

        delete ui;
    }

    void NodesSidePanel::updateTreeView()
    {
        ui->nodesListTreeWidget->clear();
        _tree_view_category_items.clear();
        _node_category_items.clear();

        for (const QString& category : { "Action", "Condition", "Control", "Decorator", "SubTree" })
        {
            auto item = new QTreeWidgetItem(ui->nodesListTreeWidget, { category });

            QFont font = item->font(0);
            font.setBold(true);

            item->setFont(0, font);
            item->setFlags(item->flags() ^ Qt::ItemIsDragEnabled);
            item->setFlags(item->flags() ^ Qt::ItemIsSelectable);

            _tree_view_category_items[category] = item;
        }

        for (const auto& it : _tree_nodes_model)
        {
            const auto& ID = it.first;
            const NodeModel& model = it.second;

            if (model.registrationId == "Root")
            {
                continue;
            }

            QString category = QString::fromStdString(toStr(model.type));

            auto parent = _tree_view_category_items[category];

            if (!model.category.isEmpty())
            {
                if (_node_category_items.count({ category, model.category }) == 0)
                {
                    auto nodeCategory = new QTreeWidgetItem(parent, { model.category });

                    QFont font = nodeCategory->font(0);
                    font.setBold(true);

                    nodeCategory->setFont(0, font);
                    nodeCategory->setFlags(nodeCategory->flags() ^ Qt::ItemIsDragEnabled);
                    nodeCategory->setFlags(nodeCategory->flags() ^ Qt::ItemIsSelectable);

                    _node_category_items[{ category, model.category }] = nodeCategory;
                }

                parent = _node_category_items[{ category, model.category }];
            }

            auto item = new QTreeWidgetItem(parent, { ID });

            // const bool is_builtin = Core::BuiltInNodeModels().count(ID) > 0;

            QFont font = item->font(0);

            // font.setItalic(is_builtin);
            item->setFont(0, font);
            item->setData(0, Qt::UserRole, ID);
        }

        ui->nodesListTreeWidget->expandAll();
    }

    void NodesSidePanel::clear()
    {
    }

    void NodesSidePanel::on_nodesListTreeWidget_itemSelectionChanged()
    {
        auto selected_items = ui->nodesListTreeWidget->selectedItems();

        if (selected_items.size() == 0)
        {
            ui->nodeParamsFrame->setHidden(true);
        }
        else
        {
            auto selected_item = selected_items.front();
            QString item_name = selected_item->text(0);
            ui->nodeParamsFrame->setHidden(false);
            ui->nodeParamsTitle->setText(item_name + QString(" Parameters"));

            const auto& model = _tree_nodes_model.at(item_name);

            ui->nodeParamsTableWidget->setRowCount(int(model.ports.size()));

            int row = 0;
            for (const auto& port_it : model.ports)
            {
                ui->nodeParamsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(toStr(port_it.second.direction))));
                ui->nodeParamsTableWidget->setItem(row, 1, new QTableWidgetItem(port_it.first));
                ui->nodeParamsTableWidget->setItem(row, 2, new QTableWidgetItem(port_it.second.description));
                row++;
            }

            ui->nodeParamsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        }
    }

    void NodesSidePanel::on_nodesListFilter_textChanged([[maybe_unused]] const QString& text)
    {
        for (auto& it : _tree_view_category_items)
        {
            for (int i = 0; i < it.second->childCount(); ++i)
            {
                auto child = it.second->child(i);
                auto modelName = child->data(0, Qt::UserRole).toString();
                bool show = modelName.contains(text, Qt::CaseInsensitive);
                child->setHidden(!show);
            }
        }
    }

    void NodesSidePanel::onRemoveModel(QString selected_name)
    {
        NodeType node_type = _tree_nodes_model.at(selected_name).type;

        _tree_nodes_model.erase(selected_name);
        // _model_registry->unregisterModel(selected_name);
        updateTreeView();
        if (node_type == NodeType::SUBTREE)
        {
            emit destroySubtree(selected_name);
        }
    }

    void NodesSidePanel::onReplaceModel(const QString& old_name, const NodeModel& new_model)
    {
        _tree_nodes_model.erase(old_name);
        // _model_registry->unregisterModel(old_name);
        emit addNewModel(new_model);

        if (new_model.type == NodeType::SUBTREE)
        {
            emit renameSubtree(old_name, new_model.registrationId);
        }

        emit nodeModelEdited(old_name, new_model.registrationId);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

#include <Editor/Widgets/moc_NodesSidePanel.cpp>

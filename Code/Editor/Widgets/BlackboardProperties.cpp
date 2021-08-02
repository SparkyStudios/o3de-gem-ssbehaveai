#include <Editor/Application/precompiled.h>

#include <QDialog>
#include <QIcon>
#include <QItemSelection>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QToolButton>

#include <AzQtComponents/Components/Widgets/PushButton.h>

#include <Editor/Widgets/BlackboardProperties.h>
#include <Editor/Widgets/ui_BlackboardProperties.h>

#include <Editor/Windows/BlackboardPropertyDialog.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    BlackboardProperties::BlackboardProperties(Core::BlackboardPropertyModels& models, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::BlackboardProperties)
        , _blackboard_properties_model(models)
        , _main_window(parent)
    {
        ui->setupUi(this);

        AzQtComponents::PushButton::applySmallIconStyle(ui->addPropertyButton);

        ui->propertiesList->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->propertiesList->setIconSize(QSize(16, 16));

        connect(ui->addPropertyButton, &QToolButton::clicked, this, &BlackboardProperties::OnAddProperty);
        connect(ui->propertiesList, &QListWidget::itemDoubleClicked, this, &BlackboardProperties::OnPropertyDoubleClicked);
    }

    BlackboardProperties::~BlackboardProperties()
    {
        delete ui;
    }

    void BlackboardProperties::UpdateProperties()
    {
        ui->propertiesList->clear();

        for (auto&& model : _blackboard_properties_model)
        {
            const auto& ID = model.first;

            QIcon icon(model.second.isPrivate ? ":/icons/blackboard-property-private.png" : ":/icons/blackboard-property-public.png");

            auto* item = new QListWidgetItem(icon, ID);
            item->setToolTip(model.second.description);
            item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
            item->setData(Qt::UserRole, ID);

            ui->propertiesList->addItem(item);
        }
    }

    void BlackboardProperties::OnAddProperty()
    {
        Core::BlackboardPropertyModel model;
        Windows::BlackboardPropertyDialog* dialog = new Windows::BlackboardPropertyDialog(model, _main_window);
        dialog->setWindowTitle("Add Blackboard Property");
        int result = dialog->exec();

        if (result == QDialog::Accepted)
        {
            if (model.name.isEmpty())
            {
                QMessageBox::critical(_main_window, "Error", "Cannot create a property with empty name.");
                return;
            }

            if (_blackboard_properties_model.count(model.name) != 0)
            {
                QMessageBox::critical(_main_window, "Error", "A property with that name already exists. Try again with a different name.");
                return;
            }

            _blackboard_properties_model.insert({ model.name, model });
            UpdateProperties();
        }
    }

    void BlackboardProperties::OnPropertyDoubleClicked(QListWidgetItem* item)
    {
        if (_blackboard_properties_model.count(item->text()) != 0)
        {
            Core::BlackboardPropertyModel& model = _blackboard_properties_model.at(item->text());
            Windows::BlackboardPropertyDialog* dialog = new Windows::BlackboardPropertyDialog(model, _main_window);
            dialog->setWindowTitle("Edit Blackboard Property");
            int result = dialog->exec();

            if (result == QDialog::Accepted)
            {
                _blackboard_properties_model[model.name] = model;
                UpdateProperties();
            }
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

#include <Editor/Widgets/moc_BlackboardProperties.cpp>

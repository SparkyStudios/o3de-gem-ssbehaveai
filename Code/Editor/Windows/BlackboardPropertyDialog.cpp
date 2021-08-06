#include <Editor/Application/precompiled.h>

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

#include <Editor/Windows/BlackboardPropertyDialog.h>
#include <Editor/Windows/ui_BlackboardPropertyDialog.h>

#include <Editor/Windows/MainWindow.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Windows
{
    BlackboardPropertyDialog::BlackboardPropertyDialog(Core::BlackboardPropertyModel& model, QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::BlackboardPropertyDialog)
        , _model(model)
    {
        ui->setupUi(this);

        auto* main_win = dynamic_cast<Windows::MainWindow*>(parent);
        if (!main_win)
        {
            QMessageBox::critical(main_win, "Error", "Unable to find the main window. This dialog will now close.");
            close();
            return;
        }

        AZStd::vector<AZStd::string> properties = main_win->m_factory.GetRegistry()->GetRegisteredProperties();
        for (auto&& property : properties)
        {
            ui->propertyTypeComboBox->addItem(property.c_str());
        }

        ui->propertyNameLineEdit->setText(model.name);
        ui->propertyTypeComboBox->setCurrentText(model.type);
        ui->propertyDescriptionLineEdit->setText(model.description);
        ui->propertySuffixLineEdit->setText(model.suffix);
        ui->isPrivateCheckBox->setChecked(model.isPrivate);

        connect(ui->propertyNameLineEdit, &QLineEdit::textChanged, this, &BlackboardPropertyDialog::OnPropertyNameChanged);
        connect(ui->propertyTypeComboBox, &QComboBox::currentTextChanged, this, &BlackboardPropertyDialog::OnPropertyTypeChanged);
        connect(ui->propertyDescriptionLineEdit, &QLineEdit::textChanged, this, &BlackboardPropertyDialog::OnPropertyDescriptionChanged);
        connect(ui->propertySuffixLineEdit, &QLineEdit::textChanged, this, &BlackboardPropertyDialog::OnPropertySuffixChanged);
        connect(ui->isPrivateCheckBox, &QCheckBox::stateChanged, this, &BlackboardPropertyDialog::OnPropertyIsPrivateChanged);
    }

    BlackboardPropertyDialog::~BlackboardPropertyDialog()
    {
        delete ui;
    }

    void BlackboardPropertyDialog::OnPropertyNameChanged(const QString& name)
    {
        _model.name = name;
    }

    void BlackboardPropertyDialog::OnPropertyTypeChanged(const QString& type)
    {
        _model.type = type;
    }

    void BlackboardPropertyDialog::OnPropertyDescriptionChanged(const QString& description)
    {
        _model.description = description;
    }

    void BlackboardPropertyDialog::OnPropertySuffixChanged(const QString& suffix)
    {
        _model.suffix = suffix;
    }

    void BlackboardPropertyDialog::OnPropertyIsPrivateChanged(int isPrivate)
    {
        _model.isPrivate = isPrivate == Qt::Checked;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Windows

#include <Editor/Windows/moc_BlackboardPropertyDialog.cpp>
#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>

#include <QDialog>

#include <Editor/Core/EditorBase.h>
#endif

namespace Ui
{
    class BlackboardPropertyDialog;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Windows
{
    class BlackboardPropertyDialog : public QDialog
    {
        Q_OBJECT

    public:
        BlackboardPropertyDialog(Core::BlackboardPropertyModel& model, QWidget* parent = nullptr);
        ~BlackboardPropertyDialog();

    private slots:
        void OnPropertyNameChanged(const QString& name);
        void OnPropertyTypeChanged(const QString& type);
        void OnPropertyDescriptionChanged(const QString& description);
        void OnPropertySuffixChanged(const QString& suffix);
        void OnPropertyIsPrivateChanged(int suffix);

    private:
        Ui::BlackboardPropertyDialog* ui;
        Core::BlackboardPropertyModel& _model;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Windows

#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>

#include <QListWidgetItem>
#include <QWidget>

#include <Editor/Core/EditorBase.h>
#endif

namespace Ui
{
    class BlackboardProperties;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    class BlackboardProperties : public QWidget
    {
        Q_OBJECT

    public:
        explicit BlackboardProperties(Core::BlackboardPropertyModels& models, QWidget* parent);
        ~BlackboardProperties() override;

        void UpdateProperties();

    public slots:
        void OnAddProperty();
        void OnPropertyDoubleClicked(QListWidgetItem* item);

    private:
        Ui::BlackboardProperties* ui;
        Core::BlackboardPropertyModels& _blackboard_properties_model;

        QWidget* _main_window;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

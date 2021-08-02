#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>

#include <QWidget>
#endif

namespace Ui
{
    class StatusBar;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    class StatusBar : public QWidget
    {
        Q_OBJECT

    public:
        explicit StatusBar(QWidget* parent);
        ~StatusBar();

        void SetOpenedFileName(const QString& filename);

    public slots:
        void OnValidTree();
        void OnInvalidTree();

    private:
        Ui::StatusBar* ui;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

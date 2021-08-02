#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>

#include <QWidget>
#endif

namespace Ui
{
    class NodeProperties;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    class NodeProperties : public QWidget
    {
        Q_OBJECT

    public:
        explicit NodeProperties(QWidget* parent = nullptr);
        ~NodeProperties() override;

        void SetNode(QtNodes::Node* node);

    private:
        Ui::NodeProperties* ui;

        QtNodes::Node* _currentNode;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

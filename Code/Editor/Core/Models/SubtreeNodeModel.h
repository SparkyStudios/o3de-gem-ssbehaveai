#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Core/Models/SSBehaviorTreeNodeDataModel.h>
#include <QPushButton>
#endif

namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models
{
    class SubtreeNodeModel : public SSBehaviorTreeNodeDataModel
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(SubtreeNodeModel, AZ::SystemAllocator, 0);
        AZ_RTTI(SubtreeNodeModel, "{a2f26d29-43d9-4456-995c-74c594ada49f}", SSBehaviorTreeNodeDataModel);

        SubtreeNodeModel(const NodeModel& model);

        ~SubtreeNodeModel() override = default;

        void setExpanded(bool expand);

        bool expanded() const
        {
            return _expanded;
        }

        unsigned int nPorts(QtNodes::PortType portType) const override
        {
            int out_port = _expanded ? 1 : 0;
            return portType == QtNodes::PortType::In ? 1 : out_port;
        }

        virtual const char* className() const final
        {
            return Name();
        }

        static const char* Name()
        {
            return "SubTree";
        }

        QPushButton* expandButton()
        {
            return _expand_button;
        }

        virtual void setInstanceName(const QString& name) override;

        QJsonObject save() const override;

        void restore(QJsonObject const&) override;

    signals:
        void expandButtonPushed();

    private:
        QPushButton* _expand_button;
        bool _expanded;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models

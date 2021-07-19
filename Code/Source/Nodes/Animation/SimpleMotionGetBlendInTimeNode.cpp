#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    SimpleMotionGetBlendInTimeNode::SimpleMotionGetBlendInTimeNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionGetBlendInTimeNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void SimpleMotionGetBlendInTimeNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionGetBlendInTimeNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionGetBlendInTimeNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({ BT::OutputPort<float>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION) }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionGetBlendInTimeNode::Tick()
    {
        float value;
        EBUS_EVENT_ID_RESULT(value, GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, GetBlendInTime);
        SetOutputValue<float>(NODE_PORT_VALUE_NAME, value);

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

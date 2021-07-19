#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    SimpleMotionGetPlaySpeedNode::SimpleMotionGetPlaySpeedNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionGetPlaySpeedNode::Reflect(AZ::ReflectContext* reflection)
    {
        AZ_UNUSED(reflection);
    }

    void SimpleMotionGetPlaySpeedNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionGetPlaySpeedNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionGetPlaySpeedNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::OutputPort<float>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionGetPlaySpeedNode::Tick()
    {
        float value;
        EBUS_EVENT_ID_RESULT(value, GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, GetPlaySpeed);
        SetOutputValue<float>(NODE_PORT_VALUE_NAME, value);

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

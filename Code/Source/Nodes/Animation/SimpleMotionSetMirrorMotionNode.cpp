#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    SimpleMotionSetMirrorMotionNode::SimpleMotionSetMirrorMotionNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionSetMirrorMotionNode::Reflect(AZ::ReflectContext* reflection)
    {
        AZ_UNUSED(reflection);
    }

    void SimpleMotionSetMirrorMotionNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionSetMirrorMotionNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionSetMirrorMotionNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<bool>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionSetMirrorMotionNode::Tick()
    {
        Core::Optional<bool> value = GetInputValue<bool>(NODE_PORT_VALUE_NAME);

        if (value.has_value())
        {
            EBUS_EVENT_ID(GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, MirrorMotion, value.value());
            return Core::SSBehaviorTreeNodeStatus::SUCCESS;
        }

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

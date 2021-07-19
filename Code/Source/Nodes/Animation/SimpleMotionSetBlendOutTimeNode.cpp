#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    SimpleMotionSetBlendOutTimeNode::SimpleMotionSetBlendOutTimeNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionSetBlendOutTimeNode::Reflect(AZ::ReflectContext* reflection)
    {
        AZ_UNUSED(reflection);
    }

    void SimpleMotionSetBlendOutTimeNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionSetBlendOutTimeNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionSetBlendOutTimeNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<float>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionSetBlendOutTimeNode::Tick()
    {
        Core::Optional<float> value = GetInputValue<float>(NODE_PORT_VALUE_NAME);

        if (value.has_value())
        {
            EBUS_EVENT_ID(GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, BlendOutTime, value.value());
            return Core::SSBehaviorTreeNodeStatus::SUCCESS;
        }

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

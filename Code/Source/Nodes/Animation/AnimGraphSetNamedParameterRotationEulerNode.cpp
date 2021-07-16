#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterRotationEulerNode::AnimGraphSetNamedParameterRotationEulerNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphSetNamedParameterRotationEulerNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterRotationEulerNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterRotationEulerNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterRotationEulerNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphSetNamedParameterRotationEulerNode::SetParameter()
    {
        Core::Optional<AZ::Vector3> value = GetInputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterRotationEuler, m_parameterIndex,
                value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

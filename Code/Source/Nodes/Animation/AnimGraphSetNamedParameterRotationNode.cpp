#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterRotationNode::AnimGraphSetNamedParameterRotationNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZ::Quaternion>(name, config)
    {
    }

    void AnimGraphSetNamedParameterRotationNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterRotationNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterRotationNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterRotationNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZ::Quaternion>::providedPorts();
    }

    void AnimGraphSetNamedParameterRotationNode::SetParameter()
    {
        Core::Optional<AZ::Quaternion> value = GetInputValue<AZ::Quaternion>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterRotation, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

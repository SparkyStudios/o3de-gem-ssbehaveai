#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterRotationNode::AnimGraphGetNamedParameterRotationNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Quaternion>(name, config)
    {
    }

    void AnimGraphGetNamedParameterRotationNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterRotationNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterRotationNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterRotationNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Quaternion>::providedPorts();
    }

    void AnimGraphGetNamedParameterRotationNode::GetParameter()
    {
        AZ::Quaternion value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterRotation, m_parameterIndex);
        SetOutputValue<AZ::Quaternion>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
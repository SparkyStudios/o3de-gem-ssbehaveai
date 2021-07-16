#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterFloatNode::AnimGraphGetNamedParameterFloatNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<float>(name, config)
    {
    }

    void AnimGraphGetNamedParameterFloatNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterFloatNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterFloatNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterFloatNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<float>::providedPorts();
    }

    void AnimGraphGetNamedParameterFloatNode::GetParameter()
    {
        float value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterFloat, m_parameterIndex);
        SetOutputValue<float>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
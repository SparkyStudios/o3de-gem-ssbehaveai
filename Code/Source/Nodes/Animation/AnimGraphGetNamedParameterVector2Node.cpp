#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterVector2Node::AnimGraphGetNamedParameterVector2Node(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Vector2>(name, config)
    {
    }

    void AnimGraphGetNamedParameterVector2Node::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterVector2Node::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterVector2Node>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterVector2Node::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Vector2>::providedPorts();
    }

    void AnimGraphGetNamedParameterVector2Node::GetParameter()
    {
        AZ::Vector2 value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterVector2, m_parameterIndex);
        SetOutputValue<AZ::Vector2>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
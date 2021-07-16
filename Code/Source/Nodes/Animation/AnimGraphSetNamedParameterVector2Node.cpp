#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterVector2Node::AnimGraphSetNamedParameterVector2Node(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZ::Vector2>(name, config)
    {
    }

    void AnimGraphSetNamedParameterVector2Node::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterVector2Node::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterVector2Node>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterVector2Node::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZ::Vector2>::providedPorts();
    }

    void AnimGraphSetNamedParameterVector2Node::SetParameter()
    {
        Core::Optional<AZ::Vector2> value = GetInputValue<AZ::Vector2>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterVector2, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

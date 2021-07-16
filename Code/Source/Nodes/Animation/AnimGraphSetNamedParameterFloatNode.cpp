#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterFloatNode::AnimGraphSetNamedParameterFloatNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<float>(name, config)
    {
    }

    void AnimGraphSetNamedParameterFloatNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterFloatNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterFloatNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterFloatNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<float>::providedPorts();
    }

    void AnimGraphSetNamedParameterFloatNode::SetParameter()
    {
        Core::Optional<float> value = GetInputValue<float>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterFloat, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

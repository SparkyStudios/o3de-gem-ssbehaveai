#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterBoolNode::AnimGraphSetNamedParameterBoolNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<bool>(name, config)
    {
    }

    void AnimGraphSetNamedParameterBoolNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterBoolNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterBoolNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterBoolNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<bool>::providedPorts();
    }

    void AnimGraphSetNamedParameterBoolNode::SetParameter()
    {
        Core::Optional<bool> value = GetInputValue<bool>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterBool, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

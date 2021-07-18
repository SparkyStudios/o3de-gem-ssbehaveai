#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterStringNode::AnimGraphSetNamedParameterStringNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZStd::string>(name, config)
    {
    }

    void AnimGraphSetNamedParameterStringNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterStringNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterStringNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterStringNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZStd::string>::providedPorts();
    }

    void AnimGraphSetNamedParameterStringNode::SetParameter()
    {
        Core::Optional<AZStd::string> value = GetInputValue<AZStd::string>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterString, m_parameterIndex,
                value.value().c_str());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

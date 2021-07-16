#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterVector3Node::AnimGraphSetNamedParameterVector3Node(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphSetNamedParameterVector3Node::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphSetNamedParameterVector3Node::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterVector3Node>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphSetNamedParameterVector3Node::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphSetNamedParameterVector3Node::SetParameter()
    {
        Core::Optional<AZ::Vector3> value = GetInputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME);
        if (value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterVector3, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

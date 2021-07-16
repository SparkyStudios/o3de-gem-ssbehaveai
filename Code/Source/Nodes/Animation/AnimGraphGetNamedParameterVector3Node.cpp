#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterVector3Node::AnimGraphGetNamedParameterVector3Node(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphGetNamedParameterVector3Node::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterVector3Node::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterVector3Node>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterVector3Node::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphGetNamedParameterVector3Node::GetParameter()
    {
        AZ::Vector3 value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterVector3, m_parameterIndex);
        SetOutputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
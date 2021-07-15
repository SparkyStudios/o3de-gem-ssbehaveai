#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterStringNode::AnimGraphGetNamedParameterStringNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZStd::string>(name, config)
    {
    }

    void AnimGraphGetNamedParameterStringNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterStringNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterStringNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterStringNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZStd::string>::providedPorts();
    }

    void AnimGraphGetNamedParameterStringNode::GetParameter()
    {
        AZStd::string value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterString, m_parameterIndex);
        SetOutputValue<AZStd::string>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
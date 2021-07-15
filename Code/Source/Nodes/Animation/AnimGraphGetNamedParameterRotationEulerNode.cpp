#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterRotationEulerNode::AnimGraphGetNamedParameterRotationEulerNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphGetNamedParameterRotationEulerNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void AnimGraphGetNamedParameterRotationEulerNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterRotationEulerNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterRotationEulerNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphGetNamedParameterRotationEulerNode::GetParameter()
    {
        AZ::Vector3 value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterRotationEuler, m_parameterIndex);
        SetOutputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
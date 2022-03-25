#include <StdAfx.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    template<typename T>
    AnimGraphGetNamedParameterNode<T>::AnimGraphGetNamedParameterNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    template<typename T>
    Core::SSBehaviorTreePortsList AnimGraphGetNamedParameterNode<T>::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<AZStd::string>(NODE_PORT_PARAMETER_NAME, NODE_PORT_PARAMETER_DESCRIPTION),
            BT::OutputPort<T>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    template<typename T>
    void AnimGraphGetNamedParameterNode<T>::Start()
    {
        Core::Optional<AZStd::string> parameter = GetInputValue<AZStd::string>(NODE_PORT_PARAMETER_NAME);
        if (parameter.has_value())
        {
            EBUS_EVENT_ID_RESULT(
                m_parameterIndex, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, FindParameterIndex,
                parameter.value().c_str());

            if (m_parameterIndex == INVALID_PARAMETER_INDEX)
            {
                AZ_Warning(
                    "SSBehaviorTree", false, "[%s:%s] The named parameter \"%s\" was not found in the animation graph.",
                    RegisteredNodeName(), NodeName(), parameter.value().c_str());
            }
        }
    }

    template<typename T>
    Core::SSBehaviorTreeNodeStatus AnimGraphGetNamedParameterNode<T>::Tick()
    {
        if (m_parameterIndex == INVALID_PARAMETER_INDEX)
        {
            return Core::SSBehaviorTreeNodeStatus::FAILURE;
        }
        else
        {
            GetParameter();
            return Core::SSBehaviorTreeNodeStatus::SUCCESS;
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

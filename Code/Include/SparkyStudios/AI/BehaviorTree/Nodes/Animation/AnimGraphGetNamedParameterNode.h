#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

#include <Integration/AnimGraphComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    /**
     * @brief Base class for all AnimGraphGetNamedParameter nodes.
     *
     * @tparam T The type of the value the child class should set.
     */
    template<typename T>
    class AnimGraphGetNamedParameterNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterNode, AZ::SystemAllocator, 0);
        AZ_RTTI(((AnimGraphGetNamedParameterNode<T>), "{493e6e39-d275-4209-924f-7a62ca0aaf7e}", T), Core::SSBehaviorTreeNode);

        AnimGraphGetNamedParameterNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
            : Core::SSBehaviorTreeNode(name, config)
        {
        }

        static const AZ::u32 INVALID_PARAMETER_INDEX = 0xFFFFFFFF;

        static constexpr const char* NODE_PORT_PARAMETER_NAME = "parameter";
        static constexpr const char* NODE_PORT_PARAMETER_DESCRIPTION = "The name of the parameter";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blackboard entry in which the parameter value should be set.";

        static Core::SSBehaviorTreePortsList providedPorts()
        {
            Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

            ports.merge(Core::SSBehaviorTreePortsList({
                BT::InputPort<AZStd::string>(NODE_PORT_PARAMETER_NAME, NODE_PORT_PARAMETER_DESCRIPTION),
                BT::OutputPort<T>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
            }));

            return ports;
        }

    protected:
        void Start() override
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

        Core::SSBehaviorTreeNodeStatus Tick() override
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

        /**
         * @brief Gets the value of the parameter. This method need to be implemented in
         * child classes.
         */
        virtual void GetParameter() = 0;

        /**
         * @brief The index of the paramter in the animation graph.
         */
        AZ::u32 m_parameterIndex = INVALID_PARAMETER_INDEX;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

#include <Integration/AnimGraphComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    /**
     * @brief Base class for all AnimGraphSetNamedParameter nodes.
     *
     * @tparam T The type of the value the child class should set.
     */
    template<typename T>
    class AnimGraphSetNamedParameterNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterNode, AZ::SystemAllocator, 0);
        AZ_RTTI(((AnimGraphSetNamedParameterNode<T>), "{8d202ec2-61ef-48ee-a083-a9ffc4362a97}", T), Core::SSBehaviorTreeNode);

        AnimGraphSetNamedParameterNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
            : Core::SSBehaviorTreeNode(name, config)
        {
        }

        static constexpr AZ::u64 INVALID_PARAMETER_INDEX = 0xFFFFFFFFFFFFFFFF;

        static constexpr const char* NODE_PORT_PARAMETER_NAME = "parameter";
        static constexpr const char* NODE_PORT_PARAMETER_DESCRIPTION = "The name of the parameter";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The value to set to the parameter.";

        static Core::SSBehaviorTreePortsList providedPorts()
        {
            Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

            ports.merge(Core::SSBehaviorTreePortsList({
                BT::InputPort<AZStd::string>(NODE_PORT_PARAMETER_NAME, NODE_PORT_PARAMETER_DESCRIPTION),
                BT::InputPort<T>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
            }));

            return ports;
        }

        const std::string NodeCategory() const override
        {
            return "Animation";
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
                SetParameter();
                return Core::SSBehaviorTreeNodeStatus::SUCCESS;
            }
        }

        /**
         * @brief Sets the value of the parameter. This method need to be implemented in
         * child classes.
         */
        virtual void SetParameter() = 0;

        /**
         * @brief The index of the paramter in the animation graph.
         */
        AZ::u64 m_parameterIndex = INVALID_PARAMETER_INDEX;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

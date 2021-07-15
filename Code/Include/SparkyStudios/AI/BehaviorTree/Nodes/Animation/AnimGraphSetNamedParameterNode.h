#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>

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
        AZ_RTTI(AnimGraphSetNamedParameterNode, "{8d202ec2-61ef-48ee-a083-a9ffc4362a97}", Core::SSBehaviorTreeNode);

        AnimGraphSetNamedParameterNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static const AZ::u32 INVALID_PARAMETER_INDEX = 0xFFFFFFFF;

        static constexpr const char* NODE_PORT_PARAMETER_NAME = "parameter";
        static constexpr const char* NODE_PORT_PARAMETER_DESCRIPTION = "The name of the parameter";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The value to set to the parameter.";

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void Start() override;

        Core::SSBehaviorTreeNodeStatus Tick() override;

        /**
         * @brief Sets the value of the parameter. This method need to be implemented in
         * child classes.
         */
        virtual void SetParameter() = 0;

        /**
         * @brief The index of the paramter in the animation graph.
         */
        AZ::u32 m_parameterIndex = INVALID_PARAMETER_INDEX;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

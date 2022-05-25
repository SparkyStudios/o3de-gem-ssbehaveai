#pragma once

#include <StdAfx.h>

#include <AzCore/Component/Entity.h>
#include <AzCore/std/string/string.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    using BehaviorTreeNodeConfiguration = BT::NodeConfiguration;
    using BehaviorTreeNodeStatus = BT::NodeStatus;
    using BehaviorTreePortsList = BT::PortsList;

    template<typename T>
    using Optional = BT::Optional<T>;
    using Result = BT::Result;

    /**
     * @brief The base class for all behavior tree nodes.
     */
    class Node : public BT::StatefulActionNode
    {
        friend class Factory;

    public:
        AZ_CLASS_ALLOCATOR(Node, AZ::SystemAllocator, 0);
        AZ_RTTI(Node, "{BDC7EF90-5955-4EE7-9118-46F0D069194F}");

        Node(const std::string& name, const BehaviorTreeNodeConfiguration& config);

        ~Node() override = default;

        /**
         * @brief The name of the node in the behavior tree file.
         */
        static constexpr const char* NODE_NAME = "Node";

        /**
         * @brief Returns the list of ports provided by this node.
         * This method must be implemented in the derived class.
         *
         * @return SSBehaviorTreePortsList
         */
        static BehaviorTreePortsList providedPorts();

        /**
         * @brief Gets the category in which this node will be represented in the editor.
         *
         * @return const std::string A string value representing the category of this node.
         */
        virtual std::string NodeCategory() const;

    protected:
        /**
         * @brief Run before the first tick, to initialize the node.
         */
        virtual void Start();

        /**
         * @brief Run on each node tick.
         */
        virtual BehaviorTreeNodeStatus Tick();

        /**
         * @brief Run just before to leave the node after the last tick, to deinitialize the node.
         */
        virtual void Finish();

        /**
         * @brief Returns the current value of an input port with the given id.
         *
         * @tparam T The type of the value to return.
         * @param id The Input port id.
         *
         * @return Optional<T>
         */
        template<typename T>
        Optional<T> GetInputValue(const AZStd::string& id) const
        {
            Optional<T> value = getInput<T>(id.c_str());

            if (!value)
            {
                AZ_Error(
                    "BehaveAI [BehaviorTree]", false, "[%s:%s] Missing required input {%s}: %s", registrationName().c_str(), name().c_str(),
                    id.c_str(), value.error().c_str());
            }

            return value;
        }

        /**
         * @brief Set the value of an output port with the given id.
         *
         * @tparam T The type of the value.
         * @param  id The name of the output port for which define the value.
         * @param value The value to define into the output port.
         *
         * @return Result
         */
        template<typename T>
        Result SetOutputValue(const AZStd::string& id, const T& value)
        {
            return setOutput<T>(id.c_str(), value);
        }

        /**
         * @brief Gets the ID of the entity to which this node's behavior tree
         * is attached to.
         *
         * @return AZ::EntityId
         */
        AZ::EntityId GetEntityId() const;

        /**
         * @brief Gets the blackboard instance of this node's behavior tree.
         *
         * @return BT::Blackboard::Ptr
         */
        const BT::Blackboard::Ptr& GetBlackboard() const;

        /**
         * @brief Gets the name used for the registration of this node in
         * the behavior tree nodes factory.
         *
         * @return const char*
         */
        const char* RegisteredNodeName() const;

        /**
         * @brief Gets the instance name of the node, as defined in the behavior tree asset.
         *
         * @return const char*
         */
        const char* NodeName() const;

    private:
#pragma region BT::StatefulActionNode

        BehaviorTreeNodeStatus onStart() final;

        BehaviorTreeNodeStatus onRunning() final;

        void onHalted() final;

#pragma endregion

        bool _started;
    };

    /**
     * @brief Special node that returns success or failure based on the condition.
     */
    class BehaviorTreeConditionNode : public BT::ConditionNode
    {
    public:
        AZ_CLASS_ALLOCATOR(BehaviorTreeConditionNode, AZ::SystemAllocator, 0);
        AZ_RTTI(BehaviorTreeConditionNode, "{B951E9D3-6908-4693-8FBF-9B876F74FC89}");

        BehaviorTreeConditionNode(const std::string& name, const BehaviorTreeNodeConfiguration& config);

    protected:
        /**
         * @brief The condition to execute.
         *
         * @return bool
         */
        virtual bool Condition() = 0;

    private:
#pragma region BT::ConditionNode

        BehaviorTreeNodeStatus tick() final;

#pragma endregion
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

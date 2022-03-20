#pragma once

#include <behaviortree_cpp_v3/bt_factory.h>

#include <AzCore/RTTI/RTTI.h>

#include <SparkyStudios/AI/Behave/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    /**
     * @brief Create blackboard properties and nodes for a behavior tree.
     *
     * The first of possible properties and nodes to create are given defined by the
     * registry instance given in the constructor.
     */
    class SSBehaviorTreeFactory
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeFactory, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeFactory, "{d5bc9f90-b601-4fb8-9d06-30dbebd1fc43}");

        /**
         * @brief Construct a new SSBehaviorTreeFactory.
         *
         * @param registry The registry from which get the possible properties and nodes to create.
         * If no value or nullptr are given, then a new empty registry is created.
         */
        SSBehaviorTreeFactory(SSBehaviorTreeRegistry* registry = nullptr);

        /**
         * @brief Destroy the SSBehaviorTreeFactory.
         * This will also destroy the registry.
         */
        virtual ~SSBehaviorTreeFactory();

        /**
         * @brief Get the wrapped registry instance.
         *
         * @return const AZStd::shared_ptr<SSBehaviorTreeRegistry>&
         */
        const AZStd::shared_ptr<SSBehaviorTreeRegistry>& GetRegistry() const;

        /**
         * @brief Creates a new blackboard property from the given type.
         *
         * @param type The property value type, as specified when registering with RegisterProperty.
         * @param name The instance name of the property.
         * @return AZStd::unique_ptr<Blackboard::SSBehaviorTreeBlackboardProperty>
         */
        AZStd::unique_ptr<Blackboard::SSBehaviorTreeBlackboardProperty> CreateProperty(const AZStd::string& type, const char* name) const;

        /**
         * @brief Create a new behavior tree node from the given name. This method will only work for a node
         * that was registered with RegisterNode.
         *
         * @param name The node name.
         * @param instanceName The instance name of the node.
         * @param config The configuration for the node.
         * @return AZStd::unique_ptr<SSBehaviorTreeNode>
         */
        AZStd::unique_ptr<SSBehaviorTreeNode> CreateNode(
            const AZStd::string& name, const AZStd::string& instanceName, const SSBehaviorTreeNodeConfiguration& config = {}) const;

        /**
         * @brief Create a behavior tree from the given XML text.
         *
         * @param text The XML text to parse into a behavior tree.
         * @param blackboard The blackboard instance which this behavior tree will use.
         * @return BT::Tree
         */
        BT::Tree CreateTreeFromText(const AZStd::string& text, const Blackboard::SSBehaviorTreeBlackboard& blackboard = {}) const;

    private:
        AZStd::shared_ptr<SSBehaviorTreeRegistry> m_registry;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

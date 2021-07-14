#pragma once

#include <behaviortree_cpp_v3/bt_factory.h>

#include <AzCore/RTTI/RTTI.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    using Blackboard::SSBehaviorTreeBlackboardProperty;

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
        ~SSBehaviorTreeFactory();

        /**
         * @brief Get the wrapped registry instance.
         *
         * @return AZStd::unique_ptr<SSBehaviorTreeRegistry>
         */
        const AZStd::unique_ptr<SSBehaviorTreeRegistry>& GetRegistry() const;

        /**
         * @brief Creates a new blackboard property from the given type.
         *
         * @param type The property value type, as specified when registering with RegisterProperty.
         * @param name The instance name of the property.
         * @return SSBehaviorTreeBlackboardProperty*
         */
        AZStd::unique_ptr<SSBehaviorTreeBlackboardProperty> CreateProperty(const AZStd::string& type, const char* name) const;

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
            const AZStd::string& name,
            const AZStd::string& instanceName,
            const SSBehaviorTreeNodeConfiguration& config = SSBehaviorTreeNodeConfiguration()) const;

    private:
        AZStd::unique_ptr<SSBehaviorTreeRegistry> m_registry;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

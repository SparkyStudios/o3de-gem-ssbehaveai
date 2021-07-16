#pragma once

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    class SSBehaviorTreeFactory;

    /**
     * @brief The function used to build properties.
     */
    using SSBehaviorTreeBlackboardPropertyBuilder =
        AZStd::function<AZStd::unique_ptr<Blackboard::SSBehaviorTreeBlackboardProperty>(const char*)>;

    /**
     * @brief The function used to build behavior tree nodes.
     */
    using SSBehaviorTreeNodeBuilder =
        AZStd::function<AZStd::unique_ptr<SSBehaviorTreeNode>(const std::string&, const SSBehaviorTreeNodeConfiguration&)>;

    /**
     * @brief Register the blackboard properties and nodes used in behavior tree files.
     */
    class SSBehaviorTreeRegistry
    {
        friend class SSBehaviorTreeFactory;

    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeRegistry, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeRegistry, "{1a24e981-17fc-4d02-ac71-ff0e575b09ba}");

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         *
         * @param type The property value type.
         * @param uuid The property type UUID.
         * @param builder The property builder.
         */
        void RegisterProperty(const AZStd::string& type, const AZ::Uuid& uuid, const SSBehaviorTreeBlackboardPropertyBuilder& builder);

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         * The type parameter should have the AZ_RTTI macro for this override to work.
         *
         * @tparam T The type of the property value. Must have the AZ_RTTI macro.
         * @param type The property value type.
         * @param builder The property builder.
         */
        template<typename T>
        void RegisterProperty(const AZStd::string& type, const SSBehaviorTreeBlackboardPropertyBuilder& builder);

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         * The type parameter should have the AZ_RTTI macro for this override to work.
         * This override will use the default builder.
         *
         * @tparam T The type of the property value. Must have the AZ_RTTI macro.
         * @param type The property value type.
         */
        template<typename T>
        void RegisterProperty(const AZStd::string& type);

        /**
         * @brief Get the property builder for the given type.
         *
         * @param type The type to get the builder for.
         * @return const SSBehaviorTreeBlackboardPropertyBuilder&
         * @throws std::runtime_error if the type is not registered.
         */
        const SSBehaviorTreeBlackboardPropertyBuilder& GetPropertyBuilder(const AZStd::string& type) const;

        /**
         * @brief Get the Uuid for the given type.
         *
         * @param type The type to get the UUID for.
         * @return const AZ::Uuid&
         */
        const AZ::Uuid& GetPropertyUuid(const AZStd::string& type) const;

        /**
         * @brief Add a node for the registration process.
         *
         * @tparam T The type of the node.
         * @param name The name of the node in the behavior tree.
         */
        template<typename T>
        void DelayNodeRegistration(const AZStd::string& name);

        /**
         * @brief Perform the node registration process.
         *
         * @param nodes The list of nodes to register. These nodes must have
         * been added for registration using DelayNodeRegistration().
         */
        void EnableNodes(const AZStd::vector<AZStd::string>& nodes);

    private:
        /**
         * @brief Register a new node in this registry. The node should be saved for a delayed registration
         * before this call.
         *
         * @param name The name of the node in the behavior tree file.
         */
        void RegisterNode(const AZStd::string& name);

        AZStd::unordered_map<AZStd::string, SSBehaviorTreeBlackboardPropertyBuilder> m_registeredTypeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> m_registeredTypeUuids;

        AZStd::unordered_map<AZStd::string, SSBehaviorTreeNodeBuilder> m_registeredNodeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> m_registeredNodeUuids;

        AZStd::unordered_map<AZStd::string, AZStd::pair<BT::TreeNodeManifest, SSBehaviorTreeNodeBuilder>> m_delayedRegisterers;

        AZStd::unique_ptr<BT::BehaviorTreeFactory> m_factory;

        AZ::Uuid m_invalidUuid = AZ::Uuid::CreateNull();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

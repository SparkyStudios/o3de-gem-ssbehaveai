#pragma once

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSbehaviorTreeFactory.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/SSBehaviorTreeNode.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    /**
     * @brief The function used to build properties.
     */
    using SSBehaviorTreeBlackboardPropertyBuilder = AZStd::function<Blackboard::SSBehaviorTreeBlackboardProperty*(const char*)>;

    /**
     * @brief The function used to build behavior tree nodes.
     */
    using SSBehaviorTreeNodeBuilder =
        AZStd::function<AZStd::unique_ptr<Nodes::SSBehaviorTreeNode>(const AZStd::string&, const Nodes::SSBehaviorTreeNodeConfiguration&)>;

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
         * @brief Reflect this class in the given O3DE ReflectContext.
         * Used for serialization purposes.
         *
         * @param context The ReflectContext in which describe this class.
         */
        static void Reflect(AZ::ReflectContext* reflection);

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         *
         * @param type The property value type.
         * @param uuid The property type UUID.
         * @param builder The property builder.
         */
        void RegisterProperty(const AZStd::string& type, const AZ::Uuid& uuid, const SSBehaviorTreeBlackboardPropertyBuilder& builder);

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
         * @brief Register a new node in this registry.
         *
         * @tparam T The node's type.
         * @param name The name of the node in the behavior tree file.
         */
        template<typename T>
        void RegisterNode(const AZStd::string& name);

    private:
        AZStd::unordered_map<AZStd::string, SSBehaviorTreeBlackboardPropertyBuilder> m_registeredTypeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> m_registeredTypeUuids;

        AZStd::unordered_map<AZStd::string, SSBehaviorTreeNodeBuilder> m_registeredNodeBuilders;

        AZStd::unique_ptr<BT::BehaviorTreeFactory> m_factory;

        AZ::Uuid m_invalidUuid = AZ::Uuid::CreateNull();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

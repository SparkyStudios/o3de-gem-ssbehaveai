#pragma once

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    using SSBehaviorTreeBlackboardPropertyBuilder = AZStd::function<SSBehaviorTreeBlackboardProperty*(const char*)>;

    /**
     * @brief Stores the types used in behavior tree files, and associate them with SSBehaviorTreeBlackboardProperty classes.
     */
    class SSBehaviorTreeBlackboardPropertyRegistry
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyRegistry, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyRegistry, "{1a24e981-17fc-4d02-ac71-ff0e575b09ba}");

        static void Reflect(AZ::ReflectContext* reflection);

        /**
         * @brief Creates a new blackboard property from the given type.
         *
         * @param type The property value type, as specified when registering with RegisterTypeBuilder.
         * @param name The instance name of the property.
         * @return SSBehaviorTreeBlackboardProperty*
         */
        SSBehaviorTreeBlackboardProperty* Create(const AZStd::string& type, const char* name) const
        {
            auto it = m_registeredTypeBuilders.find(type);
            if (it != m_registeredTypeBuilders.end())
                return it->second(name);

            return nullptr;
        }

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         *
         * @param type The property value type.
         * @param builder The property builder.
         */
        void RegisterTypeBuilder(const AZStd::string& type, const SSBehaviorTreeBlackboardPropertyBuilder& builder)
        {
            m_registeredTypeBuilders.insert(AZStd::make_pair(type, builder));
        }

        /**
         * @brief Registers the UUID of the type. This must be called after RegisterTypeBuilder.
         *
         * @param type The property value type.
         * @param uuid The type's UUID.
         */
        void RegisterTypeUuid(const AZStd::string& type, const AZ::Uuid& uuid)
        {
            if (m_registeredTypeBuilders.find(type) != m_registeredTypeBuilders.end())
                m_registeredTypeUuids.insert(AZStd::make_pair(type, uuid));

            throw std::runtime_error("Error: Tries to register a type UUID in the registry before the type builder.");
        }

        /**
         * @brief Get the Uuid for the given type.
         *
         * @param type The type to get the UUID for.
         * @return const AZ::Uuid&
         */
        const AZ::Uuid& GetUuidForType(const AZStd::string& type) const
        {
            auto it = m_registeredTypeUuids.find(type);
            if (it != m_registeredTypeUuids.end())
                return it->second;

            return m_invalidUuid;
        }

    private:
        AZStd::unordered_map<AZStd::string, SSBehaviorTreeBlackboardPropertyBuilder> m_registeredTypeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> m_registeredTypeUuids;

        AZ::Uuid m_invalidUuid = AZ::Uuid::CreateNull();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

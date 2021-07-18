#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    void SSBehaviorTreeRegistry::RegisterProperty(
        const AZStd::string& type, const AZ::Uuid& uuid, const SSBehaviorTreeBlackboardPropertyBuilder& builder)
    {
        m_registeredTypeBuilders.insert(AZStd::make_pair(type, builder));
        m_registeredTypeUuids.insert(AZStd::make_pair(type, uuid));
    }

    const SSBehaviorTreeBlackboardPropertyBuilder& SSBehaviorTreeRegistry::GetPropertyBuilder(const AZStd::string& type) const
    {
        if (auto findIt = m_registeredTypeBuilders.find(type); findIt != m_registeredTypeBuilders.end())
            return findIt->second;

        AZ_Error("SSBehaviorTree", false, "Error: Cannot find a property builder for the given type. Did you forget to register?");
        return m_invalidBuilder;
    }

    const AZ::Uuid& SSBehaviorTreeRegistry::GetPropertyUuid(const AZStd::string& type) const
    {
        if (auto findIt = m_registeredTypeUuids.find(type); findIt != m_registeredTypeUuids.end())
            return findIt->second;

        return m_invalidUuid;
    }

    void SSBehaviorTreeRegistry::EnableNodes(const AZStd::vector<AZStd::string>& nodes)
    {
        AZ::SerializeContext* serializeContext = nullptr;
        EBUS_EVENT_RESULT(serializeContext, AZ::ComponentApplicationBus, GetSerializeContext);

        if (!serializeContext)
        {
            AZ_Error("SSBehaviorTreeEditor", false, "Can't get serialize context from component application.");
            return;
        }

        for (const auto& node : nodes)
        {
            AZ_Printf("SSBehaviorTree", "Enabling node: %s\n", node.c_str());
            RegisterNode(node, serializeContext);
        }

        // TODO: Should I need to clear the delayed nodes?
    }

    void SSBehaviorTreeRegistry::RegisterNode(const AZStd::string& name, AZ::ReflectContext* context)
    {
        if (auto findIt = m_delayedRegisterers.find(name); findIt != m_delayedRegisterers.end())
        {
            if (context)
            {
                // If the node is found in the delayed registration list, its reflector is also registered.
                m_delayedReflectors[name](context);
            }

            // Register the node in the BT factory.
            m_factory->registerBuilder(findIt->second.first, findIt->second.second);

            // Mark the node as registered.
            m_registeredNodeBuilders.insert(AZStd::make_pair(name, findIt->second.second));
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

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

    template<typename T>
    void SSBehaviorTreeRegistry::RegisterProperty(const AZStd::string& type, const SSBehaviorTreeBlackboardPropertyBuilder& builder)
    {
        static_assert(
            AZStd::is_base_of<SSBehaviorTreeBlackboardProperty, T>::value, "T must be derived from SSBehaviorTreeBlackboardProperty");
        static_assert(!AZStd::is_abstract<T>::value, "T must not be abstract");

        RegisterProperty(type, azrtti_typeid<T>(), builder);
    }

    template<typename T>
    void SSBehaviorTreeRegistry::RegisterProperty(const AZStd::string& type)
    {
        static_assert(
            AZStd::is_base_of<SSBehaviorTreeBlackboardProperty, T>::value, "T must be derived from SSBehaviorTreeBlackboardProperty");
        static_assert(!AZStd::is_abstract<T>::value, "T must not be abstract");

        SSBehaviorTreeBlackboardPropertyBuilder builder = [](const char* name)
        {
            return AZStd::make_unique<T>(name);
        };

        RegisterProperty(type, azrtti_typeid<T>(), builder);
    }

    const SSBehaviorTreeBlackboardPropertyBuilder& SSBehaviorTreeRegistry::GetPropertyBuilder(const AZStd::string& type) const
    {
        auto it = m_registeredTypeBuilders.find(type);
        if (it != m_registeredTypeBuilders.end())
            return it->second;

        throw std::runtime_error("Error: Cannot find a property builder for the given type. Did you forget to register?");
    }

    const AZ::Uuid& SSBehaviorTreeRegistry::GetPropertyUuid(const AZStd::string& type) const
    {
        auto it = m_registeredTypeUuids.find(type);
        if (it != m_registeredTypeUuids.end())
            return it->second;

        return m_invalidUuid;
    }

    template<typename T>
    void SSBehaviorTreeRegistry::DelayNodeRegistration(const AZStd::string& name)
    {
        static_assert(AZStd::is_base_of<SSBehaviorTreeNode, T>::value, "T must be derived from SSBehaviorTreeNode");
        static_assert(!AZStd::is_abstract<T>::value, "T must not be abstract");

        SSBehaviorTreeNodeBuilder builder = [](const std::string& name, const SSBehaviorTreeNodeConfiguration& config)
        {
            return AZStd::make_unique<T>(name, config);
        };

        BT::TreeNodeManifest manifest{ BT::getType<T>(), name.c_str(), BT::getProvidedPorts<T>() };

        m_delayedRegisterers.insert(AZStd::make_pair(name, AZStd::make_pair(manifest, builder)));
        m_registeredNodeUuids.insert(AZStd::make_pair(name, azrtti_typeid<T>()));
    }

    void SSBehaviorTreeRegistry::EnableNodes(const AZStd::vector<AZStd::string>& nodes)
    {
        for (const auto& node : nodes)
        {
            RegisterNode(node);
        }

        // TODO: Should I need to clear the delayed nodes?
    }

    void SSBehaviorTreeRegistry::RegisterNode(const AZStd::string& name)
    {
        if (auto findIt = m_delayedRegisterers.find(name); findIt != m_delayedRegisterers.end())
        {
            // Register the node in the BT factory.
            m_factory->registerBuilder(findIt->second.first, findIt->second.second);

            // Mark the node as registered.
            m_registeredNodeBuilders.insert(AZStd::make_pair(name, findIt->second.second));
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

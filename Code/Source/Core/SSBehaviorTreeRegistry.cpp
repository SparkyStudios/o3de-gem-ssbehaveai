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
    void SSBehaviorTreeRegistry::RegisterNode(const AZStd::string& name)
    {
        static_assert(AZStd::is_base_of<SSBehaviorTreeNode, T>::value, "T must be derived from SSBehaviorTreeNode");
        static_assert(!AZStd::is_abstract<T>::value, "T must not be abstract");

        SSBehaviorTreeNodeBuilder builder = [](const AZStd::string& name, const SSBehaviorTreeNodeConfiguration& config)
        {
            return AZStd::unique_ptr<SSBehaviorTreeNode>(aznew T(name, config));
        };

        // Register the node in the BT factory.
        m_factory->registerBuilder<T>(name, builder);

        m_registeredNodeBuilders.insert(AZStd::make_pair(name, builder));
    };
} // namespace SparkyStudios::AI::BehaviorTree::Core

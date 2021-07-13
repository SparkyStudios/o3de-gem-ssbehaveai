#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeFactory.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    SSBehaviorTreeFactory::SSBehaviorTreeFactory(SSBehaviorTreeRegistry* registry)
    {
        if (registry)
            m_registry = AZStd::move(registry);
        else
            m_registry = aznew SSBehaviorTreeRegistry();
    }

    SSBehaviorTreeFactory::~SSBehaviorTreeFactory()
    {
        delete m_registry;
    }

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeFactory::CreateProperty(const AZStd::string& type, const char* name) const
    {
        auto it = m_registry->m_registeredTypeBuilders.find(type);
        if (it != m_registry->m_registeredTypeBuilders.end())
            return it->second(name);

        return nullptr;
    }

    AZStd::unique_ptr<SSBehaviorTreeNode> SSBehaviorTreeFactory::CreateNode(
        const AZStd::string& name, const AZStd::string& instanceName, const Nodes::SSBehaviorTreeNodeConfiguration& config) const
    {
        auto it = m_registry->m_registeredNodeBuilders.find(name);
        if (it != m_registry->m_registeredNodeBuilders.end())
            return it->second(instanceName, config);

        throw std::runtime_error("Unable to create the node. Did you forget to register?");
    }
} // namespace SparkyStudios::AI::BehaviorTree::Core

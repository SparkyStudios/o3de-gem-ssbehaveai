#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeFactory.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    SSBehaviorTreeFactory::SSBehaviorTreeFactory(SSBehaviorTreeRegistry* registry)
    {
        if (registry)
            m_registry = AZStd::unique_ptr<SSBehaviorTreeRegistry>(registry);
        else
            m_registry = AZStd::make_unique<SSBehaviorTreeRegistry>();
    }

    SSBehaviorTreeFactory::~SSBehaviorTreeFactory()
    {
        m_registry.reset();
    }

    const AZStd::unique_ptr<SSBehaviorTreeRegistry>& SSBehaviorTreeFactory::GetRegistry() const
    {
        return m_registry;
    }

    AZStd::unique_ptr<SSBehaviorTreeBlackboardProperty> SSBehaviorTreeFactory::CreateProperty(
        const AZStd::string& type, const char* name) const
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
        {
            auto node = it->second(instanceName, config);
            // node->setRegistrationID(name.c_str());
        }

        throw std::runtime_error("Unable to create the node. Did you forget to register?");
    }
} // namespace SparkyStudios::AI::BehaviorTree::Core

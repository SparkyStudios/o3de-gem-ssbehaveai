
#include <SSBehaviorTreeSystemComponent.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::BehaviorTree
{
    void SSBehaviorTreeSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<SSBehaviorTreeSystemComponent, AZ::Component>()->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<SSBehaviorTreeSystemComponent>(
                      "SSBehaviorTree", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void SSBehaviorTreeSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("SSBehaviorTreeService"));
    }

    void SSBehaviorTreeSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("SSBehaviorTreeService"));
    }

    void SSBehaviorTreeSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void SSBehaviorTreeSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    SSBehaviorTreeSystemComponent::SSBehaviorTreeSystemComponent()
    {
        if (SSBehaviorTreeInterface::Get() == nullptr)
        {
            SSBehaviorTreeInterface::Register(this);
        }
    }

    SSBehaviorTreeSystemComponent::~SSBehaviorTreeSystemComponent()
    {
        if (SSBehaviorTreeInterface::Get() == this)
        {
            SSBehaviorTreeInterface::Unregister(this);
        }
    }

    void SSBehaviorTreeSystemComponent::Init()
    {
    }

    void SSBehaviorTreeSystemComponent::Activate()
    {
        SSBehaviorTreeRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void SSBehaviorTreeSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        SSBehaviorTreeRequestBus::Handler::BusDisconnect();
    }

    void SSBehaviorTreeSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

    const Core::SSBehaviorTreeFactory& SSBehaviorTreeSystemComponent::GetFactory() const
    {
        return m_factory;
    }
} // namespace SparkyStudios::AI::BehaviorTree

#include <StdAfx.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <SSBehaviorTreeEditorSystemComponent.h>

namespace SparkyStudios::AI::BehaviorTree
{
    void SSBehaviorTreeEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<SSBehaviorTreeEditorSystemComponent, SSBehaviorTreeSystemComponent>()->Version(0);
        }
    }

    SSBehaviorTreeEditorSystemComponent::SSBehaviorTreeEditorSystemComponent() = default;

    SSBehaviorTreeEditorSystemComponent::~SSBehaviorTreeEditorSystemComponent() = default;

    void SSBehaviorTreeEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("SSBehaviorTreeEditorService"));
    }

    void SSBehaviorTreeEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("SSBehaviorTreeEditorService"));
    }

    void SSBehaviorTreeEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void SSBehaviorTreeEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void SSBehaviorTreeEditorSystemComponent::Activate()
    {
        SSBehaviorTreeSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void SSBehaviorTreeEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        SSBehaviorTreeSystemComponent::Deactivate();
    }

} // namespace SparkyStudios::AI::BehaviorTree


#pragma once

#include <SSBehaviorTreeSystemComponent.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace SparkyStudios::AI::BehaviorTree
{
    /// System component for SSBehaviorTree editor
    class SSBehaviorTreeEditorSystemComponent
        : public SSBehaviorTreeSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = SSBehaviorTreeSystemComponent;

    public:
        AZ_COMPONENT(SSBehaviorTreeEditorSystemComponent, "{c6d81d4a-51ca-4c74-9d0d-96f813233c74}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeEditorSystemComponent();
        ~SSBehaviorTreeEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree

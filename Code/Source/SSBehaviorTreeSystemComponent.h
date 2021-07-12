
#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <SSBehaviorTree/SSBehaviorTreeBus.h>

namespace SSBehaviorTree
{
    class SSBehaviorTreeSystemComponent
        : public AZ::Component
        , protected SSBehaviorTreeRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT(SSBehaviorTreeSystemComponent, "{4a9e985c-692e-47c3-9573-94dd1aa64dce}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        SSBehaviorTreeSystemComponent();
        ~SSBehaviorTreeSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // SSBehaviorTreeRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZTickBus interface implementation
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace SSBehaviorTree

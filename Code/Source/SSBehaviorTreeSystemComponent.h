#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

#include <SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeSystemComponent
        : public AZ::Component
        , protected SSBehaviorTreeRequestBus::Handler
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
        const Core::SSBehaviorTreeFactory& GetFactory() const override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

    private:
        void RegisterDefaultProperties();
        void RegisterDefaultNodes();

        Core::SSBehaviorTreeFactory m_factory;
    };

} // namespace SparkyStudios::AI::BehaviorTree

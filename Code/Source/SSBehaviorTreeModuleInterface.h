
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <SSBehaviorTreeSystemComponent.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeModuleInterface : public AZ::Module
    {
    public:
        AZ_RTTI(SSBehaviorTreeModuleInterface, "{0c335fb2-93a5-4f99-a8cd-8ff1c7763dc1}", AZ::Module);
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeModuleInterface, AZ::SystemAllocator, 0);

        SSBehaviorTreeModuleInterface()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SSBehaviorTreeSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SSBehaviorTreeSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree

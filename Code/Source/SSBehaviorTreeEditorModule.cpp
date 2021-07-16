#include <StdAfx.h>

#include <SSBehaviorTreeEditorSystemComponent.h>
#include <SSBehaviorTreeModuleInterface.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeEditorModule : public SSBehaviorTreeModuleInterface
    {
    public:
        AZ_RTTI(SSBehaviorTreeEditorModule, "{2fc25376-c0a0-4b56-bfbe-5a65bad02586}", SSBehaviorTreeModuleInterface);
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeEditorModule, AZ::SystemAllocator, 0);

        SSBehaviorTreeEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SSBehaviorTreeEditorSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SSBehaviorTreeEditorSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree

AZ_DECLARE_MODULE_CLASS(Gem_SSBehaviorTree, SparkyStudios::AI::BehaviorTree::SSBehaviorTreeEditorModule)

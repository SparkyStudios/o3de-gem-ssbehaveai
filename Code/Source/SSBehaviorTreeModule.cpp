#include <StdAfx.h>

#include <SSBehaviorTreeComponent.h>
#include <SSBehaviorTreeModuleInterface.h>
#include <SSBehaviorTreeSystemComponent.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeModule : public SSBehaviorTreeModuleInterface
    {
    public:
        AZ_RTTI(SSBehaviorTreeModule, "{2fc25376-c0a0-4b56-bfbe-5a65bad02586}", SSBehaviorTreeModuleInterface);
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeModule, AZ::SystemAllocator, 0);

        SSBehaviorTreeModule()
        {
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SSBehaviorTreeSystemComponent::CreateDescriptor(),
                    SSBehaviorTreeComponent::CreateDescriptor(),
                });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SSBehaviorTreeSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree

AZ_DECLARE_MODULE_CLASS(Gem_SSBehaviorTree, SparkyStudios::AI::BehaviorTree::SSBehaviorTreeModule)

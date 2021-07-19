#include <StdAfx.h>

#include <SSBehaviorTreeEditorSystemComponent.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::BehaviorTree
{
    SSBehaviorTreeEditorSystemComponent::SSBehaviorTreeEditorSystemComponent()
    {
        m_assetHandler = aznew Assets::SSBehaviorTreeAssetHandler();
    }

    SSBehaviorTreeEditorSystemComponent::~SSBehaviorTreeEditorSystemComponent()
    {
        delete m_assetHandler;
    }

    void SSBehaviorTreeEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<SSBehaviorTreeEditorSystemComponent, SSBehaviorTreeSystemComponent>()->Version(0);
        }
    }

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

    void SSBehaviorTreeEditorSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
        required.push_back(AZ_CRC_CE("AssetCatalogService"));
    }

    void SSBehaviorTreeEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void SSBehaviorTreeEditorSystemComponent::Activate()
    {
        if (AZ::Data::AssetManager::IsReady())
        {
            AZ::Data::AssetManager::Instance().RegisterHandler(m_assetHandler, m_assetHandler->GetAssetType());
        }

        // Register with the Asset Catalog
        AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequests::EnableCatalogForAsset, m_assetHandler->GetAssetType());
        AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequests::AddExtension, m_assetHandler->GetExtension().c_str());

        BaseSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void SSBehaviorTreeEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        BaseSystemComponent::Deactivate();

        if (AZ::Data::AssetManager::IsReady())
        {
            AZ::Data::AssetManager::Instance().UnregisterHandler(m_assetHandler);
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree

// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Navigation/BehaveNavigationEditorSystemComponent.h>
#include <Navigation/Utils/Constants.h>

#include <AzCore/Serialization/SerializeContext.h>

#include <AzFramework/Asset/GenericAssetHandler.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    namespace Assets
    {
        static constexpr char kBehaveAssetCategoryName[] = "Behave AI";

#pragma region BehaveNavigationAgentAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationAgentAsset>* s_navigationAgentAssetHandler = nullptr;

        static constexpr int kNavigationAgentAssetKey = 0;
        static constexpr char kNavigationAgentAssetDisplayName[] = "Navigation Agent";
        static constexpr char kNavigationAgentAssetExtension[] = "bhnavagent";
#pragma endregion

#pragma region BehaveNavigationMeshAreaAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationMeshAreaAsset>* s_navigationMeshAreaAssetHandler = nullptr;

        static constexpr int kNavigationMeshAreaAssetKey = 2;
        static constexpr char kNavigationMeshAreaAssetDisplayName[] = "Navigation Mesh Area";
        static constexpr char kNavigationMeshAreaAssetExtension[] = "bhnavmesharea";
#pragma endregion

#pragma region BehaveNavigationMeshSettingsAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationMeshSettingsAsset>* s_navigationMeshSettingAssetHandler = nullptr;

        static constexpr int kNavigationMeshSettingsAssetKey = 1;
        static constexpr char kNavigationMeshSettingsAssetDisplayName[] = "Navigation Mesh Settings";
        static constexpr char kNavigationMeshSettingsAssetExtension[] = "bhnavmeshconfig";
#pragma endregion

        void RegisterAssetHandlers()
        {
            s_navigationAgentAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationAgentAsset>(
                kNavigationAgentAssetDisplayName, kBehaveAssetCategoryName, kNavigationAgentAssetExtension);

            s_navigationMeshAreaAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationMeshAreaAsset>(
                kNavigationMeshAreaAssetDisplayName, kBehaveAssetCategoryName, kNavigationMeshAreaAssetExtension);

            s_navigationMeshSettingAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationMeshSettingsAsset>(
                kNavigationMeshSettingsAssetDisplayName, kBehaveAssetCategoryName, kNavigationMeshSettingsAssetExtension);

            s_navigationAgentAssetHandler->Register();
            s_navigationMeshAreaAssetHandler->Register();
            s_navigationMeshSettingAssetHandler->Register();
        }

        void UnregisterAssetHandlers()
        {
            if (s_navigationAgentAssetHandler)
            {
                s_navigationAgentAssetHandler->Unregister();
                delete s_navigationAgentAssetHandler;
                s_navigationAgentAssetHandler = nullptr;
            }

            if (s_navigationMeshAreaAssetHandler)
            {
                s_navigationMeshAreaAssetHandler->Unregister();
                delete s_navigationMeshAreaAssetHandler;
                s_navigationMeshAreaAssetHandler = nullptr;
            }

            if (s_navigationMeshSettingAssetHandler)
            {
                s_navigationMeshSettingAssetHandler->Unregister();
                delete s_navigationMeshSettingAssetHandler;
                s_navigationMeshSettingAssetHandler = nullptr;
            }
        }
    } // namespace Assets

    void BehaveNavigationEditorSystemComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<BehaveNavigationEditorSystemComponent, BehaveNavigationSystemComponent>()->Version(0);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<BehaveNavigationEditorSystemComponent>(
                      "BehaveNavigation", "2D navigation meshes and path finding powered by Recast Navigation.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    BehaveNavigationEditorSystemComponent::BehaveNavigationEditorSystemComponent() = default;

    BehaveNavigationEditorSystemComponent::~BehaveNavigationEditorSystemComponent() = default;

    void BehaveNavigationEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("BehaveNavigationEditorService"));
    }

    void BehaveNavigationEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("BehaveNavigationEditorService"));
    }

    void BehaveNavigationEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void BehaveNavigationEditorSystemComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void BehaveNavigationEditorSystemComponent::LoadNavigationAgentAsset(const AZ::Data::AssetId& assetId)
    {
        m_navigationAgentAssets[assetId] = AZ::Data::AssetManager::Instance().GetAsset(
            assetId, azrtti_typeid<BehaveNavigationAgentAsset>(), AZ::Data::AssetLoadBehavior::Default);

        AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);
    }

    void BehaveNavigationEditorSystemComponent::LoadNavigationMeshAreaAsset(const AZ::Data::AssetId& assetId)
    {
        m_navigationMeshAreaAssets[assetId] = AZ::Data::AssetManager::Instance().GetAsset(
            assetId, azrtti_typeid<BehaveNavigationMeshAreaAsset>(), AZ::Data::AssetLoadBehavior::Default);

        AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);
    }

    void BehaveNavigationEditorSystemComponent::LoadNavigationMeshSettingsAsset(const AZ::Data::AssetId& assetId)
    {
        m_navigationMeshSettingsAssets[assetId] = AZ::Data::AssetManager::Instance().GetAsset(
            assetId, azrtti_typeid<BehaveNavigationMeshSettingsAsset>(), AZ::Data::AssetLoadBehavior::Default);

        AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);
    }

    void BehaveNavigationEditorSystemComponent::AddAsset(const AZ::Data::Asset<AZ::Data::AssetData>& asset)
    {
        if (asset.GetType() == azrtti_typeid<BehaveNavigationAgentAsset>())
        {
            m_navigationAgentAssets[asset.GetId()] = asset;
        }
        else if (asset.GetType() == azrtti_typeid<BehaveNavigationMeshAreaAsset>())
        {
            m_navigationMeshAreaAssets[asset.GetId()] = asset;
        }
        else if (asset.GetType() == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
        {
            m_navigationMeshSettingsAssets[asset.GetId()] = asset;
        }

        AzToolsFramework::PropertyEditorGUIMessages::Bus::Broadcast(
            &AzToolsFramework::PropertyEditorGUIMessages::RequestRefresh,
            AzToolsFramework::PropertyModificationRefreshLevel::Refresh_AttributesAndValues);
    }

    void BehaveNavigationEditorSystemComponent::Activate()
    {
        Assets::RegisterAssetHandlers();

        BaseSystemComponent::Activate();

        AzFramework::AssetCatalogEventBus::Handler::BusConnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        BehaveNavigationMeshAreaProviderRequestBus::Handler::BusConnect();
    }

    void BehaveNavigationEditorSystemComponent::Deactivate()
    {
        BehaveNavigationMeshAreaProviderRequestBus::Handler::BusDisconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AzFramework::AssetCatalogEventBus::Handler::BusDisconnect();

        BaseSystemComponent::Deactivate();

        Assets::UnregisterAssetHandlers();

        AZ::Data::AssetBus::MultiHandler::BusDisconnect();
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogLoaded([[maybe_unused]] const char* string)
    {
        // Automatically register all managed assets at Editor startup

        AZStd::unordered_map<AZ::Data::AssetId, int> assetIds;

        // First run through all the assets and gather the asset IDs for all navigation agent assets
        AZ::Data::AssetCatalogRequestBus::Broadcast(
            &AZ::Data::AssetCatalogRequestBus::Events::EnumerateAssets, nullptr,
            [&assetIds](const AZ::Data::AssetId assetId, const AZ::Data::AssetInfo& assetInfo)
            {
                if (const auto assetType = azrtti_typeid<BehaveNavigationAgentAsset>(); assetInfo.m_assetType == assetType)
                {
                    assetIds.insert({ assetId, Assets::kNavigationAgentAssetKey });
                }

                if (const auto assetType = azrtti_typeid<BehaveNavigationMeshAreaAsset>(); assetInfo.m_assetType == assetType)
                {
                    assetIds.insert({ assetId, Assets::kNavigationMeshAreaAssetKey });
                }

                if (const auto assetType = azrtti_typeid<BehaveNavigationMeshSettingsAsset>(); assetInfo.m_assetType == assetType)
                {
                    assetIds.insert({ assetId, Assets::kNavigationMeshSettingsAssetKey });
                }
            },
            nullptr);

        // Next, trigger all the loads. This is done outside EnumerateAssets to ensure that we don't have any deadlocks caused by
        // lock inversion. If this thread locks AssetCatalogRequestBus mutex with EnumerateAssets, then locks m_assetMutex in
        // AssetManager::FindOrCreateAsset, it's possible for those locks to get locked in reverse on a loading thread, causing a deadlock.
        for (auto&& assetId : assetIds)
        {
            if (assetId.second == Assets::kNavigationAgentAssetKey)
                LoadNavigationAgentAsset(assetId.first);
            else if (assetId.second == Assets::kNavigationMeshAreaAssetKey)
                LoadNavigationMeshAreaAsset(assetId.first);
            else if (assetId.second == Assets::kNavigationMeshSettingsAssetKey)
                LoadNavigationMeshSettingsAsset(assetId.first);
        }
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogAssetAdded(const AZ::Data::AssetId& assetId)
    {
        AZ::Data::AssetInfo assetInfo;
        AZ::Data::AssetCatalogRequestBus::BroadcastResult(assetInfo, &AZ::Data::AssetCatalogRequests::GetAssetInfoById, assetId);

        if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationAgentAsset>())
        {
            // A new navigation agent asset was added, so load it.
            LoadNavigationAgentAsset(assetId);
        }
        else if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshAreaAsset>())
        {
            // A new navigation mesh area asset was added, so load it.
            LoadNavigationMeshAreaAsset(assetId);
        }
        else if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
        {
            // A new navigation mesh settings asset was added, so load it.
            LoadNavigationMeshSettingsAsset(assetId);
        }
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogAssetRemoved(
        const AZ::Data::AssetId& assetId, const AZ::Data::AssetInfo& assetInfo)
    {
        if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationAgentAsset>())
        {
            // A navigation agent asset was removed, so stop listening for it and remove it from our set of loaded assets.
            AZ::Data::AssetBus::MultiHandler::BusDisconnect(assetId);
            m_navigationAgentAssets.erase(assetId);
        }
        else if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshAreaAsset>())
        {
            // A navigation mesh area asset was removed, so stop listening for it and remove it from our set of loaded assets.
            AZ::Data::AssetBus::MultiHandler::BusDisconnect(assetId);
            m_navigationMeshAreaAssets.erase(assetId);
        }
        else if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
        {
            // A navigation mesh settings asset was removed, so stop listening for it and remove it from our set of loaded assets.
            AZ::Data::AssetBus::MultiHandler::BusDisconnect(assetId);
            m_navigationMeshSettingsAssets.erase(assetId);
        }
    }

    void BehaveNavigationEditorSystemComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        AddAsset(asset);
    }

    void BehaveNavigationEditorSystemComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        AddAsset(asset);
    }

    void BehaveNavigationEditorSystemComponent::GetRegisteredNavigationMeshAreaNames(BehaveNavigationMeshAreaNameSet& names) const
    {
        names.clear();
        names.insert(kDefaultNavigationMeshAreaName);

        for (auto&& assetPair : m_navigationMeshAreaAssets)
        {
            if (const auto& asset = assetPair.second; asset.IsReady())
            {
                for (const auto& area : asset->mAreas)
                {
                    names.insert(area.GetName());
                }
            }
        }
    }

    void BehaveNavigationEditorSystemComponent::GetRegisteredNavigationMeshAreas(BehaveNavigationMeshAreaVector& areas) const
    {
        areas.clear();
        areas.push_back(BehaveNavigationMeshArea::Default());

        AZ::u8 id = kDefaultNavigationMeshAreaId;

        for (auto&& assetPair : m_navigationMeshAreaAssets)
        {
            if (const auto& asset = assetPair.second; asset.IsReady())
            {
                for (BehaveNavigationMeshArea area : asset->mAreas)
                {
                    area.SetId(++id);
                    areas.push_back(area);
                }
            }
        }
    }

    BehaveNavigationMeshArea BehaveNavigationEditorSystemComponent::GetNavigationMeshArea(const AZStd::string& name) const
    {
        for (auto&& assetPair : m_navigationMeshAreaAssets)
        {
            if (const auto& asset = assetPair.second; asset.IsReady())
            {
                for (const auto& area : asset->mAreas)
                {
                    if (area.GetName() == name)
                    {
                        return area;
                    }
                }
            }
        }

        return BehaveNavigationMeshArea::Default();
    }
} // namespace SparkyStudios::AI::Behave::Navigation

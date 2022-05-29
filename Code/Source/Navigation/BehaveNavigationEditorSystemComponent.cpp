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
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/Settings/SettingsRegistryMergeUtils.h>

#include <AzFramework/Asset/GenericAssetHandler.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    namespace Assets
    {
        static constexpr char kBehaveAssetCategoryName[] = "Behave AI";

#pragma region BehaveNavigationAgentAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationAgentAsset>* s_navigationAgentsAssetHandler = nullptr;

        static constexpr char kDefaultNavigationAgentsAssetFilePath[] = "Assets/AI/Navigation/Agents";

        static constexpr char kNavigationAgentAssetDisplayName[] = "Navigation Agent";
        static constexpr char kNavigationAgentAssetExtension[] = "bhnavagent";
#pragma endregion

#pragma region BehaveNavigationMeshAreaAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationMeshAreaAsset>* s_navigationMeshAreaAssetHandler = nullptr;

        static constexpr char kDefaultNavigationMeshAreasAssetFilePath[] = "Assets/AI/Navigation/Areas";

        static constexpr char kNavigationMeshAreaAssetDisplayName[] = "Navigation Mesh Area";
        static constexpr char kNavigationMeshAreaAssetExtension[] = "bhnavmesharea";
#pragma endregion

#pragma region BehaveNavigationMeshSettingsAsset
        static AzFramework::GenericAssetHandler<BehaveNavigationMeshSettingsAsset>* s_navigationMeshSettingAssetHandler = nullptr;

        static constexpr char kNavigationMeshSettingsAssetDisplayName[] = "Navigation Mesh Settings";
        static constexpr char kNavigationMeshSettingsAssetExtension[] = "bhnavmeshconfig";
#pragma endregion

        static void RegisterAssetHandlers()
        {
            s_navigationAgentsAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationAgentAsset>(
                kNavigationAgentAssetDisplayName, kBehaveAssetCategoryName, kNavigationAgentAssetExtension);

            s_navigationMeshAreaAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationMeshAreaAsset>(
                kNavigationMeshAreaAssetDisplayName, kBehaveAssetCategoryName, kNavigationMeshAreaAssetExtension);

            s_navigationMeshSettingAssetHandler = aznew AzFramework::GenericAssetHandler<BehaveNavigationMeshSettingsAsset>(
                kNavigationMeshSettingsAssetDisplayName, kBehaveAssetCategoryName, kNavigationMeshSettingsAssetExtension);

            s_navigationAgentsAssetHandler->Register();
            s_navigationMeshAreaAssetHandler->Register();
            s_navigationMeshSettingAssetHandler->Register();
        }

        static void UnregisterAssetHandlers()
        {
            if (s_navigationAgentsAssetHandler)
            {
                s_navigationAgentsAssetHandler->Unregister();
                delete s_navigationAgentsAssetHandler;
                s_navigationAgentsAssetHandler = nullptr;
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

        static AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> CreateDefaultNavigationMeshAreasAsset(
            const AZStd::string& fullTargetFilePath, const AZStd::string& relativePath)
        {
            AZ::IO::FileIOStream fileStream(fullTargetFilePath.c_str(), AZ::IO::OpenMode::ModeWrite);
            if (fileStream.IsOpen())
            {
                const auto& assetType = azrtti_typeid<BehaveNavigationMeshAreaAsset>();

                AZ::Data::AssetId assetId;
                EBUS_EVENT_RESULT(assetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), assetType, true);

                AZ::Data::Asset<AZ::Data::AssetData> newAsset =
                    AZ::Data::AssetManager::Instance().FindOrCreateAsset(assetId, assetType, AZ::Data::AssetLoadBehavior::Default);

                if (auto* asset = azrtti_cast<BehaveNavigationMeshAreaAsset*>(newAsset.GetData()))
                {
                    asset->mAreas.push_back(BehaveNavigationMeshArea(0, "Ground", 1, eNMAF_WALK));
                    asset->mAreas.push_back(BehaveNavigationMeshArea(1, "Water", 2, eNMAF_SWIM));
                    asset->mAreas.push_back(BehaveNavigationMeshArea(2, "Ledge", 1, eNMAF_JUMP));
                    asset->mAreas.push_back(BehaveNavigationMeshArea(3, "Snow", 5, eNMAF_WALK));
                    asset->mAreas.push_back(BehaveNavigationMeshArea(4, "Disabled", 0, eNMAF_DISABLED));

                    // Check it out in the source control system
                    EBUS_EVENT(
                        AzToolsFramework::SourceControlCommandBus, RequestEdit, fullTargetFilePath.c_str(), true,
                        [](bool /*success*/, const AzToolsFramework::SourceControlFileInfo& /*info*/)
                        {
                        });

                    // Save the asset into a file
                    if (s_navigationMeshAreaAssetHandler != nullptr &&
                        s_navigationMeshAreaAssetHandler->SaveAssetData(newAsset, &fileStream))
                    {
                        return newAsset;
                    }

                    AZ_Error(
                        "BehaveAI [Navigation]", false, "Unable to save default navigation mesh areas to %s", fullTargetFilePath.c_str());
                }
            }

            return AZStd::nullopt;
        }

        static AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> CreateDefaultNavigationAgentsAsset(
            const AZStd::string& fullTargetFilePath, const AZStd::string& relativePath)
        {
            AZ::IO::FileIOStream fileStream(fullTargetFilePath.c_str(), AZ::IO::OpenMode::ModeWrite);
            if (fileStream.IsOpen())
            {
                const auto& assetType = azrtti_typeid<BehaveNavigationAgentAsset>();

                AZ::Data::AssetId assetId;
                EBUS_EVENT_RESULT(assetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), assetType, true);

                AZ::Data::Asset<AZ::Data::AssetData> newAsset =
                    AZ::Data::AssetManager::Instance().FindOrCreateAsset(assetId, assetType, AZ::Data::AssetLoadBehavior::Default);

                if (auto* asset = azrtti_cast<BehaveNavigationAgentAsset*>(newAsset.GetData()))
                {
                    asset->mAgents.push_back(NavigationAgent());

                    // Check it out in the source control system
                    EBUS_EVENT(
                        AzToolsFramework::SourceControlCommandBus, RequestEdit, fullTargetFilePath.c_str(), true,
                        [](bool /*success*/, const AzToolsFramework::SourceControlFileInfo& /*info*/)
                        {
                        });

                    // Save the asset into a file
                    if (s_navigationAgentsAssetHandler != nullptr && s_navigationAgentsAssetHandler->SaveAssetData(newAsset, &fileStream))
                    {
                        return newAsset;
                    }

                    AZ_Error("BehaveAI [Navigation]", false, "Unable to save default navigation agents to %s", fullTargetFilePath.c_str());
                }
            }

            return AZStd::nullopt;
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

    void BehaveNavigationEditorSystemComponent::LoadNavigationMeshSettingsAsset(const AZ::Data::AssetId& assetId)
    {
        m_navigationMeshSettingsAssets[assetId] = AZ::Data::AssetManager::Instance().GetAsset(
            assetId, azrtti_typeid<BehaveNavigationMeshSettingsAsset>(), AZ::Data::AssetLoadBehavior::Default);

        AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);
    }

    void BehaveNavigationEditorSystemComponent::AddAsset(const AZ::Data::Asset<AZ::Data::AssetData>& asset)
    {
        if (asset.GetType() == azrtti_typeid<BehaveNavigationMeshAreaAsset>())
        {
            m_navigationMeshAreasAsset = asset;
        }
        else if (asset.GetType() == azrtti_typeid<BehaveNavigationAgentAsset>())
        {
            m_navigationAgentsAsset = asset;
        }
        else if (asset.GetType() == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
        {
            m_navigationMeshSettingsAssets[asset.GetId()] = asset;
        }

        EBUS_EVENT(AzToolsFramework::PropertyEditorGUIMessages::Bus, RequestRefresh, AzToolsFramework::Refresh_AttributesAndValues);
    }

    AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> BehaveNavigationEditorSystemComponent::RetrieveNavigationMeshAreas()
    {
        AZ::Data::AssetId resultAssetId;

        // Constructing the path to the asset
        const AZStd::string& assetExtension = Assets::kNavigationMeshAreaAssetExtension;

        // Use the path relative to the asset root to avoid hardcoding full path in the configuration
        AZStd::string relativePath = Assets::kDefaultNavigationMeshAreasAssetFilePath;
        AzFramework::StringFunc::Path::ReplaceExtension(relativePath, assetExtension.c_str());

        // Try to find an already existing asset
        EBUS_EVENT_RESULT(
            resultAssetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(),
            azrtti_typeid<BehaveNavigationMeshAreaAsset>(), false);

        if (!resultAssetId.IsValid())
        {
            // No file for the navigation mesh areas asset, create it
            AZ::IO::Path fullPath;
            if (const auto* settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
            {
                settingsRegistry->Get(fullPath.Native(), AZ::SettingsRegistryMergeUtils::FilePathKey_ProjectPath);
            }

            fullPath /= Assets::kDefaultNavigationMeshAreasAssetFilePath;
            fullPath.ReplaceExtension(AZ::IO::PathView(assetExtension));

            if (auto asset = Assets::CreateDefaultNavigationMeshAreasAsset(fullPath.Native(), relativePath))
            {
                return asset;
            }

            AZ_Warning(
                "BehaveAI [Navigation]", false,
                "Failed to create navigation mesh areas asset at %s. "
                "Please check if the file is writable",
                fullPath.c_str());
        }
        else
        {
            AZ::Data::Asset<AZ::Data::AssetData> asset = AZ::Data::AssetManager::Instance().GetAsset<BehaveNavigationMeshAreaAsset>(
                resultAssetId, AZ::Data::AssetLoadBehavior::NoLoad);

            return asset;
        }

        return AZStd::nullopt;
    }

    AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> BehaveNavigationEditorSystemComponent::RetrieveNavigationAgents()
    {
        AZ::Data::AssetId resultAssetId;

        // Constructing the path to the asset
        const AZStd::string& assetExtension = Assets::kNavigationAgentAssetExtension;

        // Use the path relative to the asset root to avoid hardcoding full path in the configuration
        AZStd::string relativePath = Assets::kDefaultNavigationAgentsAssetFilePath;
        AzFramework::StringFunc::Path::ReplaceExtension(relativePath, assetExtension.c_str());

        // Try to find an already existing asset
        EBUS_EVENT_RESULT(
            resultAssetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(),
            azrtti_typeid<BehaveNavigationAgentAsset>(), false);

        if (!resultAssetId.IsValid())
        {
            // No file for the navigation mesh areas asset, create it
            AZ::IO::Path fullPath;
            if (const auto* settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
            {
                settingsRegistry->Get(fullPath.Native(), AZ::SettingsRegistryMergeUtils::FilePathKey_ProjectPath);
            }

            fullPath /= Assets::kDefaultNavigationAgentsAssetFilePath;
            fullPath.ReplaceExtension(AZ::IO::PathView(assetExtension));

            if (auto asset = Assets::CreateDefaultNavigationAgentsAsset(fullPath.Native(), relativePath))
            {
                return asset;
            }

            AZ_Warning(
                "BehaveAI [Navigation]", false,
                "Failed to create navigation agents asset at %s. "
                "Please check if the file is writable",
                fullPath.c_str());
        }
        else
        {
            AZ::Data::Asset<AZ::Data::AssetData> asset =
                AZ::Data::AssetManager::Instance().GetAsset<BehaveNavigationAgentAsset>(resultAssetId, AZ::Data::AssetLoadBehavior::NoLoad);

            return asset;
        }

        return AZStd::nullopt;
    }

    void BehaveNavigationEditorSystemComponent::Activate()
    {
        Assets::RegisterAssetHandlers();

        BaseSystemComponent::Activate();

        AzFramework::AssetCatalogEventBus::Handler::BusConnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        BehaveNavigationMeshAreaProviderRequestBus::Handler::BusConnect();
        NavigationAgentProviderRequestBus::Handler::BusConnect();
    }

    void BehaveNavigationEditorSystemComponent::Deactivate()
    {
        NavigationAgentProviderRequestBus::Handler::BusDisconnect();
        BehaveNavigationMeshAreaProviderRequestBus::Handler::BusDisconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AzFramework::AssetCatalogEventBus::Handler::BusDisconnect();

        BaseSystemComponent::Deactivate();

        Assets::UnregisterAssetHandlers();

        AZ::Data::AssetBus::MultiHandler::BusDisconnect();
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogLoaded([[maybe_unused]] const char* string)
    {
        // Create or retrieve navigation mesh areas from library
        if (const auto asset = RetrieveNavigationMeshAreas(); asset.has_value())
        {
            m_navigationMeshAreasAsset = asset.value();
            AZ::Data::AssetBus::MultiHandler::BusConnect(asset->GetId());
        }

        // Create or retrieve navigation agents from library
        if (const auto asset = RetrieveNavigationAgents(); asset.has_value())
        {
            m_navigationAgentsAsset = asset.value();
            AZ::Data::AssetBus::MultiHandler::BusConnect(asset->GetId());
        }

        // Automatically register all managed assets at Editor startup
        AZStd::unordered_set<AZ::Data::AssetId> assetIds;

        // First run through all the assets and gather the asset IDs for all navigation agent assets
        EBUS_EVENT(
            AZ::Data::AssetCatalogRequestBus, EnumerateAssets, nullptr,
            [&assetIds](const AZ::Data::AssetId assetId, const AZ::Data::AssetInfo& assetInfo)
            {
                if (const auto assetType = azrtti_typeid<BehaveNavigationMeshSettingsAsset>(); assetInfo.m_assetType == assetType)
                {
                    assetIds.insert(assetId);
                }
            },
            nullptr);

        // Next, trigger all the loads. This is done outside EnumerateAssets to ensure that we don't have any deadlocks caused by
        // lock inversion. If this thread locks AssetCatalogRequestBus mutex with EnumerateAssets, then locks m_assetMutex in
        // AssetManager::FindOrCreateAsset, it's possible for those locks to get locked in reverse on a loading thread, causing a deadlock.
        for (auto&& assetId : assetIds)
        {
            LoadNavigationMeshSettingsAsset(assetId);
        }
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogAssetAdded(const AZ::Data::AssetId& assetId)
    {
        AZ::Data::AssetInfo assetInfo;
        EBUS_EVENT_RESULT(assetInfo, AZ::Data::AssetCatalogRequestBus, GetAssetInfoById, assetId);

        if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
        {
            // A new navigation mesh settings asset was added, so load it.
            LoadNavigationMeshSettingsAsset(assetId);
        }
    }

    void BehaveNavigationEditorSystemComponent::OnCatalogAssetRemoved(
        const AZ::Data::AssetId& assetId, const AZ::Data::AssetInfo& assetInfo)
    {
        if (assetInfo.m_assetType == azrtti_typeid<BehaveNavigationMeshSettingsAsset>())
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

        if (m_navigationMeshAreasAsset.IsReady())
        {
            for (const auto& area : m_navigationMeshAreasAsset->mAreas)
            {
                names.insert(area.GetName());
            }
        }
    }

    void BehaveNavigationEditorSystemComponent::GetRegisteredNavigationMeshAreas(BehaveNavigationMeshAreaVector& areas) const
    {
        areas.clear();
        areas.push_back(BehaveNavigationMeshArea::Default());

        if (m_navigationMeshAreasAsset.IsReady())
        {
            AZ::u8 id = 0;

            for (BehaveNavigationMeshArea area : m_navigationMeshAreasAsset->mAreas)
            {
                area.SetId(id++);
                areas.push_back(area);
            }
        }
    }

    BehaveNavigationMeshArea BehaveNavigationEditorSystemComponent::GetNavigationMeshArea(const AZStd::string& name) const
    {
        if (m_navigationMeshAreasAsset.IsReady())
        {
            for (const auto& area : m_navigationMeshAreasAsset->mAreas)
            {
                if (area.GetName() == name)
                {
                    return area;
                }
            }
        }

        return BehaveNavigationMeshArea::Default();
    }

    void BehaveNavigationEditorSystemComponent::GetRegisteredNavigationAgentNames(NavigationAgentNameSet& names) const
    {
        names.clear();

        if (m_navigationAgentsAsset.IsReady())
        {
            for (const auto& area : m_navigationAgentsAsset->mAgents)
            {
                names.insert(area.GetName());
            }
        }
    }

    void BehaveNavigationEditorSystemComponent::GetRegisteredNavigationAgents(NavigationAgentList& agents) const
    {
        agents.clear();

        if (m_navigationAgentsAsset.IsReady())
        {
            for (const auto& agent : m_navigationAgentsAsset->mAgents)
            {
                agents.push_back(agent);
            }
        }
    }

    NavigationAgent BehaveNavigationEditorSystemComponent::GetNavigationAgent(const AZStd::string& name) const
    {
        if (m_navigationAgentsAsset.IsReady())
        {
            for (const auto& area : m_navigationAgentsAsset->mAgents)
            {
                if (area.GetName() == name)
                {
                    return area;
                }
            }
        }

        return {};
    }
} // namespace SparkyStudios::AI::Behave::Navigation

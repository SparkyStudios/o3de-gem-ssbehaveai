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

#include <Navigation/NavigationEditorSystemComponent.h>
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

#pragma region NavigationAgentsAsset
        static AzFramework::GenericAssetHandler<NavigationAgentsAsset>* sNavigationAgentsAssetHandler = nullptr;

        static constexpr char kDefaultNavigationAgentsAssetFilePath[] = "Assets/AI/Navigation/Agents";

        static constexpr char kNavigationAgentsAssetDisplayName[] = "Navigation Agents";
        static constexpr char kNavigationAgentsAssetExtension[] = "bhnavagents";
#pragma endregion

#pragma region NavigationAreasAsset
        static AzFramework::GenericAssetHandler<NavigationAreasAsset>* sNavigationAreasAssetHandler = nullptr;

        static constexpr char kDefaultNavigationAreasAssetFilePath[] = "Assets/AI/Navigation/Areas";

        static constexpr char kNavigationAreasAssetDisplayName[] = "Navigation Areas";
        static constexpr char kNavigationAreasAssetExtension[] = "bhnavareas";
#pragma endregion

#pragma region NavigationMeshSettingsAsset
        static AzFramework::GenericAssetHandler<NavigationMeshSettingsAsset>* sNavigationMeshSettingsAssetHandler = nullptr;

        static constexpr char kNavigationMeshSettingsAssetDisplayName[] = "Navigation Settings";
        static constexpr char kNavigationMeshSettingsAssetExtension[] = "bhnavmeshconfig";
#pragma endregion

        static void RegisterAssetHandlers()
        {
            sNavigationAgentsAssetHandler = aznew AzFramework::GenericAssetHandler<NavigationAgentsAsset>(
                kNavigationAgentsAssetDisplayName, kBehaveAssetCategoryName, kNavigationAgentsAssetExtension);

            sNavigationAreasAssetHandler = aznew AzFramework::GenericAssetHandler<NavigationAreasAsset>(
                kNavigationAreasAssetDisplayName, kBehaveAssetCategoryName, kNavigationAreasAssetExtension);

            sNavigationMeshSettingsAssetHandler = aznew AzFramework::GenericAssetHandler<NavigationMeshSettingsAsset>(
                kNavigationMeshSettingsAssetDisplayName, kBehaveAssetCategoryName, kNavigationMeshSettingsAssetExtension);

            sNavigationAgentsAssetHandler->Register();
            sNavigationAreasAssetHandler->Register();
            sNavigationMeshSettingsAssetHandler->Register();
        }

        static void UnregisterAssetHandlers()
        {
            if (sNavigationAgentsAssetHandler)
            {
                sNavigationAgentsAssetHandler->Unregister();
                delete sNavigationAgentsAssetHandler;
                sNavigationAgentsAssetHandler = nullptr;
            }

            if (sNavigationAreasAssetHandler)
            {
                sNavigationAreasAssetHandler->Unregister();
                delete sNavigationAreasAssetHandler;
                sNavigationAreasAssetHandler = nullptr;
            }

            if (sNavigationMeshSettingsAssetHandler)
            {
                sNavigationMeshSettingsAssetHandler->Unregister();
                delete sNavigationMeshSettingsAssetHandler;
                sNavigationMeshSettingsAssetHandler = nullptr;
            }
        }

        static AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> CreateDefaultNavigationMeshAreasAsset(
            const AZStd::string& fullTargetFilePath, const AZStd::string& relativePath)
        {
            AZ::IO::FileIOStream fileStream(fullTargetFilePath.c_str(), AZ::IO::OpenMode::ModeWrite);
            if (fileStream.IsOpen())
            {
                const auto& assetType = azrtti_typeid<NavigationAreasAsset>();

                AZ::Data::AssetId assetId;
                EBUS_EVENT_RESULT(assetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), assetType, true);

                AZ::Data::Asset<AZ::Data::AssetData> newAsset =
                    AZ::Data::AssetManager::Instance().FindOrCreateAsset(assetId, assetType, AZ::Data::AssetLoadBehavior::Default);

                if (auto* asset = azrtti_cast<NavigationAreasAsset*>(newAsset.GetData()))
                {
                    asset->mAreas.push_back(NavigationArea(0, "Ground", 1, eNAF_WALK));
                    asset->mAreas.push_back(NavigationArea(1, "Water", 2, eNAF_SWIM));
                    asset->mAreas.push_back(NavigationArea(2, "Ledge", 1, eNAF_JUMP));
                    asset->mAreas.push_back(NavigationArea(3, "Snow", 5, eNAF_WALK));
                    asset->mAreas.push_back(NavigationArea(4, "Disabled", 0, eNAF_DISABLED));

                    // Check it out in the source control system
                    EBUS_EVENT(
                        AzToolsFramework::SourceControlCommandBus, RequestEdit, fullTargetFilePath.c_str(), true,
                        [](bool /*success*/, const AzToolsFramework::SourceControlFileInfo& /*info*/)
                        {
                        });

                    // Save the asset into a file
                    if (sNavigationAreasAssetHandler != nullptr && sNavigationAreasAssetHandler->SaveAssetData(newAsset, &fileStream))
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
                const auto& assetType = azrtti_typeid<NavigationAgentsAsset>();

                AZ::Data::AssetId assetId;
                EBUS_EVENT_RESULT(assetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), assetType, true);

                AZ::Data::Asset<AZ::Data::AssetData> newAsset =
                    AZ::Data::AssetManager::Instance().FindOrCreateAsset(assetId, assetType, AZ::Data::AssetLoadBehavior::Default);

                if (auto* asset = azrtti_cast<NavigationAgentsAsset*>(newAsset.GetData()))
                {
                    asset->mAgents.push_back(NavigationAgent());

                    // Check it out in the source control system
                    EBUS_EVENT(
                        AzToolsFramework::SourceControlCommandBus, RequestEdit, fullTargetFilePath.c_str(), true,
                        [](bool /*success*/, const AzToolsFramework::SourceControlFileInfo& /*info*/)
                        {
                        });

                    // Save the asset into a file
                    if (sNavigationAgentsAssetHandler != nullptr && sNavigationAgentsAssetHandler->SaveAssetData(newAsset, &fileStream))
                    {
                        return newAsset;
                    }

                    AZ_Error("BehaveAI [Navigation]", false, "Unable to save default navigation agents to %s", fullTargetFilePath.c_str());
                }
            }

            return AZStd::nullopt;
        }
    } // namespace Assets

    void NavigationEditorSystemComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<NavigationEditorSystemComponent, NavigationSystemComponent>()->Version(0);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<NavigationEditorSystemComponent>(
                      "Behave AI - Navigation System",
                      "2D navigation meshes and path finding for small and large worlds, powered by Recast Navigation.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void NavigationEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("BehaveAI_EditorNavigationService"));
    }

    void NavigationEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("BehaveAI_EditorNavigationService"));
    }

    void NavigationEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void NavigationEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void NavigationEditorSystemComponent::LoadNavigationMeshSettingsAsset(const AZ::Data::AssetId& assetId)
    {
        _navigationMeshSettingsAssets[assetId] = AZ::Data::AssetManager::Instance().GetAsset(
            assetId, azrtti_typeid<NavigationMeshSettingsAsset>(), AZ::Data::AssetLoadBehavior::Default);

        AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);
    }

    void NavigationEditorSystemComponent::AddAsset(const AZ::Data::Asset<AZ::Data::AssetData>& asset)
    {
        if (asset.GetType() == azrtti_typeid<NavigationAreasAsset>())
        {
            _navigationMeshAreasAsset = asset;
        }
        else if (asset.GetType() == azrtti_typeid<NavigationAgentsAsset>())
        {
            _navigationAgentsAsset = asset;
        }
        else if (asset.GetType() == azrtti_typeid<NavigationMeshSettingsAsset>())
        {
            _navigationMeshSettingsAssets[asset.GetId()] = asset;
        }

        EBUS_EVENT(AzToolsFramework::PropertyEditorGUIMessages::Bus, RequestRefresh, AzToolsFramework::Refresh_AttributesAndValues);
    }

    AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> NavigationEditorSystemComponent::RetrieveNavigationMeshAreas()
    {
        AZ::Data::AssetId resultAssetId;

        // Constructing the path to the asset
        const AZStd::string& assetExtension = Assets::kNavigationAreasAssetExtension;

        // Use the path relative to the asset root to avoid hardcoding full path in the configuration
        AZStd::string relativePath = Assets::kDefaultNavigationAreasAssetFilePath;
        AzFramework::StringFunc::Path::ReplaceExtension(relativePath, assetExtension.c_str());

        // Try to find an already existing asset
        EBUS_EVENT_RESULT(
            resultAssetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), azrtti_typeid<NavigationAreasAsset>(),
            false);

        if (!resultAssetId.IsValid())
        {
            // No file for the navigation mesh areas asset, create it
            AZ::IO::Path fullPath;
            if (const auto* settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
            {
                settingsRegistry->Get(fullPath.Native(), AZ::SettingsRegistryMergeUtils::FilePathKey_ProjectPath);
            }

            fullPath /= Assets::kDefaultNavigationAreasAssetFilePath;
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
            AZ::Data::Asset<AZ::Data::AssetData> asset =
                AZ::Data::AssetManager::Instance().GetAsset<NavigationAreasAsset>(resultAssetId, AZ::Data::AssetLoadBehavior::NoLoad);

            return asset;
        }

        return AZStd::nullopt;
    }

    AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> NavigationEditorSystemComponent::RetrieveNavigationAgents()
    {
        AZ::Data::AssetId resultAssetId;

        // Constructing the path to the asset
        const AZStd::string& assetExtension = Assets::kNavigationAgentsAssetExtension;

        // Use the path relative to the asset root to avoid hardcoding full path in the configuration
        AZStd::string relativePath = Assets::kDefaultNavigationAgentsAssetFilePath;
        AzFramework::StringFunc::Path::ReplaceExtension(relativePath, assetExtension.c_str());

        // Try to find an already existing asset
        EBUS_EVENT_RESULT(
            resultAssetId, AZ::Data::AssetCatalogRequestBus, GetAssetIdByPath, relativePath.c_str(), azrtti_typeid<NavigationAgentsAsset>(),
            false);

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
                AZ::Data::AssetManager::Instance().GetAsset<NavigationAgentsAsset>(resultAssetId, AZ::Data::AssetLoadBehavior::NoLoad);

            return asset;
        }

        return AZStd::nullopt;
    }

    void NavigationEditorSystemComponent::Activate()
    {
        Assets::RegisterAssetHandlers();

        BaseSystemComponent::Activate();

        AzFramework::AssetCatalogEventBus::Handler::BusConnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        NavigationAreaProviderRequestBus::Handler::BusConnect();
        NavigationAgentProviderRequestBus::Handler::BusConnect();
    }

    void NavigationEditorSystemComponent::Deactivate()
    {
        NavigationAgentProviderRequestBus::Handler::BusDisconnect();
        NavigationAreaProviderRequestBus::Handler::BusDisconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AzFramework::AssetCatalogEventBus::Handler::BusDisconnect();

        BaseSystemComponent::Deactivate();

        Assets::UnregisterAssetHandlers();

        AZ::Data::AssetBus::MultiHandler::BusDisconnect();
    }

    void NavigationEditorSystemComponent::OnCatalogLoaded([[maybe_unused]] const char* string)
    {
        // Create or retrieve navigation mesh areas from library
        if (const auto asset = RetrieveNavigationMeshAreas(); asset.has_value())
        {
            _navigationMeshAreasAsset = asset.value();
            AZ::Data::AssetBus::MultiHandler::BusConnect(asset->GetId());
        }

        // Create or retrieve navigation agents from library
        if (const auto asset = RetrieveNavigationAgents(); asset.has_value())
        {
            _navigationAgentsAsset = asset.value();
            AZ::Data::AssetBus::MultiHandler::BusConnect(asset->GetId());
        }

        // Automatically register all managed assets at Editor startup
        AZStd::unordered_set<AZ::Data::AssetId> assetIds;

        // First run through all the assets and gather the asset IDs for all navigation agent assets
        EBUS_EVENT(
            AZ::Data::AssetCatalogRequestBus, EnumerateAssets, nullptr,
            [&assetIds](const AZ::Data::AssetId assetId, const AZ::Data::AssetInfo& assetInfo)
            {
                if (const auto assetType = azrtti_typeid<NavigationMeshSettingsAsset>(); assetInfo.m_assetType == assetType)
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

    void NavigationEditorSystemComponent::OnCatalogAssetAdded(const AZ::Data::AssetId& assetId)
    {
        AZ::Data::AssetInfo assetInfo;
        EBUS_EVENT_RESULT(assetInfo, AZ::Data::AssetCatalogRequestBus, GetAssetInfoById, assetId);

        if (assetInfo.m_assetType == azrtti_typeid<NavigationMeshSettingsAsset>())
        {
            // A new navigation mesh settings asset was added, so load it.
            LoadNavigationMeshSettingsAsset(assetId);
        }
    }

    void NavigationEditorSystemComponent::OnCatalogAssetRemoved(const AZ::Data::AssetId& assetId, const AZ::Data::AssetInfo& assetInfo)
    {
        if (assetInfo.m_assetType == azrtti_typeid<NavigationMeshSettingsAsset>())
        {
            // A navigation mesh settings asset was removed, so stop listening for it and remove it from our set of loaded assets.
            AZ::Data::AssetBus::MultiHandler::BusDisconnect(assetId);
            _navigationMeshSettingsAssets.erase(assetId);
        }
    }

    void NavigationEditorSystemComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        AddAsset(asset);
    }

    void NavigationEditorSystemComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        AddAsset(asset);
    }

    void NavigationEditorSystemComponent::GetRegisteredNavigationAreaNames(BehaveNavigationMeshAreaNameSet& names) const
    {
        names.clear();
        names.insert(kDefaultNavigationMeshAreaName);

        if (_navigationMeshAreasAsset.IsReady())
        {
            for (const auto& area : _navigationMeshAreasAsset->mAreas)
            {
                names.insert(area.GetName());
            }
        }
    }

    void NavigationEditorSystemComponent::GetRegisteredNavigationAreas(BehaveNavigationMeshAreaVector& areas) const
    {
        areas.clear();
        areas.push_back(NavigationArea::Default());

        if (_navigationMeshAreasAsset.IsReady())
        {
            AZ::u8 id = 0;

            for (NavigationArea area : _navigationMeshAreasAsset->mAreas)
            {
                area.SetId(id++);
                areas.push_back(area);
            }
        }
    }

    NavigationArea NavigationEditorSystemComponent::GetNavigationArea(const AZStd::string& name) const
    {
        if (_navigationMeshAreasAsset.IsReady())
        {
            for (const auto& area : _navigationMeshAreasAsset->mAreas)
            {
                if (area.GetName() == name)
                {
                    return area;
                }
            }
        }

        return NavigationArea::Default();
    }

    void NavigationEditorSystemComponent::GetRegisteredNavigationAgentNames(NavigationAgentNameSet& names) const
    {
        names.clear();

        if (_navigationAgentsAsset.IsReady())
        {
            for (const auto& area : _navigationAgentsAsset->mAgents)
            {
                names.insert(area.GetName());
            }
        }
    }

    void NavigationEditorSystemComponent::GetRegisteredNavigationAgents(NavigationAgentList& agents) const
    {
        agents.clear();

        if (_navigationAgentsAsset.IsReady())
        {
            for (const auto& agent : _navigationAgentsAsset->mAgents)
            {
                agents.push_back(agent);
            }
        }
    }

    NavigationAgent NavigationEditorSystemComponent::GetNavigationAgent(const AZStd::string& name) const
    {
        if (_navigationAgentsAsset.IsReady())
        {
            for (const auto& area : _navigationAgentsAsset->mAgents)
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

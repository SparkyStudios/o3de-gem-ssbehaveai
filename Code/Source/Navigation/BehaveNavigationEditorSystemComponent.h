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

#pragma once

#include <Navigation/Assets/BehaveNavigationAgentAsset.h>
#include <Navigation/Assets/BehaveNavigationMeshAreaAsset.h>
#include <Navigation/Assets/BehaveNavigationMeshSettingsAsset.h>
#include <Navigation/BehaveNavigationMeshAreaProviderRequestBus.h>
#include <Navigation/BehaveNavigationSystemComponent.h>
#include <Navigation/NavigationAgentProviderRequestBus.h>

#include <AzFramework/Asset/AssetCatalogBus.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    /// System component for BehaveNavigation editor
    class BehaveNavigationEditorSystemComponent
        : public BehaveNavigationSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
        , private AzFramework::AssetCatalogEventBus::Handler
        , private AZ::Data::AssetBus::MultiHandler
        , private BehaveNavigationMeshAreaProviderRequestBus::Handler
        , private NavigationAgentProviderRequestBus::Handler
    {
        using BaseSystemComponent = BehaveNavigationSystemComponent;

    public:
        AZ_COMPONENT(BehaveNavigationEditorSystemComponent, "{2F22E936-57CC-4739-97A1-1175042179AD}", BaseSystemComponent);

        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        BehaveNavigationEditorSystemComponent() = default;
        ~BehaveNavigationEditorSystemComponent() override = default;

    private:
        void LoadNavigationMeshSettingsAsset(const AZ::Data::AssetId& assetId);
        void AddAsset(const AZ::Data::Asset<AZ::Data::AssetData>& asset);
        AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> RetrieveNavigationMeshAreas();
        AZStd::optional<AZ::Data::Asset<AZ::Data::AssetData>> RetrieveNavigationAgents();

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AzFramework::AssetCatalogEventBus
        void OnCatalogLoaded(const char* /*catalogFile*/) override;
        void OnCatalogAssetAdded(const AZ::Data::AssetId& assetId) override;
        void OnCatalogAssetRemoved(const AZ::Data::AssetId& assetId, const AZ::Data::AssetInfo& assetInfo) override;

        // AZ::Data::AssetBus
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        // BehaveNavigationMeshAreaProviderRequestBus
        void GetRegisteredNavigationMeshAreaNames(BehaveNavigationMeshAreaNameSet& names) const override;
        void GetRegisteredNavigationMeshAreas(BehaveNavigationMeshAreaVector& areas) const override;
        [[nodiscard]] BehaveNavigationMeshArea GetNavigationMeshArea(const AZStd::string& name) const override;

        // NavigationAgentProviderRequests
        void GetRegisteredNavigationAgentNames(NavigationAgentNameSet& names) const override;
        void GetRegisteredNavigationAgents(NavigationAgentList& agents) const override;
        [[nodiscard]] NavigationAgent GetNavigationAgent(const AZStd::string& name) const override;

        AZ::Data::Asset<BehaveNavigationAgentAsset> m_navigationAgentsAsset;
        AZ::Data::Asset<BehaveNavigationMeshAreaAsset> m_navigationMeshAreasAsset;
        AZStd::unordered_map<AZ::Data::AssetId, AZ::Data::Asset<BehaveNavigationMeshSettingsAsset>> m_navigationMeshSettingsAssets;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

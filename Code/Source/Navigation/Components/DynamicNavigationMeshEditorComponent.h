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

#include <Navigation/Components/DynamicNavigationMeshComponent.h>
#include <Navigation/Utils/RecastNavMeshDebugDraw.h>
#include <Navigation/Utils/RecastNavigationMesh.h>

#include <AzCore/Component/TransformBus.h>

#include <AzFramework/Entity/EntityDebugDisplayBus.h>

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

#include <LmbrCentral/Shape/ShapeComponentBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class DynamicNavigationMeshEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , public AZ::TickBus::Handler
        , public INavigationMesh
        , protected NavigationMeshNotificationBus::Handler
        , private LmbrCentral::ShapeComponentNotificationsBus::Handler
        , private AzFramework::EntityDebugDisplayEventBus::Handler
        , private AZ::Data::AssetBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(DynamicNavigationMeshEditorComponent, "{819CE185-5738-406A-AADB-E5B0177B1D05}");

        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ~DynamicNavigationMeshEditorComponent() override = default;

        AZ::u32 NavigationMeshSettingsAssetUpdated();

        // SparkyStudios::AI::Behave::Navigation::INavigationMesh
        [[nodiscard]] const NavigationMeshSettingsAsset* GetSettings() const override;
        [[nodiscard]] const AZ::Aabb& GetBoundingBox() const override;
        [[nodiscard]] const OffMeshConnections& GetOffMeshConnections() const override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;

        // AZ::TickBus
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        int GetTickOrder() override;

        // SparkyStudios::AI::Behave::Navigation::NavigationMeshNotificationBus
        void OnNavigationMeshUpdated() override;

        // LmbrCentral::ShapeComponentNotificationsBus
        void OnShapeChanged(ShapeChangeReasons changeReason) override;

        // AzToolsFramework::EntityDebugDisplayEventBus
        void DisplayEntityViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay) override;

        // AZ::Data::AssetBus
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

    private:
        using NavigationAgentList = AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>>;

        void SetSettings(const AZ::Data::Asset<AZ::Data::AssetData>& settings = {});
        void UpdateNavMeshAABB();

        AZ::Crc32 OnBuildNavigationMesh();

        AZ::Transform _currentEntityTransform{};

        bool _enableDebug = false;
        bool _depthTest = false;
        RecastNavMeshDebugDraw _debugDraw{};

        AZ::Data::Asset<NavigationMeshSettingsAsset> _settings{};
        AZ::Aabb _aabb = AZ::Aabb::CreateNull();
        OffMeshConnections _offMeshConnections;

        bool _waitingOnNavMeshBuild = false;
        RecastNavigationMesh* _navigationMesh = nullptr;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

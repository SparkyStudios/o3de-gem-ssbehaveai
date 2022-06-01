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
        , public IBehaveNavigationMesh
        , protected BehaveNavigationMeshNotificationBus::Handler
        , private LmbrCentral::ShapeComponentNotificationsBus::Handler
        , private AzFramework::EntityDebugDisplayEventBus::Handler
        , private AZ::Data::AssetBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(DynamicNavigationMeshEditorComponent, "{819CE185-5738-406A-AADB-E5B0177B1D05}");

        // Perform reflection for this component. The context parameter is the reflection context.
        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ~DynamicNavigationMeshEditorComponent() override;

        AZ::u32 NavigationMeshSettingsAssetUpdated();

        // SparkyStudios::AI::Behave::Navigation::IBehaveNavigationMesh
        [[nodiscard]] const BehaveNavigationMeshSettingsAsset* GetSettings() const override;
        [[nodiscard]] const AZ::Aabb& GetBoundingBox() const override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;

        // AZ::TickBus
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        int GetTickOrder() override;

        // SparkyStudios::AI::Behave::Navigation::BehaveNavigationMeshNotificationBus
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

        [[nodiscard]] AZ::Crc32 OnBuildNavigationMesh() const;
        [[nodiscard]] AZ::Crc32 GetBuildButtonState() const;

        AZ::Transform _currentEntityTransform{};

        bool _enableDebug = false;
        bool _depthTest = false;
        RecastNavMeshDebugDraw _debugDraw{};

        AZ::Data::Asset<BehaveNavigationMeshSettingsAsset> _settings{};
        AZ::Aabb _aabb = AZ::Aabb::CreateNull();

        bool _waitingOnNavMeshBuild = false;
        RecastNavigationMesh* _navigationMesh = nullptr;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

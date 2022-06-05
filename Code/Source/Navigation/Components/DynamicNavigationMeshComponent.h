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

#include <SparkyStudios/AI/Behave/Navigation/NavigationMeshBus.h>
#include <SparkyStudios/AI/Behave/Navigation/INavigationMesh.h>

#include <Navigation/Assets/NavigationMeshSettingsAsset.h>
#include <Navigation/Utils/RecastNavigationMesh.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework/Input/Events/InputChannelEventListener.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class DynamicNavigationMeshComponent
        : public AZ::Component
        , public AZ::TickBus::Handler
        , public INavigationMesh
        , protected NavigationMeshRequestBus::Handler
        , protected NavigationMeshNotificationBus::Handler
        , protected AzFramework::GameEntityContextEventBus::Handler
    {
        friend class DynamicNavigationMeshEditorComponent;

    public:
        AZ_COMPONENT(DynamicNavigationMeshComponent, "{A7F61BD8-0BAD-453A-ABEF-9A893CBE9AFF}");

        static void Reflect(AZ::ReflectContext* rc);

        DynamicNavigationMeshComponent() = default;
        explicit DynamicNavigationMeshComponent(AZ::Data::Asset<NavigationMeshSettingsAsset> settings, AZ::Aabb aabb, OffMeshConnections offMeshConnections);

        // INavigationMesh
        [[nodiscard]] const NavigationMeshSettingsAsset* GetSettings() const override;
        [[nodiscard]] const AZ::Aabb& GetBoundingBox() const override;
        [[nodiscard]] const OffMeshConnections& GetOffMeshConnections() const override;

        // NavigationMeshRequestBus
        bool UpdateNavigationMesh() override;
        AZStd::vector<AZ::Vector3> FindPathToEntity(const AZ::EntityId& from, const AZ::EntityId& to) override;
        AZStd::vector<AZ::Vector3> FindPathToPosition(const AZ::Vector3& from, const AZ::Vector3& to) override;

        void OnNavigationMeshUpdated() override;

        // AzFramework::GameEntityContextEventBus
        void OnGameEntitiesStarted() override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // AZ::TickBus
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

    private:
        AZ::Data::Asset<NavigationMeshSettingsAsset> _settings;
        AZ::Aabb _aabb = AZ::Aabb::CreateNull();
        OffMeshConnections _offMeshConnections;

        RecastNavigationMesh* _navigationMesh = nullptr;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

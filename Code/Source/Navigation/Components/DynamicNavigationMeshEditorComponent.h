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
#include <Navigation/Utils/RecastDebugDraw.h>
#include <Navigation/Utils/RecastNavigationMesh.h>

#include <AzCore/Component/TransformBus.h>
#include <AzCore/Serialization/DynamicSerializableField.h>
#include <AzCore/Serialization/EditContext.h>

#include <AzFramework/Entity/EntityDebugDisplayBus.h>

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

#include <LmbrCentral/Shape/ShapeComponentBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class DynamicNavigationMeshEditorComponent final
        : public AzToolsFramework::Components::EditorComponentBase
        , public AZ::TickBus::Handler
        , protected BehaveNavigationMeshNotificationBus::Handler
        , private LmbrCentral::ShapeComponentNotificationsBus::Handler
        , private AZ::TransformNotificationBus::Handler
        , private AzFramework::EntityDebugDisplayEventBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(DynamicNavigationMeshEditorComponent, "{819CE185-5738-406A-AADB-E5B0177B1D05}");

        // Perform reflection for this component. The context parameter is the reflection context.
        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ~DynamicNavigationMeshEditorComponent() override;

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::TickBus
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        int GetTickOrder() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // SparkyStudios::AI::Behave::Navigation::BehaveNavigationMeshNotificationBus
        void OnNavigationMeshUpdated() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // LmbrCentral::ShapeComponentNotificationsBus
        void OnShapeChanged(ShapeChangeReasons changeReason) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::TransformNotificationBus::Handler
        void OnTransformChanged(const AZ::Transform& local, const AZ::Transform& world) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::EntityDebugDisplayEventBus
        void DisplayEntityViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        void UpdateNavMeshAABB();
        void SyncSettings();

        AZ::Crc32 OnBuildNavigationMesh();
        AZ::Crc32 GetBuildButtonState();

        AZ::Transform _currentEntityTransform{};

        bool _enableDebug{};
        bool _depthTest{};
        RecastDebugDraw _debugDraw;

        DynamicNavigationMeshComponent _navMeshComponent;
        NavigationMeshSettings _settings;

        bool _waitingOnNavMeshBuild = false;
        RecastNavigationMesh* _navigationMesh{};
    };
} // namespace SparkyStudios::AI::Behave::Navigation

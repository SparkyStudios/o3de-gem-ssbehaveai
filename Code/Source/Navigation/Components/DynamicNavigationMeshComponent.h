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

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework/Input/Events/InputChannelEventListener.h>

#include <AzFramework/Physics/Common/PhysicsSceneQueries.h>

#include <Navigation/Utils/RecastNavigationMesh.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class DynamicNavigationMeshComponent
        : public AZ::Component
        , protected BehaveNavigationMeshRequestBus::Handler
        , protected BehaveNavigationMeshNotificationBus::Handler
        , protected AzFramework::GameEntityContextEventBus::Handler
        , public AZ::TickBus::Handler
    {
        friend class DynamicNavigationMeshEditorComponent;

    public:
        AZ_COMPONENT(DynamicNavigationMeshComponent, "{A7F61BD8-0BAD-453A-ABEF-9A893CBE9AFF}");

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // BehaveNavigationMeshRequestBus
        bool UpdateNavigationMesh() override;
        AZStd::vector<AZ::Vector3> FindPathToEntity(AZ::EntityId from, AZ::EntityId to) override;
        AZStd::vector<AZ::Vector3> FindPathToPosition(const AZ::Vector3& from, const AZ::Vector3& to) override;
        //////////////////////////////////////////////////////////////////////////

        void OnNavigationMeshUpdated() override;

        //////////////////////////////////////////////////////////////////////////
        // AzFramework::GameEntityContextEventBus
        void OnGameEntitiesStarted() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::TickBus
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        NavigationMeshSettings _settings;
        RecastNavigationMesh* _navigationMesh = nullptr;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

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

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Aabb.h>
#include <AzCore/Math/Vector3.h>

class dtNavMeshQuery;
class dtNavMesh;

namespace SparkyStudios::AI::Behave::Navigation
{
    class BehaveNavigationMeshRequests : public AZ::ComponentBus
    {
    public:
        virtual bool UpdateNavigationMesh() = 0;

        virtual AZStd::vector<AZ::Vector3> FindPathToEntity(AZ::EntityId from, AZ::EntityId to) = 0;
        virtual AZStd::vector<AZ::Vector3> FindPathToPosition(const AZ::Vector3& from, const AZ::Vector3& target) = 0;
    };

    using BehaveNavigationMeshRequestBus = AZ::EBus<BehaveNavigationMeshRequests>;

    class BehaveNavigationMeshNotifications : public AZ::ComponentBus
    {
    public:
        virtual void OnNavigationMeshUpdated() = 0;
    };

    using BehaveNavigationMeshNotificationBus = AZ::EBus<BehaveNavigationMeshNotifications>;

    class BehaveWalkableInterface : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(BehaveWalkableInterface, "{0F974E8D-7601-47D1-B557-E771E4A83DEF}");

        //! Overrides the default AZ::EBusTraits handler policy to allow one listener only.
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

        virtual bool IsWalkable(AZ::EntityId navigationMeshEntity) = 0;
    };

    // Use to mark objects as walkable
    using BehaveWalkableRequestBus = AZ::EBus<BehaveWalkableInterface>;

} // namespace SparkyStudios::AI::Behave::Navigation

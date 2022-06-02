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

#include <Navigation/BehaveNavigationMeshAreaProviderRequestBus.h>
#include <Navigation/Components/DynamicNavigationMeshComponent.h>

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Jobs/JobFunction.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <AzFramework/Input/Devices/Keyboard/InputDeviceKeyboard.h>
#include <AzFramework/Physics/Shape.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void DynamicNavigationMeshComponent::Reflect(AZ::ReflectContext* rc)
    {
        OffMeshConnections::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // We may have been reflected by DynamicNavigationMeshEditorComponent already, so check first
            if (sc->FindClassData(azrtti_typeid<DynamicNavigationMeshComponent>()) == nullptr)
            {
                sc->Class<DynamicNavigationMeshComponent, AZ::Component>()
                    ->Version(0)
                    ->Field("Settings", &DynamicNavigationMeshComponent::_settings)
                    ->Field("OffMeshConnections", &DynamicNavigationMeshComponent::_offMeshConnections)
                    ->Field("Bounds", &DynamicNavigationMeshComponent::_aabb);
            }
        }
    }

    DynamicNavigationMeshComponent::DynamicNavigationMeshComponent(
        AZ::Data::Asset<BehaveNavigationMeshSettingsAsset> settings, AZ::Aabb aabb, OffMeshConnections offMeshConnections)
        : _settings(AZStd::move(settings))
        , _aabb(AZStd::move(aabb))
        , _offMeshConnections(AZStd::move(offMeshConnections))
    {
    }

    const BehaveNavigationMeshSettingsAsset* DynamicNavigationMeshComponent::GetSettings() const
    {
        return _settings.Get();
    }

    const AZ::Aabb& DynamicNavigationMeshComponent::GetBoundingBox() const
    {
        return _aabb;
    }

    const OffMeshConnections& DynamicNavigationMeshComponent::GetOffMeshConnections() const
    {
        return _offMeshConnections;
    }

    bool DynamicNavigationMeshComponent::UpdateNavigationMesh()
    {
        return _navigationMesh->BuildNavigationMesh(this);
    }

    AZStd::vector<AZ::Vector3> DynamicNavigationMeshComponent::FindPathToEntity(const AZ::EntityId& from, const AZ::EntityId& to)
    {
        if (_navigationMesh->IsNavigationMeshReady())
        {
            if (from.IsValid() && to.IsValid())
            {
                AZ::Vector3 start = AZ::Vector3::CreateZero(), end = AZ::Vector3::CreateZero();
                AZ::TransformBus::EventResult(start, from, &AZ::TransformBus::Events::GetWorldTranslation);
                AZ::TransformBus::EventResult(end, to, &AZ::TransformBus::Events::GetWorldTranslation);

                return FindPathToPosition(start, end);
            }
        }

        return {};
    }

    AZStd::vector<AZ::Vector3> DynamicNavigationMeshComponent::FindPathToPosition(const AZ::Vector3& from, const AZ::Vector3& to)
    {
        if (!_navigationMesh->IsNavigationMeshReady())
            return {};

        AZStd::vector<AZ::Vector3> pathPoints;
        {
            const dtNavMeshQuery* navMeshQuery = _navigationMesh->GetNavigationMeshQuery();
            const dtNavMesh* navMesh = _navigationMesh->GetNavigationMesh();

            RecastVector3 startRecast{ from }, endRecast{ to };
            constexpr float halfExtents[3] = { 1.0f, 1.0f, 1.0f };

            dtPolyRef startPoly = 0, endPoly = 0;

            RecastVector3 nearestStartPoint, nearestEndPoint;

            const dtQueryFilter filter;

            dtStatus result = navMeshQuery->findNearestPoly(startRecast.data(), halfExtents, &filter, &startPoly, nearestStartPoint.data());
            if (result != DT_SUCCESS)
                return {};

            result = navMeshQuery->findNearestPoly(endRecast.data(), halfExtents, &filter, &endPoly, nearestEndPoint.data());
            if (result != DT_SUCCESS)
                return {};

            constexpr int maxPathLength = 100;
            dtPolyRef path[maxPathLength] = {};
            int pathLength = 0;

            // find an approximate path
            navMeshQuery->findPath(
                startPoly, endPoly, nearestStartPoint.data(), nearestEndPoint.data(), &filter, path, &pathLength, maxPathLength);

            AZStd::vector<RecastVector3> approximatePath;
            approximatePath.resize(pathLength);

            for (int pathIndex = 0; pathIndex < pathLength; ++pathIndex)
            {
                RecastVector3 center = RecastNavigationMesh::GetPolyCenter(navMesh, path[pathIndex]);
                approximatePath.push_back(center);
            }

            constexpr int maxDetailedPathLength = 100;
            RecastVector3 detailedPath[maxDetailedPathLength] = {};
            AZ::u8 detailedPathFlags[maxDetailedPathLength] = {};
            dtPolyRef detailedPolyPathRefs[maxDetailedPathLength] = {};
            int detailedPathCount = 0;

            navMeshQuery->findStraightPath(
                startRecast.data(), endRecast.data(), path, pathLength, detailedPath[0].data(), detailedPathFlags, detailedPolyPathRefs,
                &detailedPathCount, maxDetailedPathLength);

            // TODO: Debug Draw Path

            for (int i = 0; i < detailedPathCount; ++i)
            {
                pathPoints.push_back(detailedPath[i].AsVector3());
            }
        }

        return pathPoints;
    }

    void DynamicNavigationMeshComponent::OnNavigationMeshUpdated()
    {
    }

    void DynamicNavigationMeshComponent::OnGameEntitiesStarted()
    {
        UpdateNavigationMesh();
    }

    void DynamicNavigationMeshComponent::Init()
    {
    }

    void DynamicNavigationMeshComponent::Activate()
    {
        _navigationMesh = new RecastNavigationMesh(GetEntityId());

        BehaveNavigationMeshNotificationBus::Handler::BusConnect(GetEntityId());
        BehaveNavigationMeshRequestBus::Handler::BusConnect(GetEntityId());

        AzFramework::GameEntityContextEventBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void DynamicNavigationMeshComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        AzFramework::GameEntityContextEventBus::Handler::BusDisconnect();

        BehaveNavigationMeshRequestBus::Handler::BusDisconnect();
        BehaveNavigationMeshNotificationBus::Handler::BusDisconnect();

        delete _navigationMesh;
    }

    void DynamicNavigationMeshComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }
} // namespace SparkyStudios::AI::Behave::Navigation

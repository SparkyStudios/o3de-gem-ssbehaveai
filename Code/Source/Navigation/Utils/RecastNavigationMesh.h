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

#include <SparkyStudios/AI/Behave/Navigation/IBehaveNavigationMesh.h>

#include <Navigation/Utils/RecastMath.h>
#include <Navigation/Utils/RecastSmartPointer.h>

#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <Recast.h>

#include <AzCore/Math/Aabb.h>

#include <AzFramework/Physics/PhysicsScene.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class RecastNavigationMesh
    {
        friend class DynamicNavigationMeshEditorComponent;
        friend class DynamicNavigationMeshComponent;

    public:
        RecastNavigationMesh(AZ::EntityId navigationMeshEntityId, bool isEditor = false);

        bool BuildNavigationMesh(const IBehaveNavigationMesh* navMesh);

        dtNavMesh* GetNavigationMesh() const;
        dtNavMeshQuery* GetNavigationMeshQuery() const;

        bool IsNavigationMeshReady() const;

        static RecastVector3 GetPolyCenter(const dtNavMesh* navMesh, dtPolyRef ref);

    private:
        RecastGeometry GetColliderGeometry(const AZ::Aabb& aabb, const AzPhysics::SceneQueryHits& overlapHits);

        bool BuildNavigationMeshInternal();

        AZ::EntityId _entityId;
        bool _isEditor;

        const BehaveNavigationMeshSettingsAsset* _settings;
        AZ::Aabb _aabb;

        AZStd::atomic<bool> _navMeshReady = false;

        RecastGeometry _geom;
        AZStd::vector<RecastAreaConvexVolume> _areaConvexVolumes;

        AZStd::unique_ptr<rcContext> _context;
        AZStd::vector<AZ::u8> _trianglesArea;

        RecastPointer<rcHeightfield> _solidHeightField;
        RecastPointer<rcCompactHeightfield> _compactHeightField;
        RecastPointer<rcContourSet> _contourSet;
        RecastPointer<rcPolyMesh> _polyMesh;
        RecastPointer<rcPolyMeshDetail> _detailMesh;
        RecastPointer<dtNavMesh> _navMesh;
        RecastPointer<dtNavMeshQuery> _navQuery;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

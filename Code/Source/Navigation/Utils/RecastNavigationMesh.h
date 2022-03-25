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

#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <Recast.h>

#include <Navigation/Utils/RecastSmartPointer.h>
#include <Navigation/Utils/RecastVector3.h>

#include <AzCore/Math/Aabb.h>

#include <AzFramework/Physics/PhysicsScene.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    enum class NavigationMeshPartitionType
    {
        Watershed,
        Monotone,
        Layers,
    };

    struct NavigationMeshSettings
    {
    public:
        AZ_TYPE_INFO(NavigationMeshSettings, "{6390ABAE-0D7B-4F5B-8ED2-56A92C38CB52}");

        using NavigationMeshPartitionTypeComboBoxVec = AZStd::vector<AZStd::pair<NavigationMeshPartitionType, AZStd::string>>;

        static void Reflect(AZ::ReflectContext* rc);

        int m_borderPadding = 0;

        float m_cellSize = 0.3f;
        float m_cellHeight = 0.2f;

        int m_regionMinSize = 8;
        int m_regionMergedSize = 20;

        NavigationMeshPartitionType m_partitionType = NavigationMeshPartitionType::Watershed;

        bool m_filterLowHangingObstacles = true;
        bool m_filterLedgeSpans = true;
        bool m_filterWalkableLowHeightSpans = true;

        float m_edgeMaxError = 1.3f;
        int m_edgeMaxLength = 12;
        int m_maxVerticesPerPoly = 6;

        int m_detailSampleDist = 6;
        int m_detailSampleMaxError = 1;

        bool m_enableTiling = true;
        int m_tileSize = 16;

        AZ::Aabb m_aabb{};

    private:
        friend class DynamicNavigationMeshEditorComponent;
        friend class DynamicNavigationMeshComponent;

        AZStd::string StatGetVoxelSize() const;
    };

    class RecastNavigationMesh
    {
        friend class DynamicNavigationMeshEditorComponent;
        friend class DynamicNavigationMeshComponent;

    public:
        RecastNavigationMesh(AZ::EntityId navigationMeshEntityId, bool isEditor = false);

        bool BuildNavigationMesh(const NavigationMeshSettings& settings);

        dtNavMesh* GetNavigationMesh() const;
        dtNavMeshQuery* GetNavigationMeshQuery() const;

        bool IsNavigationMeshReady() const;

        static RecastVector3 GetPolyCenter(const dtNavMesh* navMesh, dtPolyRef ref);

    private:
        struct Geometry
        {
            AZStd::vector<RecastVector3> m_vertices;
            AZStd::vector<AZ::s32> m_indices;

            void Clear();
        };

        Geometry GetColliderGeometry(const AZ::Aabb& aabb, const AzPhysics::SceneQueryHits& overlapHits);

        bool BuildNavigationMeshInternal();

        AZ::EntityId _entityId;
        bool _isEditor;

        NavigationMeshSettings _settings;

        AZStd::atomic<bool> _navMeshReady = false;

        Geometry _geom;

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

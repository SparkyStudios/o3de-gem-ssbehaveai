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

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

#include <DetourNavMeshBuilder.h>

#include <Navigation/Assets/BehaveNavigationMeshSettingsAsset.h>
#include <Navigation/BehaveNavigationMeshAreaProviderRequestBus.h>
#include <Navigation/Utils/Constants.h>
#include <Navigation/Utils/RecastContext.h>
#include <Navigation/Utils/RecastNavigationMesh.h>

#include <AzCore/Component/TransformBus.h>
#include <AzCore/Jobs/JobFunction.h>

#include <AzFramework/Physics/Common/PhysicsSceneQueries.h>
#include <AzFramework/Physics/PhysicsScene.h>
#include <AzFramework/Physics/Shape.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    RecastNavigationMesh::RecastNavigationMesh(AZ::EntityId navigationMeshEntityId, bool isEditor)
        : _entityId(AZStd::move(navigationMeshEntityId))
        , _isEditor(isEditor)
        , _areaConvexVolumes()
    {
        _context = AZStd::make_unique<RecastContext>();
    }

    dtNavMesh* RecastNavigationMesh::GetNavigationMesh() const
    {
        return _navMesh.get();
    }

    dtNavMeshQuery* RecastNavigationMesh::GetNavigationMeshQuery() const
    {
        return _navQuery.get();
    }

    bool RecastNavigationMesh::IsNavigationMeshReady() const
    {
        return _navMeshReady.load();
    }

    RecastVector3 RecastNavigationMesh::GetPolyCenter(const dtNavMesh* navMesh, const dtPolyRef ref)
    {
        RecastVector3 center(0, 0, 0);

        const dtMeshTile* tile = nullptr;
        const dtPoly* poly = nullptr;

        if (const dtStatus status = navMesh->getTileAndPolyByRef(ref, &tile, &poly); dtStatusFailed(status))
            return {};

        if (poly->vertCount == 0)
            return {};

        for (int i = 0; i < aznumeric_cast<int>(poly->vertCount); ++i)
        {
            const float* v = &tile->verts[poly->verts[i] * 3];
            center.mX += v[0];
            center.mY += v[1];
            center.mZ += v[2];
        }

        const float s = 1.0f / aznumeric_cast<float>(poly->vertCount);
        center.mX *= s;
        center.mY *= s;
        center.mZ *= s;

        return center;
    }

    RecastGeometry RecastNavigationMesh::GetColliderGeometry(const AZ::Aabb& aabb, const AzPhysics::SceneQueryHits& overlapHits)
    {
        RecastGeometry geom{};

        AZ::Aabb volumeAabb = aabb;

        AZStd::size_t indicesCount = 0;

        AZStd::vector<AZ::Vector3> vertices;
        AZStd::vector<AZ::u32> indices;

        for (const AzPhysics::SceneQueryHit& overlapHit : overlapHits.m_hits)
        {
            if ((overlapHit.m_resultFlags & AzPhysics::SceneQuery::EntityId) != 0)
            {
                AZ::EntityId hitEntityId = overlapHit.m_entityId;

                bool isWalkable = false, isArea = false;
                EBUS_EVENT_ID_RESULT(isWalkable, hitEntityId, BehaveWalkableRequestBus, IsWalkable, _entityId);
                EBUS_EVENT_ID_RESULT(isArea, hitEntityId, BehaveNavigationMeshAreaRequestBus, IsNavigationMeshArea, _entityId);

                // If this collider is not a part of the navigation mesh generation, skip it
                if (!isWalkable && !isArea)
                    continue;

                AZ::Transform t = AZ::Transform::CreateIdentity();
                EBUS_EVENT_ID_RESULT(t, hitEntityId, AZ::TransformBus, GetWorldTM);

                if (isArea)
                {
                    RecastAreaConvexVolume area;

                    BehaveNavigationMeshArea areaSettings;
                    EBUS_EVENT_ID_RESULT(areaSettings, hitEntityId, BehaveNavigationMeshAreaRequestBus, GetNavigationMeshArea);
                    AZ::PolygonPrism areaPolygon;
                    EBUS_EVENT_ID_RESULT(areaPolygon, hitEntityId, BehaveNavigationMeshAreaRequestBus, GetNavigationMeshAreaPolygon);

                    area = RecastAreaConvexVolume(areaPolygon, t);
                    area.mArea = static_cast<AZ::u8>(areaSettings);

                    _areaConvexVolumes.push_back(area);
                }
                else if (isWalkable)
                {
                    // Most physics bodies just have world transforms, but some also have local transforms including terrain.
                    // We are not applying the local orientation because it causes terrain geometry to be oriented incorrectly

                    vertices.clear();
                    indices.clear();

                    overlapHit.m_shape->GetGeometry(vertices, indices, &volumeAabb);
                    if (!vertices.empty())
                    {
                        if (!indices.empty())
                        {
                            for (const AZ::Vector3& vertex : vertices)
                            {
                                const AZ::Vector3 translated = t.TransformPoint(vertex);

                                geom.mVertices.push_back(RecastVector3(translated));
                            }

                            for (size_t i = 2; i < indices.size(); i += 3)
                            {
                                geom.mIndices.push_back(aznumeric_cast<AZ::u32>(indicesCount + indices[i - 0]));
                                geom.mIndices.push_back(aznumeric_cast<AZ::u32>(indicesCount + indices[i - 1]));
                                geom.mIndices.push_back(aznumeric_cast<AZ::u32>(indicesCount + indices[i - 2]));
                            }

                            indicesCount += vertices.size();
                        }
                        else
                        {
                            AZ_Assert(false, "Not implemented");
                        }
                    }
                }
            }
        }

        return geom;
    }

    bool RecastNavigationMesh::BuildNavigationMesh(const IBehaveNavigationMesh* navMesh)
    {
        _navMeshReady = false;
        _settings = navMesh->GetSettings();
        _aabb = navMesh->GetBoundingBox();

        _geom.Clear();
        _areaConvexVolumes.clear();

        AZ::Vector3 dimension = _aabb.GetExtents();
        AZ::Transform pose = AZ::Transform::CreateFromQuaternionAndTranslation(AZ::Quaternion::CreateIdentity(), _aabb.GetCenter());

        Physics::BoxShapeConfiguration shapeConfiguration;
        shapeConfiguration.m_dimensions = dimension;

        AzPhysics::OverlapRequest request = AzPhysics::OverlapRequestHelpers::CreateBoxOverlapRequest(dimension, pose, nullptr);
        request.m_queryType = AzPhysics::SceneQuery::QueryType::Static;
        request.m_collisionGroup = AzPhysics::CollisionGroup::All;

        auto sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get();
        AzPhysics::SceneHandle sceneHandle =
            sceneInterface->GetSceneHandle(_isEditor ? AzPhysics::EditorPhysicsSceneName : AzPhysics::DefaultPhysicsSceneName);
        AzPhysics::SceneQueryHits results = sceneInterface->QueryScene(sceneHandle, &request);

        if (results.m_hits.empty())
            return false;

        AZ_Printf("DynamicNavigationMeshComponent", "found %llu physx meshes", results.m_hits.size());

        _geom = GetColliderGeometry(_aabb, results);

        auto* job = AZ::CreateJobFunction(
            [this]()
            {
                if ((_navMeshReady = BuildNavigationMeshInternal()))
                {
                    EBUS_EVENT_ID(_entityId, BehaveNavigationMeshNotificationBus, OnNavigationMeshUpdated);
                }
            },
            true);

        job->Start();

        return true;
    }

    bool RecastNavigationMesh::BuildNavigationMeshInternal()
    {
        if (_geom.mVertices.empty())
            return false;

        const float* vertices = _geom.mVertices.front().data();
        const int vertexCount = static_cast<int>(_geom.mVertices.size());
        const int* triangleData = _geom.mIndices.data();
        const int triangleCount = static_cast<int>(_geom.mIndices.size()) / 3;

        // Step 1. Initialize build config.
        // --------------------------------

        // Init build configuration from settings
        rcConfig config{};
        memset(&config, 0, sizeof(config));

        config.cs = _settings->m_cellSize;
        config.ch = _settings->m_cellHeight;
        config.walkableSlopeAngle = _settings->m_agent.GetMaxSlopeAngle();
        config.walkableHeight = static_cast<int>(std::ceil(_settings->m_agent.GetHeight() / config.ch));
        config.walkableClimb = static_cast<int>(std::floor(_settings->m_agent.GetMaxClimb() / config.ch));
        config.walkableRadius = static_cast<int>(std::ceil(_settings->m_agent.GetRadius() / config.cs));
        config.maxEdgeLen = static_cast<int>(_settings->m_edgeMaxLength / _settings->m_cellSize);
        config.maxSimplificationError = _settings->m_edgeMaxError;
        config.minRegionArea = rcSqr<int>(_settings->m_regionMinSize); // Note: area = size*size
        config.mergeRegionArea = rcSqr<int>(_settings->m_regionMergedSize); // Note: area = size*size
        config.maxVertsPerPoly = static_cast<int>(_settings->m_maxVerticesPerPoly);
        config.detailSampleDist = _settings->m_detailSampleDist < 0.9f ? 0 : _settings->m_cellSize * _settings->m_detailSampleDist;
        config.detailSampleMaxError = _settings->m_cellHeight * _settings->m_detailSampleMaxError;

        // Set the area where the navigation will be build.
        // Here the bounds of the input mesh are used, but the
        // area could be specified by an user defined bounding box, etc.

        const RecastVector3 worldMin(_aabb.GetMin());
        const RecastVector3 worldMax(_aabb.GetMax());

        rcVcopy(config.bmin, &worldMin.mX);
        rcVcopy(config.bmax, &worldMax.mX);
        rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);

        // Reset build times gathering.
        _context->resetTimers();

        // Start the build process.
        _context->startTimer(RC_TIMER_TOTAL);

        _context->log(RC_LOG_PROGRESS, "Building navigation:");
        _context->log(RC_LOG_PROGRESS, " - %d x %d cells", config.width, config.height);
        _context->log(RC_LOG_PROGRESS, " - %d verts, %d triangles", vertexCount, triangleCount);

        // Step 2. Rasterize input polygon soup.
        // -------------------------------------

        // Allocate voxel height field where we rasterize our input data to.
        _solidHeightField.reset(rcAllocHeightfield());

        if (!_solidHeightField)
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Out of memory 'solid'.");
            return false;
        }

        if (!rcCreateHeightfield(
                _context.get(), *_solidHeightField, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not create solid height field.");
            return false;
        }

        // Allocate array that can hold triangle area types.
        // If we have multiple meshes we need to process, allocate
        // an array which can hold the max number of triangles we need to process.
        _trianglesArea.resize(triangleCount, 0);

        // Find triangles which are walkable based on their slope and rasterize them.
        // If our input data is multiple meshes, you can transform them here, calculate
        // the type for each of the meshes and rasterize them.
        rcMarkWalkableTriangles(
            _context.get(), config.walkableSlopeAngle, vertices, vertexCount, triangleData, triangleCount, _trianglesArea.data());

        if (!rcRasterizeTriangles(
                _context.get(), vertices, vertexCount, triangleData, _trianglesArea.data(), triangleCount,
                *_solidHeightField /*, config.walkableClimb*/))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not rasterize triangles.");
            return false;
        }

        _trianglesArea.clear();

        // Step 3. Filter walkable surfaces.
        // ---------------------------------

        // Once all geometry is rasterized, we do initial pass of filtering to
        // remove unwanted overhangs caused by the conservative rasterization
        // as well as filter spans where the character cannot possibly stand.
        if (_settings->m_filterLowHangingObstacles)
            rcFilterLowHangingWalkableObstacles(_context.get(), config.walkableClimb, *_solidHeightField);

        if (_settings->m_filterLedgeSpans)
            rcFilterLedgeSpans(_context.get(), config.walkableHeight, config.walkableClimb, *_solidHeightField);

        if (_settings->m_filterWalkableLowHeightSpans)
            rcFilterWalkableLowHeightSpans(_context.get(), config.walkableHeight, *_solidHeightField);

        // Step 4. Partition walkable surface to simple regions.
        // -----------------------------------------------------

        // Compact the height field so that it is faster to handle from now on.
        // This will result more cache coherent data as well as the neighbors
        // between walkable cells will be calculated.
        _compactHeightField.reset(rcAllocCompactHeightfield());

        if (!_compactHeightField)
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Out of memory 'chf'.");
            return false;
        }

        if (!rcBuildCompactHeightfield(
                _context.get(), config.walkableHeight, config.walkableClimb, *_solidHeightField, *_compactHeightField))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build compact data.");
            return false;
        }

        // Erode the walkable area by agent radius.
        if (!rcErodeWalkableArea(_context.get(), config.walkableRadius, *_compactHeightField))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not erode.");
            return false;
        }

        // Mark navigation mesh areas.
        for (const auto& v : _areaConvexVolumes)
        {
            rcMarkConvexPolyArea(
                _context.get(), v.mVertices.front().data(), static_cast<int>(v.mVertices.size()), v.mHMin, v.mHMax, v.mArea,
                *_compactHeightField);
        }

        if (_settings->m_partitionType == NavigationMeshPartitionType::Watershed)
        {
            // Prepare for region partitioning, by calculating distance field along the walkable surface.
            if (!rcBuildDistanceField(_context.get(), *_compactHeightField))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build distance field.");
                return false;
            }

            // Partition the walkable surface into simple regions without holes.
            if (!rcBuildRegions(_context.get(), *_compactHeightField, 0, config.minRegionArea, config.mergeRegionArea))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build watershed regions.");
                return false;
            }
        }
        else if (_settings->m_partitionType == NavigationMeshPartitionType::Monotone)
        {
            // Partition the walkable surface into simple regions without holes.
            // Monotone partitioning does not need distance field.
            if (!rcBuildRegionsMonotone(_context.get(), *_compactHeightField, 0, config.minRegionArea, config.mergeRegionArea))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build monotone regions.");
                return false;
            }
        }
        else // NavigationMeshPartitionType::Layers
        {
            // Partition the walkable surface into simple regions without holes.
            if (!rcBuildLayerRegions(_context.get(), *_compactHeightField, 0, config.minRegionArea))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build layer regions.");
                return false;
            }
        }

        // Step 5. Trace and simplify region contours.
        // -------------------------------------------

        // Create contours.
        _contourSet.reset(rcAllocContourSet());

        if (!_contourSet)
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Out of memory while allocating contours.");
            return false;
        }

        if (!rcBuildContours(_context.get(), *_compactHeightField, config.maxSimplificationError, config.maxEdgeLen, *_contourSet))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not create contours.");
            return false;
        }

        // Step 6. Build polygons mesh from contours.
        // ------------------------------------------

        // Build polygon nav mesh from the contours.
        _polyMesh.reset(rcAllocPolyMesh());

        if (!_polyMesh)
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Out of memory while allocating polygon nav mesh.");
            return false;
        }

        if (!rcBuildPolyMesh(_context.get(), *_contourSet, config.maxVertsPerPoly, *_polyMesh))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not triangulate contours.");
            return false;
        }

        // Step 7. Create detail mesh which allows to access approximate height on each polygon.
        // -------------------------------------------------------------------------------------

        _detailMesh.reset(rcAllocPolyMeshDetail());

        if (!_detailMesh)
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Out of memory while allocating detail mesh.");
            return false;
        }

        if (!rcBuildPolyMeshDetail(
                _context.get(), *_polyMesh, *_compactHeightField, config.detailSampleDist, config.detailSampleMaxError, *_detailMesh))
        {
            _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build detail mesh.");
            return false;
        }

        // Step 8. Create Detour data from Recast poly mesh.
        // -------------------------------------------------

        // The GUI may allow more max points per polygon than Detour can handle.
        // Only build the detour nav mesh if we do not exceed the limit.
        if (config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
        {
            BehaveNavigationMeshAreaVector areas;
            EBUS_EVENT(BehaveNavigationMeshAreaProviderRequestBus, GetRegisteredNavigationMeshAreas, areas);

            // Update poly flags from navigation mesh areas.
            if (!areas.empty())
            {
                for (int i = 0; i < _polyMesh->npolys; ++i)
                {
                    for (const auto& area : areas)
                    {
                        if (_polyMesh->areas[i] == area.GetId())
                        {
                            _polyMesh->flags[i] = area.GetFlags();
                            break;
                        }
                    }
                }
            }

            unsigned char* navData = nullptr;
            int navDataSize = 0;

            dtNavMeshCreateParams params{};

            // Poly Mesh
            params.verts = _polyMesh->verts;
            params.vertCount = _polyMesh->nverts;
            params.polys = _polyMesh->polys;
            params.polyAreas = _polyMesh->areas;
            params.polyFlags = _polyMesh->flags;
            params.polyCount = _polyMesh->npolys;
            params.nvp = _polyMesh->nvp;

            // Detail Mesh
            params.detailMeshes = _detailMesh->meshes;
            params.detailVerts = _detailMesh->verts;
            params.detailVertsCount = _detailMesh->nverts;
            params.detailTris = _detailMesh->tris;
            params.detailTriCount = _detailMesh->ntris;

            // Off-Mesh Connections
            params.offMeshConVerts = nullptr;
            params.offMeshConRad = nullptr;
            params.offMeshConDir = nullptr;
            params.offMeshConAreas = nullptr;
            params.offMeshConFlags = nullptr;
            params.offMeshConUserID = nullptr;
            params.offMeshConCount = 0;

            // Agent
            params.walkableHeight = _settings->m_agent.GetHeight();
            params.walkableRadius = _settings->m_agent.GetRadius();
            params.walkableClimb = _settings->m_agent.GetMaxClimb();

            // World
            rcVcopy(params.bmin, _polyMesh->bmin);
            rcVcopy(params.bmax, _polyMesh->bmax);
            params.cs = config.cs;
            params.ch = config.ch;
            params.buildBvTree = true;

            if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not build Detour nav mesh.");
                return false;
            }

            _navMesh.reset(dtAllocNavMesh());
            if (!_navMesh)
            {
                dtFree(navData);
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not create Detour nav mesh");
                return false;
            }

            dtStatus status = _navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
            if (dtStatusFailed(status))
            {
                dtFree(navData);
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not init Detour nav mesh");
                return false;
            }

            _navQuery.reset(dtAllocNavMeshQuery());

            status = _navQuery->init(_navMesh.get(), 2048);
            if (dtStatusFailed(status))
            {
                _context->log(RC_LOG_ERROR, "Navigation Mesh Builder: Could not init Detour nav mesh query");
                return false;
            }

            _context->stopTimer(RC_TIMER_TOTAL);

            return true;
        }

        return false;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

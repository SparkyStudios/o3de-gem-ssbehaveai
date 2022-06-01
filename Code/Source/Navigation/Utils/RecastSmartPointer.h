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

#include <DetourCrowd.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <Recast.h>

#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    template<typename T>
    struct RecastDeleter;

    template<typename T>
    using RecastPointer = AZStd::unique_ptr<T, RecastDeleter<T>>;

    template<>
    struct RecastDeleter<rcHeightfield>
    {
        void operator()(rcHeightfield* p) const
        {
            rcFreeHeightField(p);
        }
    };

    template<>
    struct RecastDeleter<rcCompactHeightfield>
    {
        void operator()(rcCompactHeightfield* p) const
        {
            rcFreeCompactHeightfield(p);
        }
    };

    template<>
    struct RecastDeleter<rcContourSet>
    {
        void operator()(rcContourSet* p) const
        {
            rcFreeContourSet(p);
        }
    };

    template<>
    struct RecastDeleter<rcPolyMesh>
    {
        void operator()(rcPolyMesh* p) const
        {
            rcFreePolyMesh(p);
        }
    };

    template<>
    struct RecastDeleter<rcPolyMeshDetail>
    {
        void operator()(rcPolyMeshDetail* p) const
        {
            rcFreePolyMeshDetail(p);
        }
    };

    template<>
    struct RecastDeleter<rcChunkedGeometry>
    {
        void operator()(rcChunkedGeometry* p) const
        {
            delete p;
        }
    };

    template<>
    struct RecastDeleter<dtNavMesh>
    {
        void operator()(dtNavMesh* p) const
        {
            dtFreeNavMesh(p);
        }
    };

    template<>
    struct RecastDeleter<dtNavMeshQuery>
    {
        void operator()(dtNavMeshQuery* p) const
        {
            dtFreeNavMeshQuery(p);
        }
    };

    template<>
    struct RecastDeleter<dtCrowd>
    {
        void operator()(dtCrowd* p) const
        {
            dtFreeCrowd(p);
        }
    };
} // namespace SparkyStudios::AI::Behave::Navigation

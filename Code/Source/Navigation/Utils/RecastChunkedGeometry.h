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

struct rcChunkedGeometryNode
{
    float bmin[2];
    float bmax[2];
    int i;
    int n;
};

struct rcChunkedGeometry
{
    rcChunkedGeometry()
        : nodes(0)
        , nnodes(0)
        , tris(0)
        , ntris(0)
        , maxTrisPerChunk(0)
    {
    }

    ~rcChunkedGeometry()
    {
        delete[] nodes;
        delete[] tris;
    }

    rcChunkedGeometryNode* nodes;
    int nnodes;
    int* tris;
    int ntris;
    int maxTrisPerChunk;

private:
    // Explicitly disabled copy constructor and copy assignment operator.
    rcChunkedGeometry(const rcChunkedGeometry&) = delete;
    rcChunkedGeometry& operator=(const rcChunkedGeometry&) = delete;
};

/// Creates partitioned triangle mesh (AABB tree),
/// where each node contains at max trisPerChunk triangles.
bool rcCreateChunkedGeometry(const float* verts, const int* tris, int ntris, int trisPerChunk, rcChunkedGeometry* cm);

/// Returns the chunk indices which overlap the input rectable.
int rcGetChunksOverlappingRect(const rcChunkedGeometry* cm, float bmin[2], float bmax[2], int* ids, const int maxIds);

/// Returns the chunk indices which overlap the input segment.
int rcGetChunksOverlappingSegment(const rcChunkedGeometry* cm, float p[2], float q[2], int* ids, const int maxIds);

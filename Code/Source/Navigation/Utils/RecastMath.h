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

#include <Navigation/Utils/RecastChunkedGeometry.h>
#include <Navigation/Utils/RecastSmartPointer.h>

#include <AzCore/Math/PolygonPrism.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    /**
     * @brief Convert O3DE coordinate space (+Z up) into Recast coordinate space (+Y up).
     */
    struct RecastVector3
    {
        RecastVector3() = default;

        /**
         * @brief Create a new instance from an O3DE vector.
         *
         * @param in The O3DE vector.
         */
        explicit RecastVector3(const AZ::Vector3& in);

        /**
         * @brief Create a new instance from an array pointer coming from Recast.
         *
         * @param in The pointer containing vector components.
         */
        explicit RecastVector3(const float* in);

        /**
         * @brief Create a new instance from a set of values.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param z The z coordinate.
         */
        RecastVector3(float x, float y, float z);

        /**
         * @brief Returns the pointer to the first vector component.
         */
        float* data();

        /**
         * @brief Returns the pointer to the first vector component.
         */
        [[nodiscard]] const float* data() const;

        /**
         * @brief Returns the vector as an O3DE vector.
         */
        [[nodiscard]] AZ::Vector3 AsVector3() const;

        /**
         * @brief Vectors components.
         */
        float mXYZ[3] = { 0.0f, 0.0f, 0.0f };
    };

    /**
     * @brief Stores geometry data for a single Recast navigation mesh tile.
     */
    struct RecastNavigationMeshGeometry
    {
        /**
         * @brief The coordinate of this tile over the X-axis.
         */
        AZ::s32 mTileX = 0;

        /**
         * @brief The coordinate of this tile over the Y-axis.
         */
        AZ::s32 mTileY = 0;

        /**
         * @brief The geometry's vertex buffer.
         */
        AZStd::vector<RecastVector3> mVertices;

        /**
         * @brief The geometry's index buffer.
         */
        AZStd::vector<AZ::s32> mIndices;

        /**
         * @brief The chunked geometry. Used only if tiling is enabled.
         */
        RecastPointer<rcChunkedGeometry> mChunkedGeometry = nullptr;

        /**
         * @brief Clear the geometry data.
         */
        void Clear();

        /**
         * @brief Checks if this tile is empty (has no vertices).
         *
         * @returns true if the tile has no vertices, false otherwise.
         */
        [[nodiscard]] bool IsEmpty() const;
    };

    /**
     * @brief Stores a polygon prism to send to Recast as a convex volume.
     */
    struct RecastAreaConvexVolume
    {
        /**
         * @brief The convex volume's vertices.
         */
        AZStd::vector<RecastVector3> mVertices;

        /**
         * @brief The convex volume's minimum height.
         *
         * This is the distance from the ground to the bottom of the convex volume.
         */
        float mHMin;

        /**
         * @brief The convex volume's maximum height.
         *
         * This is the distance from the ground to the top of the convex volume.
         */
        float mHMax;

        /**
         * @brief The navigation area's ID associated to this volume.
         */
        AZ::u8 mArea;

        RecastAreaConvexVolume();

        explicit RecastAreaConvexVolume(const AZ::PolygonPrism& prism, const AZ::Transform& transform);
    };
} // namespace SparkyStudios::AI::Behave::Navigation

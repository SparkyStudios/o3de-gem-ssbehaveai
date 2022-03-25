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

#include <AzCore/Math/Vector3.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    /**
     * @brief Convert O3DE coordinate space (+Z up) into Recast coordinate space (+Y up).
     */
    class RecastVector3
    {
    public:
        RecastVector3() = default;

        /**
         * @brief Create a new instance from O3DE vector.
         *
         * @param in The O3DE vector.
         */
        explicit RecastVector3(const AZ::Vector3& in)
        {
            m_x = in.GetX();
            m_y = in.GetZ();
            m_z = in.GetY();
        }

        /**
         * @brief Create a new instance from an array pointer.
         *
         * @param in The pointer containing vector components.
         * Mostly coming from recast itself.
         */
        explicit RecastVector3(const float* in)
        {
            m_x = in[0];
            m_y = in[1];
            m_z = in[2];
        }

        /**
         * @brief Create a new instance from a set of values.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param z The z coordinate.
         */
        RecastVector3(float x, float y, float z)
        {
            m_x = x;
            m_y = y;
            m_z = z;
        }

        /**
         * @brief Returns the pointer to the first vector component.
         */
        float* data()
        {
            return &m_x;
        }

        /**
         * @brief Returns the pointer to the first vector component.
         */
        const float* data() const
        {
            return &m_x;
        }

        /**
         * @brief Returns the vector as an O3DE vector.
         */
        AZ::Vector3 AsVector3() const
        {
            return AZ::Vector3(m_x, m_z, m_y);
        }

        float m_x = 0, m_y = 0, m_z = 0;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

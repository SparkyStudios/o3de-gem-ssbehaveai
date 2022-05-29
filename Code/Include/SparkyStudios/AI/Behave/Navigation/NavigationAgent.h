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

#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class NavigationAgent final
    {
    public:
        typedef AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>> List;

        AZ_TYPE_INFO(NavigationAgent, "{DE530E60-899E-4FB4-B030-8F77DB4E44CF}");
        AZ_CLASS_ALLOCATOR(NavigationAgent, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        NavigationAgent();

        explicit NavigationAgent(AZStd::string name, float height, float maxClimb, float radius, float maxSlopAngle);

        bool operator==(const NavigationAgent& rhs) const;

        bool operator!=(const NavigationAgent& rhs) const;

        bool operator<(const NavigationAgent& rhs) const;

        void SetName(const AZStd::string& name);

        void SetHeight(float height);

        void SetMaxClimb(float maxClimb);

        void SetRadius(float radius);

        void SetMaxSlopeAngle(float maxSlopeAngle);

        [[nodiscard]] AZ::u32 GetId() const;

        [[nodiscard]] const AZStd::string& GetName() const;

        [[nodiscard]] float GetHeight() const;

        [[nodiscard]] float GetMaxClimb() const;

        [[nodiscard]] float GetRadius() const;

        [[nodiscard]] float GetMaxSlopeAngle() const;

    private:
        AZ::Crc32 OnNameChanged();

        AZ::Crc32 _id;

        /**
         * @brief Navigation agent name.
         */
        AZStd::string _name;

        /**
         * @brief Minimum floor to 'ceiling' height that will still allow
         * the floor area to be considered walkable.
         */
        float _height = 2.0f;

        /**
         * @brief Maximum ledge height that is considered to still be traversable.
         */
        float _climb = 0.9f;

        /**
         * @brief The distance to erode/shrink the walkable area of the
         * heightfield away from obstructions.
         */
        float _radius = 0.6f;

        /**
         * @brief The maximum slope that is considered walkable.
         */
        float _slopAngle = 45.0f;
    };

    typedef AZStd::unordered_set<AZStd::string> NavigationAgentNameSet;
    typedef AZStd::vector<NavigationAgent> NavigationAgentList;

    AZ_INLINE bool NavigationAgent::operator==(const NavigationAgent& rhs) const
    {
        return _id == rhs._id;
    }

    AZ_INLINE bool NavigationAgent::operator!=(const NavigationAgent& rhs) const
    {
        return !(*this == rhs);
    }

    AZ_INLINE bool NavigationAgent::operator<(const NavigationAgent& rhs) const
    {
        return rhs._id < _id;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

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
    class BehaveNavigationMeshArea final
    {
    public:
        using List = AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>>;

        AZ_TYPE_INFO(BehaveNavigationMeshArea, "{06E238C1-E763-4344-B764-54F8EFBACDB3}");
        AZ_CLASS_ALLOCATOR(BehaveNavigationMeshArea, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief Returns the default navigation mesh area.
         */
        static const BehaveNavigationMeshArea& Default();

        /**
         * @brief Gets the list of registered navigation mesh areas.
         *
         * @return A vector of key-pair values, with the navigation mesh area Crc32 representation
         * as the key, and the navigation mesh area name as the value.
         */
        static List GetRegisteredNavigationAgents();

        /**
         * @brief Creates a new navigation mesh area.
         */
        BehaveNavigationMeshArea();

        /**
         * @brief Creates a new navigation mesh area.
         *
         * @param name The name of the navigation mesh area.
         * @param cost The cost of the navigation mesh area.
         */
        explicit BehaveNavigationMeshArea(const AZStd::string& name, float cost = 1.0f);

        /**
         * @brief Creates a new navigation mesh area.
         *
         * @param value The Crc32 representation of the navigation mesh area.
         * @param cost The cost of the navigation mesh area.
         */
        explicit BehaveNavigationMeshArea(const AZ::Crc32& value, float cost = 1.0f);

        AZ_INLINE bool operator==(const BehaveNavigationMeshArea& other) const;

        AZ_INLINE bool operator<(const BehaveNavigationMeshArea& other) const;

        AZ_INLINE explicit operator AZ::Crc32() const;

        AZ_INLINE explicit operator AZ::u32() const;

        void SetName(const AZStd::string& name);

        void SetCost(float cost);

        [[nodiscard]] AZStd::string GetDisplayName() const;

        [[nodiscard]] float GetCost() const;

    private:
        bool FindDisplayName(const AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>>& selectableAreas, AZStd::string& name) const;

        [[nodiscard]] List BuildSelectableNavigationMeshAreaList() const;

        AZ::u32 _areaCrc;
        float _cost;
    };

    using BehaveNavigationMeshAreaNameSet = AZStd::unordered_set<AZStd::string>;
    using BehaveNavigationMeshAreaVector = AZStd::vector<BehaveNavigationMeshArea>;

    AZ_INLINE bool BehaveNavigationMeshArea::operator==(const BehaveNavigationMeshArea& other) const
    {
        return other._areaCrc == _areaCrc;
    }

    AZ_INLINE bool BehaveNavigationMeshArea::operator<(const BehaveNavigationMeshArea& other) const
    {
        return other._areaCrc < _areaCrc;
    }

    AZ_INLINE BehaveNavigationMeshArea::operator AZ::Crc32() const
    {
        return _areaCrc;
    }

    AZ_INLINE BehaveNavigationMeshArea::operator AZ::u32() const
    {
        return static_cast<AZ::u32>(_areaCrc);
    }
} // namespace SparkyStudios::AI::Behave::Navigation

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
    typedef AZ::u8 NavigationAreaId;

    enum NavigationAreaFlag : AZ::u16
    {
        eNAF_NONE = 0x00,
        eNAF_WALK = 0x01,
        eNAF_SWIM = 0x02,
        eNAF_JUMP = 0x04,
        eNAF_FLY = 0x08,
        eNAF_DOOR = 0x10,
        eNAF_DISABLED = 0x20,
        eNAF_ALL = 0xFFFF,
    };

    /**
     * @brief Defines a navigation area on the navigation mesh.
     *
     * A navigation area have a flag, specifying the type of movement that can be done on it, and therefore
     * which agent can move across it. The agents allowed to move on this area are affected by the navigation
     * cost of the area.
     */
    class NavigationArea final
    {
    public:
        AZ_TYPE_INFO(NavigationArea, "{06E238C1-E763-4344-B764-54F8EFBACDB3}");
        AZ_CLASS_ALLOCATOR(NavigationArea, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief Returns the default navigation mesh area.
         */
        static const NavigationArea& Default();

        /**
         * @brief Creates a new navigation mesh area.
         */
        NavigationArea();

        /**
         * @brief Creates a new navigation mesh area.
         *
         * @param id The area ID.
         * @param name The name of the navigation mesh area.
         * @param cost The cost of the navigation mesh area.
         * @param flags The flags of the navigation mesh area.
         */
        explicit NavigationArea(NavigationAreaId id, AZStd::string name, float cost = 1.0f, AZ::u16 flags = eNAF_ALL);

        bool operator==(const NavigationArea& rhs) const;

        bool operator!=(const NavigationArea& rhs) const;

        bool operator<(const NavigationArea& rhs) const;

        explicit operator NavigationAreaId() const;

        void SetId(NavigationAreaId value);

        void SetName(const AZStd::string& name);

        void SetCost(float cost);

        void SetFlags(AZ::u16 flags);

        [[nodiscard]] NavigationAreaId GetId() const;

        [[nodiscard]] const AZStd::string& GetName() const;

        [[nodiscard]] float GetCost() const;

        [[nodiscard]] AZ::u16 GetFlags() const;

    private:
        [[nodiscard]] bool CheckWalkFlag() const;
        [[nodiscard]] bool CheckSwimFlag() const;
        [[nodiscard]] bool CheckJumpFlag() const;
        [[nodiscard]] bool CheckFlyFlag() const;
        [[nodiscard]] bool CheckDoorFlag() const;
        [[nodiscard]] bool CheckDisabledFlag() const;

        AZ::Crc32 OnWalkFlagChanged();
        AZ::Crc32 OnSwimFlagChanged();
        AZ::Crc32 OnJumpFlagChanged();
        AZ::Crc32 OnFlyFlagChanged();
        AZ::Crc32 OnDoorFlagChanged();
        AZ::Crc32 OnDisabledFlagChanged();

        NavigationAreaId _id;
        AZStd::string _name;
        float _cost;
        AZ::u16 _flags;
    };

    using BehaveNavigationMeshAreaNameSet = AZStd::unordered_set<AZStd::string>;
    using BehaveNavigationMeshAreaVector = AZStd::vector<NavigationArea>;

    AZ_INLINE bool NavigationArea::operator==(const NavigationArea& rhs) const
    {
        return rhs._name == _name && rhs._cost == _cost && rhs._flags == _flags;
    }

    AZ_INLINE bool NavigationArea::operator!=(const NavigationArea& rhs) const
    {
        return !(*this == rhs);
    }

    AZ_INLINE bool NavigationArea::operator<(const NavigationArea& rhs) const
    {
        return rhs._id < _id;
    }

    AZ_INLINE NavigationArea::operator NavigationAreaId() const
    {
        return _id;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(SparkyStudios::AI::Behave::Navigation::NavigationAreaFlag, "{D220378C-9F2B-4756-AF88-6E4B4ED876E0}");
} // namespace AZ

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

#include "Utils/Constants.h"

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshArea.h>

#include <AzCore/Debug/Profiler.h>
#include <AzCore/Serialization/EditContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    static BehaveNavigationMeshArea gDefaultNavigationAgent = BehaveNavigationMeshArea(
        kDefaultNavigationMeshAreaId, kDefaultNavigationMeshAreaName, kDefaultNavigationMeshAreaCost, eNMAF_ALL ^ eNMAF_DISABLED);

    void BehaveNavigationMeshArea::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Enum<NavigationMeshAreaFlag>()
                ->Version(0)
                ->Value("Walk", NavigationMeshAreaFlag::eNMAF_WALK)
                ->Value("Swim", NavigationMeshAreaFlag::eNMAF_SWIM)
                ->Value("Jump", NavigationMeshAreaFlag::eNMAF_JUMP)
                ->Value("Fly", NavigationMeshAreaFlag::eNMAF_FLY)
                ->Value("Door", NavigationMeshAreaFlag::eNMAF_DOOR)
                ->Value("Disabled", NavigationMeshAreaFlag::eNMAF_DISABLED)
                ->Value("All", NavigationMeshAreaFlag::eNMAF_ALL);

            sc->Class<BehaveNavigationMeshArea>()
                ->Version(0)
                ->Field("Id", &BehaveNavigationMeshArea::_id)
                ->Field("Name", &BehaveNavigationMeshArea::_name)
                ->Field("Cost", &BehaveNavigationMeshArea::_cost)
                ->Field("Flags", &BehaveNavigationMeshArea::_flags);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Enum<NavigationMeshAreaFlag>("Navigation Mesh Area Flags", "Flags for the navigation mesh area")
                    ->Value("Walk", NavigationMeshAreaFlag::eNMAF_WALK)
                    ->Value("Swim", NavigationMeshAreaFlag::eNMAF_SWIM)
                    ->Value("Jump", NavigationMeshAreaFlag::eNMAF_JUMP)
                    ->Value("Fly", NavigationMeshAreaFlag::eNMAF_FLY)
                    ->Value("Door", NavigationMeshAreaFlag::eNMAF_DOOR)
                    ->Value("Disabled", NavigationMeshAreaFlag::eNMAF_DISABLED)
                    ->Value("All", NavigationMeshAreaFlag::eNMAF_ALL);

                ec->Class<BehaveNavigationMeshArea>("Behave AI - Navigation Mesh Area", "Configures a navigation mesh area.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshArea::_name, "Name", "The name of the navigation mesh area.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshArea::_cost, "Cost",
                        "The traversal cost of the navigation mesh area.")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Flags")
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Walk", "Mark the area as accessible by agents with the ability to walk.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnWalkFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckWalkFlag)
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &BehaveNavigationMeshArea::CheckDisabledFlag)
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Swim", "Mark the area as accessible by agents with the ability to swim.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnSwimFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckSwimFlag)
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &BehaveNavigationMeshArea::CheckDisabledFlag)
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Jump", "Mark the area as accessible by agents with the ability to jump.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnJumpFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckJumpFlag)
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &BehaveNavigationMeshArea::CheckDisabledFlag)
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Fly", "Mark the area as accessible by agents with the ability to fly.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnFlyFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckFlyFlag)
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &BehaveNavigationMeshArea::CheckDisabledFlag)
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Door", "Mark the area as accessible by agents with the ability to door.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnDoorFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckDoorFlag)
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &BehaveNavigationMeshArea::CheckDisabledFlag)
                    ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Disabled", "Disable the navigation on this area for all agents.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshArea::OnDisabledFlagChanged)
                    ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &BehaveNavigationMeshArea::CheckDisabledFlag);
            }
        }
    }

    const BehaveNavigationMeshArea& BehaveNavigationMeshArea::Default()
    {
        return gDefaultNavigationAgent;
    }

    BehaveNavigationMeshArea::BehaveNavigationMeshArea()
        : _id(kDefaultNavigationMeshAreaId)
        , _name() // Set name to empty string
        , _cost(kDefaultNavigationMeshAreaCost)
        , _flags(0)
    {
    }

    BehaveNavigationMeshArea::BehaveNavigationMeshArea(AZ::u8 id, AZStd::string name, float cost, AZ::u16 flags)
        : _id(id)
        , _name(AZStd::move(name))
        , _cost(cost)
        , _flags(flags)
    {
    }

    void BehaveNavigationMeshArea::SetId(AZ::u8 value)
    {
        _id = value;
    }

    void BehaveNavigationMeshArea::SetName(const AZStd::string& name)
    {
        _name = name;
    }

    void BehaveNavigationMeshArea::SetCost(float cost)
    {
        _cost = cost;
    }

    void BehaveNavigationMeshArea::SetFlags(AZ::u16 flags)
    {
        _flags = flags;
    }

    AZ::u8 BehaveNavigationMeshArea::GetId() const
    {
        return _id;
    }

    const AZStd::string& BehaveNavigationMeshArea::GetName() const
    {
        return _name;
    }

    float BehaveNavigationMeshArea::GetCost() const
    {
        return _cost;
    }

    AZ::u16 BehaveNavigationMeshArea::GetFlags() const
    {
        return _flags;
    }

    bool BehaveNavigationMeshArea::CheckWalkFlag() const
    {
        return (_flags & eNMAF_WALK) == eNMAF_WALK;
    }

    bool BehaveNavigationMeshArea::CheckSwimFlag() const
    {
        return (_flags & eNMAF_SWIM) == eNMAF_SWIM;
    }

    bool BehaveNavigationMeshArea::CheckJumpFlag() const
    {
        return (_flags & eNMAF_JUMP) == eNMAF_JUMP;
    }

    bool BehaveNavigationMeshArea::CheckFlyFlag() const
    {
        return (_flags & eNMAF_FLY) == eNMAF_FLY;
    }

    bool BehaveNavigationMeshArea::CheckDoorFlag() const
    {
        return (_flags & eNMAF_DOOR) == eNMAF_DOOR;
    }

    bool BehaveNavigationMeshArea::CheckDisabledFlag() const
    {
        return (_flags & eNMAF_DISABLED) == eNMAF_DISABLED;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnWalkFlagChanged()
    {
        if (CheckWalkFlag())
        {
            _flags ^= eNMAF_WALK;
        }
        else
        {
            _flags |= eNMAF_WALK;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnSwimFlagChanged()
    {
        if (CheckSwimFlag())
        {
            _flags ^= eNMAF_SWIM;
        }
        else
        {
            _flags |= eNMAF_SWIM;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnJumpFlagChanged()
    {
        if (CheckJumpFlag())
        {
            _flags ^= eNMAF_JUMP;
        }
        else
        {
            _flags |= eNMAF_JUMP;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnFlyFlagChanged()
    {
        if (CheckFlyFlag())
        {
            _flags ^= eNMAF_FLY;
        }
        else
        {
            _flags |= eNMAF_FLY;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnDoorFlagChanged()
    {
        if (CheckDoorFlag())
        {
            _flags ^= eNMAF_DOOR;
        }
        else
        {
            _flags |= eNMAF_DOOR;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 BehaveNavigationMeshArea::OnDisabledFlagChanged()
    {
        if (CheckDisabledFlag())
        {
            _flags ^= eNMAF_DISABLED;
        }
        else
        {
            _flags |= eNMAF_DISABLED;
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

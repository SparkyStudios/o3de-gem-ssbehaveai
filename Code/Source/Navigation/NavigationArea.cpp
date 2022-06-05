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

#include <SparkyStudios/AI/Behave/Navigation/NavigationArea.h>

#include <Navigation/Utils/Constants.h>

#include <AzCore/Debug/Profiler.h>
#include <AzCore/Serialization/EditContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    static NavigationArea gDefaultNavigationAgent = NavigationArea(
        kDefaultNavigationMeshAreaId, kDefaultNavigationMeshAreaName, kDefaultNavigationMeshAreaCost, eNAF_ALL ^ eNAF_DISABLED);

    void NavigationArea::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // This class can already been reflected, so check first
            if (sc->FindClassData(azrtti_typeid<NavigationArea>()) == nullptr)
            {
                sc->Enum<NavigationAreaFlag>()
                    ->Version(0)
                    ->Value("Walk", NavigationAreaFlag::eNAF_WALK)
                    ->Value("Swim", NavigationAreaFlag::eNAF_SWIM)
                    ->Value("Jump", NavigationAreaFlag::eNAF_JUMP)
                    ->Value("Fly", NavigationAreaFlag::eNAF_FLY)
                    ->Value("Door", NavigationAreaFlag::eNAF_DOOR)
                    ->Value("Disabled", NavigationAreaFlag::eNAF_DISABLED)
                    ->Value("All", NavigationAreaFlag::eNAF_ALL);

                sc->Class<NavigationArea>()
                    ->Version(0)
                    ->Field("Id", &NavigationArea::_id)
                    ->Field("Name", &NavigationArea::_name)
                    ->Field("Cost", &NavigationArea::_cost)
                    ->Field("Flags", &NavigationArea::_flags);

                if (AZ::EditContext* ec = sc->GetEditContext())
                {
                    ec->Enum<NavigationAreaFlag>("Navigation Mesh Area Flags", "Flags for the navigation mesh area")
                        ->Value("Walk", NavigationAreaFlag::eNAF_WALK)
                        ->Value("Swim", NavigationAreaFlag::eNAF_SWIM)
                        ->Value("Jump", NavigationAreaFlag::eNAF_JUMP)
                        ->Value("Fly", NavigationAreaFlag::eNAF_FLY)
                        ->Value("Door", NavigationAreaFlag::eNAF_DOOR)
                        ->Value("Disabled", NavigationAreaFlag::eNAF_DISABLED)
                        ->Value("All", NavigationAreaFlag::eNAF_ALL);

                    ec->Class<NavigationArea>("Behave AI - Navigation Mesh Area", "Configures a navigation mesh area.")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationArea::_name, "Name",
                            "The name of the navigation mesh area.")
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationArea::_cost, "Cost",
                            "The traversal cost of the navigation mesh area.")

                        ->ClassElement(AZ::Edit::ClassElements::Group, "Flags")
                        ->UIElement(
                            AZ::Edit::UIHandlers::CheckBox, "Walk", "Mark the area as accessible by agents with the ability to walk.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnWalkFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckWalkFlag)
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, &NavigationArea::CheckDisabledFlag)
                        ->UIElement(
                            AZ::Edit::UIHandlers::CheckBox, "Swim", "Mark the area as accessible by agents with the ability to swim.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnSwimFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckSwimFlag)
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, &NavigationArea::CheckDisabledFlag)
                        ->UIElement(
                            AZ::Edit::UIHandlers::CheckBox, "Jump", "Mark the area as accessible by agents with the ability to jump.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnJumpFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckJumpFlag)
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, &NavigationArea::CheckDisabledFlag)
                        ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Fly", "Mark the area as accessible by agents with the ability to fly.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnFlyFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckFlyFlag)
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, &NavigationArea::CheckDisabledFlag)
                        ->UIElement(
                            AZ::Edit::UIHandlers::CheckBox, "Door", "Mark the area as accessible by agents with the ability to door.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnDoorFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckDoorFlag)
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, &NavigationArea::CheckDisabledFlag)
                        ->UIElement(AZ::Edit::UIHandlers::CheckBox, "Disabled", "Disable the navigation on this area for all agents.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationArea::OnDisabledFlagChanged)
                        ->Attribute(AZ::Edit::Attributes::CheckboxDefaultValue, &NavigationArea::CheckDisabledFlag);
                }
            }
        }
    }

    const NavigationArea& NavigationArea::Default()
    {
        return gDefaultNavigationAgent;
    }

    NavigationArea::NavigationArea()
        : _id(kDefaultNavigationMeshAreaId)
        , _name() // Set name to empty string
        , _cost(kDefaultNavigationMeshAreaCost)
        , _flags(0)
    {
    }

    NavigationArea::NavigationArea(NavigationAreaId id, AZStd::string name, float cost, AZ::u16 flags)
        : _id(id)
        , _name(AZStd::move(name))
        , _cost(cost)
        , _flags(flags)
    {
    }

    void NavigationArea::SetId(NavigationAreaId value)
    {
        _id = value;
    }

    void NavigationArea::SetName(const AZStd::string& name)
    {
        _name = name;
    }

    void NavigationArea::SetCost(float cost)
    {
        _cost = cost;
    }

    void NavigationArea::SetFlags(AZ::u16 flags)
    {
        _flags = flags;
    }

    NavigationAreaId NavigationArea::GetId() const
    {
        return _id;
    }

    const AZStd::string& NavigationArea::GetName() const
    {
        return _name;
    }

    float NavigationArea::GetCost() const
    {
        return _cost;
    }

    AZ::u16 NavigationArea::GetFlags() const
    {
        return _flags;
    }

    bool NavigationArea::CheckWalkFlag() const
    {
        return (_flags & eNAF_WALK) == eNAF_WALK;
    }

    bool NavigationArea::CheckSwimFlag() const
    {
        return (_flags & eNAF_SWIM) == eNAF_SWIM;
    }

    bool NavigationArea::CheckJumpFlag() const
    {
        return (_flags & eNAF_JUMP) == eNAF_JUMP;
    }

    bool NavigationArea::CheckFlyFlag() const
    {
        return (_flags & eNAF_FLY) == eNAF_FLY;
    }

    bool NavigationArea::CheckDoorFlag() const
    {
        return (_flags & eNAF_DOOR) == eNAF_DOOR;
    }

    bool NavigationArea::CheckDisabledFlag() const
    {
        return (_flags & eNAF_DISABLED) == eNAF_DISABLED;
    }

    AZ::Crc32 NavigationArea::OnWalkFlagChanged()
    {
        if (CheckWalkFlag())
        {
            _flags ^= eNAF_WALK;
        }
        else
        {
            _flags |= eNAF_WALK;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 NavigationArea::OnSwimFlagChanged()
    {
        if (CheckSwimFlag())
        {
            _flags ^= eNAF_SWIM;
        }
        else
        {
            _flags |= eNAF_SWIM;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 NavigationArea::OnJumpFlagChanged()
    {
        if (CheckJumpFlag())
        {
            _flags ^= eNAF_JUMP;
        }
        else
        {
            _flags |= eNAF_JUMP;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 NavigationArea::OnFlyFlagChanged()
    {
        if (CheckFlyFlag())
        {
            _flags ^= eNAF_FLY;
        }
        else
        {
            _flags |= eNAF_FLY;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 NavigationArea::OnDoorFlagChanged()
    {
        if (CheckDoorFlag())
        {
            _flags ^= eNAF_DOOR;
        }
        else
        {
            _flags |= eNAF_DOOR;
        }

        return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
    }

    AZ::Crc32 NavigationArea::OnDisabledFlagChanged()
    {
        if (CheckDisabledFlag())
        {
            _flags ^= eNAF_DISABLED;
        }
        else
        {
            _flags |= eNAF_DISABLED;
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

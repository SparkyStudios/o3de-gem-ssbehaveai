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

#include <SparkyStudios/AI/Behave/Navigation/NavigationAgent.h>

#include <AzCore/Serialization/EditContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationAgent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // This class can already been reflected, so check first
            if (sc->FindClassData(azrtti_typeid<NavigationAgent>()) == nullptr)
            {
                sc->Class<NavigationAgent>()
                    ->Version(0)
                    ->Field("Id", &NavigationAgent::_id)
                    ->Field("Name", &NavigationAgent::_name)
                    ->Field("MaxClimb", &NavigationAgent::_climb)
                    ->Field("Height", &NavigationAgent::_height)
                    ->Field("Radius", &NavigationAgent::_radius)
                    ->Field("MaxSlopAngle", &NavigationAgent::_slopAngle);

                if (AZ::EditContext* ec = sc->GetEditContext())
                {
                    ec->Class<NavigationAgent>("Behave AI - Navigation Agent", "A navigation agent.")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)

                        ->DataElement(AZ::Edit::UIHandlers::Default, &NavigationAgent::_name, "Name", "The navigation agent name.")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &NavigationAgent::OnNameChanged)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationAgent::_height, "Height",
                            "Minimum floor to 'ceiling' height that will still allow the floor area to be considered walkable.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.1)
                        ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationAgent::_climb, "Max Climb",
                            "Maximum ledge height that is considered to still be traversable.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationAgent::_radius, "Radius",
                            "The distance to erode/shrink the walkable area of the heightfield away from obstructions.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0)
                        ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &NavigationAgent::_slopAngle, "Max Slop",
                            "The maximum slope that is considered walkable.")
                        ->Attribute(AZ::Edit::Attributes::Suffix, " degrees")
                        ->Attribute(AZ::Edit::Attributes::Min, 0)
                        ->Attribute(AZ::Edit::Attributes::Max, 90);
                }
            }
        }
    }

    NavigationAgent::NavigationAgent()
        : NavigationAgent("", 0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    NavigationAgent::NavigationAgent(AZStd::string name, float height, float maxClimb, float radius, float maxSlopAngle)
        : _name(AZStd::move(name))
        , _height(height)
        , _climb(maxClimb)
        , _radius(radius)
        , _slopAngle(maxSlopAngle)
    {
        _id = AZ::Crc32(_name.c_str());
    }

    void NavigationAgent::SetName(const AZStd::string& name)
    {
        _id = AZ::Crc32(name.c_str());
        _name = name;
    }

    void NavigationAgent::SetHeight(float height)
    {
        _height = height;
    }

    void NavigationAgent::SetMaxClimb(float maxClimb)
    {
        _climb = maxClimb;
    }

    void NavigationAgent::SetRadius(float radius)
    {
        _radius = radius;
    }

    void NavigationAgent::SetMaxSlopeAngle(float maxSlopeAngle)
    {
        _slopAngle = maxSlopeAngle;
    }

    AZ::u32 NavigationAgent::GetId() const
    {
        return _id;
    }

    const AZStd::string& NavigationAgent::GetName() const
    {
        return _name;
    }

    float NavigationAgent::GetHeight() const
    {
        return _height;
    }

    float NavigationAgent::GetMaxClimb() const
    {
        return _climb;
    }

    float NavigationAgent::GetRadius() const
    {
        return _radius;
    }

    float NavigationAgent::GetMaxSlopeAngle() const
    {
        return _slopAngle;
    }

    AZ::Crc32 NavigationAgent::OnNameChanged()
    {
        _id = AZ::Crc32(_name.c_str());
        return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

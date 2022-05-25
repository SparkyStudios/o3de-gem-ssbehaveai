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

#include <Source/Navigation/BehaveNavigationMeshAreaProviderRequestBus.h>
#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshArea.h>

#include <AzCore/Debug/Profiler.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/std/sort.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    static BehaveNavigationMeshArea gDefaultNavigationAgent = BehaveNavigationMeshArea();

    void BehaveNavigationMeshArea::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<BehaveNavigationMeshArea>()->Version(0)->Field("AreaCRC", &BehaveNavigationMeshArea::_areaCrc);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<BehaveNavigationMeshArea>("Behave AI - Navigation Mesh Area", "Configures a navigation mesh area.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &BehaveNavigationMeshArea::_areaCrc, "Area", "The navigation mesh area name.")
                    ->Attribute(AZ::Edit::Attributes::EnumValues, &BehaveNavigationMeshArea::BuildSelectableNavigationMeshAreaList)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }

    const BehaveNavigationMeshArea& BehaveNavigationMeshArea::Default()
    {
        return gDefaultNavigationAgent;
    }

    BehaveNavigationMeshArea::List BehaveNavigationMeshArea::GetRegisteredNavigationAgents()
    {
        AZ_PROFILE_FUNCTION(Entity);

        BehaveNavigationMeshAreaNameSet labels;
        BehaveNavigationMeshAreaProviderRequestBus::Broadcast(
            &BehaveNavigationMeshAreaProviderRequestBus::Events::GetRegisteredNavigationMeshAreaNames, labels);

        AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>> registeredAgents;
        registeredAgents.reserve(labels.size());
        for (auto&& label : labels)
        {
            const AZ::u32 crc = AZ::Crc32(label.data());

            // warn when two areas have the same crc
            if (const auto entryIt = AZStd::find_if(
                    registeredAgents.begin(), registeredAgents.end(),
                    [crc](const auto& entry)
                    {
                        return entry.first == crc;
                    });
                entryIt != registeredAgents.end())
            {
                AZ_Warning(
                    "[BehaveAI] Navigation", false, "Navigation Mesh Area CRC collision between \"%s\" and \"%s\"!  \"%s\" not added.",
                    entryIt->second.data(), label.data(), label.data());
                continue;
            }

            registeredAgents.push_back({ crc, label });
        }

        return registeredAgents;
    }

    BehaveNavigationMeshArea::BehaveNavigationMeshArea()
        : BehaveNavigationMeshArea(kDefaultNavigationMeshAreaName)
    {
    }

    BehaveNavigationMeshArea::BehaveNavigationMeshArea(const AZStd::string& name, float cost)
        : _areaCrc(AZ::Crc32(name.data()))
        , _cost(cost)
    {
    }

    BehaveNavigationMeshArea::BehaveNavigationMeshArea(const AZ::Crc32& value, float cost)
        : _areaCrc(value)
        , _cost(cost)
    {
    }

    void BehaveNavigationMeshArea::SetName(const AZStd::string& name)
    {
        _areaCrc = AZ::Crc32(name.data());
    }

    void BehaveNavigationMeshArea::SetCost(float cost)
    {
        _cost = cost;
    }

    AZStd::string BehaveNavigationMeshArea::GetDisplayName() const
    {
        AZ_PROFILE_FUNCTION(Entity);

        AZStd::string name;
        FindDisplayName(GetRegisteredNavigationAgents(), name);

        return name;
    }

    float BehaveNavigationMeshArea::GetCost() const
    {
        return _cost;
    }

    bool BehaveNavigationMeshArea::FindDisplayName(
        const AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>>& selectableAreas, AZStd::string& name) const
    {
        const auto it = AZStd::find_if(
            selectableAreas.begin(), selectableAreas.end(),
            [this](const auto& entry)
            {
                return _areaCrc == entry.first;
            });

        if (it == selectableAreas.end())
        {
            // if a match was not found, generate a name using the crc
            name = AZStd::string::format("(unregistered %u)", _areaCrc);
            return false;
        }

        name = it->second;
        return true;
    }

    BehaveNavigationMeshArea::List BehaveNavigationMeshArea::BuildSelectableNavigationMeshAreaList() const
    {
        AZ_PROFILE_FUNCTION(Entity);

        AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>> selectableAreas = GetRegisteredNavigationAgents();

        if (AZStd::string name; !FindDisplayName(selectableAreas, name))
        {
            // if a match was not found, add the generated name to the selectable set
            selectableAreas.push_back({ _areaCrc, name });
            AZ_Warning("[BehaveAI] Navigation", false, "Navigation Mesh Area CRC %u is not a registered area.", _areaCrc);
        }

        AZStd::sort(
            selectableAreas.begin(), selectableAreas.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.second < rhs.second;
            });

        return selectableAreas;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

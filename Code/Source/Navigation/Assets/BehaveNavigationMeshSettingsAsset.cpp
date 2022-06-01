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

#include <Navigation/Assets/BehaveNavigationMeshSettingsAsset.h>
#include <Navigation/NavigationAgentProviderRequestBus.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/sort.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void BehaveNavigationMeshSettingsAsset::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<BehaveNavigationMeshSettingsAsset>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::EnableForAssetEditor, true)
                ->Field("Name", &BehaveNavigationMeshSettingsAsset::m_name)
                ->Field("AgentId", &BehaveNavigationMeshSettingsAsset::_agentId)
                ->Field("Agent", &BehaveNavigationMeshSettingsAsset::m_agent)
                ->Field("CellSize", &BehaveNavigationMeshSettingsAsset::m_cellSize)
                ->Field("CellHeight", &BehaveNavigationMeshSettingsAsset::m_cellHeight)
                ->Field("RegionMinSize", &BehaveNavigationMeshSettingsAsset::m_regionMinSize)
                ->Field("RegionMergedSize", &BehaveNavigationMeshSettingsAsset::m_regionMergedSize)
                ->Field("PartitionType", &BehaveNavigationMeshSettingsAsset::m_partitionType)
                ->Field("FilterLowHangingObstacles", &BehaveNavigationMeshSettingsAsset::m_filterLowHangingObstacles)
                ->Field("FilterLedgeSpans", &BehaveNavigationMeshSettingsAsset::m_filterLedgeSpans)
                ->Field("FilterWalkableLowHeightSpans", &BehaveNavigationMeshSettingsAsset::m_filterWalkableLowHeightSpans)
                ->Field("EdgeMaxError", &BehaveNavigationMeshSettingsAsset::m_edgeMaxError)
                ->Field("EdgeMaxLength", &BehaveNavigationMeshSettingsAsset::m_edgeMaxLength)
                ->Field("MaxVerticesPerPolygon", &BehaveNavigationMeshSettingsAsset::m_maxVerticesPerPoly)
                ->Field("DetailSampleDistance", &BehaveNavigationMeshSettingsAsset::m_detailSampleDist)
                ->Field("MaxSampleError", &BehaveNavigationMeshSettingsAsset::m_detailSampleMaxError)
                ->Field("EnableTiling", &BehaveNavigationMeshSettingsAsset::m_enableTiling)
                ->Field("TileSize", &BehaveNavigationMeshSettingsAsset::m_tileSize)
                ->Field("BorderPadding", &BehaveNavigationMeshSettingsAsset::m_borderPadding);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<BehaveNavigationMeshSettingsAsset>(
                      "Navigation Mesh Settings", "Settings to use when building the navigation mesh.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)

                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_name, "Name",
                        "The name of the navigation mesh.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &BehaveNavigationMeshSettingsAsset::_agentId, "Agent",
                        "The navigation agent for this settings.")
                    ->Attribute(AZ::Edit::Attributes::EnumValues, &BehaveNavigationMeshSettingsAsset::BuildSelectableNavigationAgentList)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshSettingsAsset::OnNavigationAgentChanged)

                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Clears the selected agent.")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Clear")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaveNavigationMeshSettingsAsset::OnClearAgent)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Rasterization")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_cellSize, "Cell Size",
                        "The xy-plane cell size to use for fields.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.1f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_cellHeight, "Cell Height",
                        "The z-axis cell size to use for fields.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.1f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Region")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_regionMinSize, "Min Region Size",
                        "The minimum number of cells allowed to form isolated island areas.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_regionMergedSize, "Merged Region Height",
                        "Any regions with a span count smaller than this value will, if possible, be merged with larger regions.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Partitioning")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &BehaveNavigationMeshSettingsAsset::m_partitionType, "Partition Type",
                        "Define how the regions should be partitioned.")
                    ->EnumAttribute(NavigationMeshPartitionType::Watershed, "Watershed")
                    ->EnumAttribute(NavigationMeshPartitionType::Monotone, "Monotone")
                    ->EnumAttribute(NavigationMeshPartitionType::Layers, "Layers")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Filtering")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &BehaveNavigationMeshSettingsAsset::m_filterLowHangingObstacles,
                        "Low Hanging Obstacles",
                        "Allows the formation of walkable regions that will flow over low lying objects such as curbs, and up structures "
                        "such as stairways.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &BehaveNavigationMeshSettingsAsset::m_filterLedgeSpans, "Ledge Spans",
                        "A ledge is a span with one or more neighbors whose maximum is further away than the walkable climb value from the "
                        "current span's maximum. This filter removes the impact of the overestimation of conservative voxelization so the "
                        "resulting mesh will not have regions hanging in the air over ledges.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &BehaveNavigationMeshSettingsAsset::m_filterWalkableLowHeightSpans,
                        "Walkable Low Height Spans",
                        "For this filter, the clearance above the span is the distance from the span's maximum to the next higher span's "
                        "minimum. (Same grid column.)")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Polygonization")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_edgeMaxLength, "Max Edge Length",
                        "The maximum allowed length for contour edges along the border of the mesh.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_edgeMaxError, "Max Edge Error",
                        "The maximum distance a simplified contour's border edges should deviate the original raw contour.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationMeshSettingsAsset::m_maxVerticesPerPoly, "Mar Vertices per Polygon",
                        "The maximum number of vertices allowed for polygons generated during the contour to polygon conversion process.")
                    ->Attribute(AZ::Edit::Attributes::Min, 3)
                    ->Attribute(AZ::Edit::Attributes::Max, 12)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Detail Mesh")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_detailSampleDist, "Detail Sample Distance",
                        "Sets the sampling distance to use when generating the detail mesh.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 16)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_detailSampleMaxError, "Max Sample Error",
                        "The maximum distance the detail mesh surface should deviate from heightfield data.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 16)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Tiling")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &BehaveNavigationMeshSettingsAsset::m_enableTiling, "Enable Tiling",
                        "If enabled, the navigation mesh will be built separated in tiles.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_tileSize, "Tile Size",
                        "The width/height size of tile's on the xy-plane.")
                    ->Attribute(AZ::Edit::Attributes::Min, 16)
                    ->Attribute(AZ::Edit::Attributes::Max, 1024)
                    ->Attribute(AZ::Edit::Attributes::Step, 16)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &BehaveNavigationMeshSettingsAsset::m_borderPadding, "Border Padding",
                        "The border padding of each tiles.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 64)
                    ->Attribute(AZ::Edit::Attributes::Step, 1);
            }
        }
    }

    BehaveNavigationMeshSettingsAsset::NavigationAgentComboBoxEntries BehaveNavigationMeshSettingsAsset::
        BuildSelectableNavigationAgentList() const
    {
        NavigationAgentList agents;
        EBUS_EVENT(NavigationAgentProviderRequestBus, GetRegisteredNavigationAgents, agents);

        NavigationAgentComboBoxEntries selectableAreas;
        selectableAreas.reserve(agents.size());
        for (const auto& agent : agents)
        {
            selectableAreas.push_back({ agent.GetId(), agent.GetName() });
        }

        AZStd::sort(
            selectableAreas.begin(), selectableAreas.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.second < rhs.second;
            });

        return selectableAreas;
    }

    AZ::Crc32 BehaveNavigationMeshSettingsAsset::OnNavigationAgentChanged()
    {
        NavigationAgentList agents;
        EBUS_EVENT(NavigationAgentProviderRequestBus, GetRegisteredNavigationAgents, agents);

        for (const auto& agent : agents)
        {
            if (agent.GetId() == _agentId)
            {
                m_agent = agent;
                break;
            }
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

    AZ::Crc32 BehaveNavigationMeshSettingsAsset::OnClearAgent()
    {
        _agentId = AZ::Crc32();
        OnNavigationAgentChanged();

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

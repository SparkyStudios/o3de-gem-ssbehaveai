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

#include <SparkyStudios/AI/Behave/Navigation/NavigationAgent.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace SparkyStudios::AI::Behave::Navigation
{
    enum class NavigationMeshPartitionType
    {
        Watershed,
        Monotone,
        Layers,
    };

    class NavigationMeshSettingsAsset : public AZ::Data::AssetData
    {
    public:
        AZ_RTTI(BehaveNavigationMeshSettingsAsset, "{E59EBE08-1A42-4A9F-82E9-C7156CE72034}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(NavigationMeshSettingsAsset, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief The navigation mesh settings name.
         */
        AZStd::string m_name;

        NavigationAgent m_agent;

        int m_borderPadding = 3;

        float m_cellSize = 0.3f;
        float m_cellHeight = 0.2f;

        int m_regionMinSize = 8;
        int m_regionMergedSize = 20;

        NavigationMeshPartitionType m_partitionType = NavigationMeshPartitionType::Watershed;

        bool m_filterLowHangingObstacles = true;
        bool m_filterLedgeSpans = true;
        bool m_filterWalkableLowHeightSpans = true;

        float m_edgeMaxError = 1.3f;
        int m_edgeMaxLength = 12;
        int m_maxVerticesPerPoly = 6;

        int m_detailSampleDist = 6;
        int m_detailSampleMaxError = 1;

        bool m_enableTiling = true;
        int m_tileSize = 16;

        private:
        typedef AZStd::vector<AZStd::pair<AZ::u32, AZStd::string>> NavigationAgentComboBoxEntries;

        [[nodiscard]] NavigationAgentComboBoxEntries BuildSelectableNavigationAgentList() const;
        AZ::Crc32 OnNavigationAgentChanged();
        AZ::Crc32 OnClearAgent();

        AZ::u32 _agentId;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(SparkyStudios::AI::Behave::Navigation::NavigationMeshPartitionType, "{4DEE04E5-0F7E-4442-86CE-5D996B13E3F7}");
} // namespace AZ

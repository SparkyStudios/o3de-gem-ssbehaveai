#pragma once

#include <Navigation/Assets/BehaveNavigationAgentAsset.h>

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

    class BehaveNavigationMeshSettingsAsset : public AZ::Data::AssetData
    {
    public:
        AZ_RTTI(BehaveNavigationMeshSettingsAsset, "{E59EBE08-1A42-4A9F-82E9-C7156CE72034}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(BehaveNavigationMeshSettingsAsset, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief The navigation mesh settings name.
         */
        AZStd::string m_name;

        AZ::Data::Asset<BehaveNavigationAgentAsset> m_agent;

        int m_borderPadding = 0;

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
    };
} // namespace SparkyStudios::AI::Behave::Navigation

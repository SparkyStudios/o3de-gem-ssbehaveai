#pragma once

#include <SparkyStudios/AI/Behave/Navigation/OffMeshConnection.h>

#include <AzCore/Math/Aabb.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class BehaveNavigationMeshSettingsAsset;

    class IBehaveNavigationMesh
    {
    public:
        virtual ~IBehaveNavigationMesh() = default;

        [[nodiscard]] virtual const BehaveNavigationMeshSettingsAsset* GetSettings() const = 0;
        [[nodiscard]] virtual const AZ::Aabb& GetBoundingBox() const = 0;
        [[nodiscard]] virtual const OffMeshConnections& GetOffMeshConnections() const = 0;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

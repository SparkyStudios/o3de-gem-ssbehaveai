#pragma once

#include <AzCore/Component/Component.h>
#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class WalkableComponent
        : public AZ::Component
        , public BehaveWalkableRequestBus::Handler
    {
        friend class WalkableEditorComponent;

    public:
        AZ_COMPONENT(WalkableComponent, "{D993514D-ED4F-46A9-A4B6-B90FDE5267E8}");

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // BehaveWalkableRequestBus
        bool IsWalkable([[maybe_unused]] AZ::EntityId navigationMeshEntity) override
        {
            return m_isWalkable;
        }
        //////////////////////////////////////////////////////////////////////////

    private:
        bool m_isWalkable = true;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

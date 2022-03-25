#pragma once

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

#include <Navigation/Components/WalkableComponent.h>

#include <AzCore/Component/Component.h>

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class WalkableEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , public BehaveWalkableRequestBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(WalkableEditorComponent, "{C1AD31CA-56F7-4036-92A6-DB4ABF9AE9FE}");

        static void Reflect(AZ::ReflectContext* rc);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;
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

        WalkableComponent _component;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

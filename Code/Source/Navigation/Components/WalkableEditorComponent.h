#pragma once

#include <SparkyStudios/AI/Behave/Navigation/NavigationMeshBus.h>

#include <Navigation/Components/WalkableComponent.h>

#include <AzCore/Component/Component.h>

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class WalkableEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , public WalkableRequestBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(WalkableEditorComponent, "{C1AD31CA-56F7-4036-92A6-DB4ABF9AE9FE}");

        static void Reflect(AZ::ReflectContext* rc);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;

        // WalkableRequestBus
        bool IsWalkable([[maybe_unused]] AZ::EntityId navigationMeshEntity) override
        {
            return _isWalkable;
        }

    private:
        bool _isWalkable = true;

        WalkableComponent _component;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

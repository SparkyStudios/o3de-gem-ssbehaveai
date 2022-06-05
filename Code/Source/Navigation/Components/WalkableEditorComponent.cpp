#include <Navigation/Components/WalkableEditorComponent.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void WalkableEditorComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            WalkableComponent::Reflect(rc);

            sc->Class<WalkableEditorComponent, EditorComponentBase>()->Version(0)->Field("walkable", &WalkableEditorComponent::_isWalkable);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<WalkableEditorComponent>("Walkable", "[Marks this entity to be considered by navigation mesh]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(nullptr, &WalkableEditorComponent::_isWalkable, "Is Walkable", "");
            }
        }
    }

    void WalkableEditorComponent::Activate()
    {
        WalkableRequestBus::Handler::BusConnect(GetEntityId());

        _component.Init();
        _component.m_isWalkable = _isWalkable;
    }

    void WalkableEditorComponent::Deactivate()
    {
        WalkableRequestBus::Handler::BusDisconnect();
    }

    void WalkableEditorComponent::BuildGameEntity(AZ::Entity* entity)
    {
        AZ::SerializeContext* context = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(context, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        if (!context)
        {
            AZ_Error("BehaveAI", false, "Unable to get a serialize context from component application.");
            return;
        }

        entity->AddComponent(context->CloneObject(&_component));
    }

} // namespace SparkyStudios::AI::Behave::Navigation

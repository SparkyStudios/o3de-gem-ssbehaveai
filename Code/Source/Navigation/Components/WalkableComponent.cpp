#include <Navigation/Components/WalkableComponent.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void WalkableComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (AZ::SerializeContext* sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            if (sc->FindClassData(azrtti_typeid<WalkableComponent>()) == nullptr)
            {
                sc->Class<WalkableComponent, AZ::Component>()->Version(2)->Field("Is Walkable", &WalkableComponent::m_isWalkable);
            }
        }
    }

    void WalkableComponent::Activate()
    {
        WalkableRequestBus::Handler::BusConnect(GetEntityId());
    }

    void WalkableComponent::Deactivate()
    {
        WalkableRequestBus::Handler::BusDisconnect();
    }
} // namespace SparkyStudios::AI::Behave::Navigation

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

#include <AzCore/Component/TransformBus.h>
#include <AzCore/EBus/Internal/BusContainer.h>
#include <AzCore/Math/PolygonPrism.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <LmbrCentral/Shape/PolygonPrismShapeComponentBus.h>

#include <Navigation/Components/NavigationMeshAreaEditorComponent.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationMeshAreaEditorComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            NavigationMeshAreaComponent::Reflect(rc);

            sc->Class<NavigationMeshAreaEditorComponent, EditorComponentBase>()
                ->Version(0)
                ->Field("Area", &NavigationMeshAreaEditorComponent::_area)
                ->Field("Volume", &NavigationMeshAreaEditorComponent::_polygonPrism);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<NavigationMeshAreaEditorComponent>("Navigation Mesh Area", "Set the navigation cost for a specific area.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Behave AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &NavigationMeshAreaEditorComponent::_area, "Area", "Area")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }
    }

    void NavigationMeshAreaEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("NavigationMeshAreaService"));
    }

    void NavigationMeshAreaEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("NavigationMeshAreaService"));
    }

    void NavigationMeshAreaEditorComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("PolygonPrismShapeService", 0x1cbc4ed4));
        required.push_back(AZ_CRC_CE("PhysXShapeColliderService"));
    }

    void NavigationMeshAreaEditorComponent::Activate()
    {
        AZ::TransformNotificationBus::Handler::BusConnect(GetEntityId());
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusConnect(GetEntityId());
        BehaveNavigationMeshAreaRequestBus::Handler::BusConnect(GetEntityId());

        _component.Init();
        UpdatePolygonPrism();
    }

    void NavigationMeshAreaEditorComponent::Deactivate()
    {
        BehaveNavigationMeshAreaRequestBus::Handler::BusDisconnect(GetEntityId());
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusDisconnect(GetEntityId());
        AZ::TransformNotificationBus::Handler::BusDisconnect(GetEntityId());
    }

    void NavigationMeshAreaEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        AZ::SerializeContext* context = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(context, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        if (!context)
        {
            AZ_Error("BehaveAI", false, "Unable to get a serialize context from component application.");
            return;
        }

        SyncComponent();
        gameEntity->AddComponent(context->CloneObject(&_component));
    }

    bool NavigationMeshAreaEditorComponent::IsNavigationMeshArea([[maybe_unused]] AZ::EntityId navigationMeshEntityId)
    {
        // TODO: Add entities exception list
        return true;
    }

    BehaveNavigationMeshArea NavigationMeshAreaEditorComponent::GetNavigationMeshArea()
    {
        return _area;
    }

    AZ::PolygonPrism NavigationMeshAreaEditorComponent::GetNavigationMeshAreaPolygon()
    {
        return _polygonPrism;
    }

    void NavigationMeshAreaEditorComponent::OnShapeChanged(ShapeChangeReasons changeReason)
    {
        if (changeReason == ShapeChangeReasons::ShapeChanged)
        {
            UpdatePolygonPrism();
        }
    }

    void NavigationMeshAreaEditorComponent::OnTransformChanged(
        [[maybe_unused]] const AZ::Transform& transform, [[maybe_unused]] const AZ::Transform& transform1)
    {
        UpdatePolygonPrism();
    }

    void NavigationMeshAreaEditorComponent::SyncComponent()
    {
        _component._area = _area;
        _component._polygonPrism = _polygonPrism;
    }

    void NavigationMeshAreaEditorComponent::UpdatePolygonPrism()
    {
        AZ::PolygonPrismPtr polygonPrismPtr = nullptr;
        EBUS_EVENT_ID_RESULT(polygonPrismPtr, GetEntityId(), LmbrCentral::PolygonPrismShapeComponentRequestBus, GetPolygonPrism);

        if (polygonPrismPtr)
        {
            AZ::Vector3 start = AZ::Vector3::CreateZero();
            EBUS_EVENT_ID_RESULT(start, GetEntityId(), AZ::TransformBus, GetWorldTranslation);

            _polygonPrism = *polygonPrismPtr;
        }

        SyncComponent();
    }
} // namespace SparkyStudios::AI::Behave::Navigation

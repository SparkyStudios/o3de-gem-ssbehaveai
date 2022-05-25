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

#include <AzCore/EBus/Internal/BusContainer.h>
#include <AzCore/Math/PolygonPrism.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <LmbrCentral/Shape/PolygonPrismShapeComponentBus.h>

#include <Navigation/BehaveNavigationMeshAreaProviderRequestBus.h>
#include <Navigation/Components/NavigationMeshAreaComponent.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationMeshAreaComponent::Reflect(AZ::ReflectContext* rc)
    {
        BehaveNavigationMeshArea::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<NavigationMeshAreaComponent, AZ::Component>()->Version(0)->Field("Area", &NavigationMeshAreaComponent::_area);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<NavigationMeshAreaComponent>("Navigation Mesh Area", "Set the navigation cost for a specific area.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Behave AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &NavigationMeshAreaComponent::_area, "Area", "Area")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }
    }

    void NavigationMeshAreaComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("NavigationMeshAreaService"));
    }

    void NavigationMeshAreaComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("NavigationMeshAreaService"));
    }

    void NavigationMeshAreaComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("PolygonPrismShapeService", 0x1cbc4ed4));
        required.push_back(AZ_CRC_CE("PhysXShapeColliderService"));
    }

    void NavigationMeshAreaComponent::Activate()
    {
        BehaveNavigationMeshAreaRequestBus::Handler::BusConnect(GetEntityId());
    }

    void NavigationMeshAreaComponent::Deactivate()
    {
        BehaveNavigationMeshAreaRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    bool NavigationMeshAreaComponent::IsNavigationMeshArea([[maybe_unused]] AZ::EntityId navigationMeshEntityId)
    {
        // TODO: Add entities exception list
        return true;
    }

    BehaveNavigationMeshArea NavigationMeshAreaComponent::GetNavigationMeshArea()
    {
        return _area;
    }

    AZ::PolygonPrism NavigationMeshAreaComponent::GetNavigationMeshAreaPolygon()
    {
        return _polygonPrism;
    }

    void NavigationMeshAreaComponent::OnShapeChanged(ShapeChangeReasons changeReason)
    {
        if (changeReason == ShapeChangeReasons::ShapeChanged)
        {
            AZ::PolygonPrismPtr polygonPrismPtr = nullptr;
            EBUS_EVENT_ID_RESULT(polygonPrismPtr, GetEntityId(), LmbrCentral::PolygonPrismShapeComponentRequestBus, GetPolygonPrism);

            if (polygonPrismPtr)
            {
                _polygonPrism = *polygonPrismPtr;
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation

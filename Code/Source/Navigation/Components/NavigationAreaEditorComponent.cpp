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
#include <AzCore/Debug/Profiler.h>
#include <AzCore/EBus/Internal/BusContainer.h>
#include <AzCore/Math/PolygonPrism.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/sort.h>

#include <LmbrCentral/Shape/PolygonPrismShapeComponentBus.h>

#include <Navigation/Components/NavigationAreaComponent.h>
#include <Navigation/Components/NavigationAreaEditorComponent.h>

#include <Source/Navigation/NavigationAreaProviderRequestBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationAreaEditorComponent::Reflect(AZ::ReflectContext* rc)
    {
        NavigationAreaComponent::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<NavigationAreaEditorComponent, EditorComponentBase>()
                ->Version(0)
                ->Field("Area", &NavigationAreaEditorComponent::_areaId)
                ->Field("Volume", &NavigationAreaEditorComponent::_polygonPrism);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<NavigationAreaEditorComponent>("Navigation Mesh Area", "Set the navigation cost for a specific area.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Behave AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &NavigationAreaEditorComponent::_areaId, "Area", "The navigation mesh area name.")
                    ->Attribute(AZ::Edit::Attributes::EnumValues, &NavigationAreaEditorComponent::BuildSelectableNavigationMeshAreaList)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }

    void NavigationAreaEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("BehaveAI_NavigationAreaService"));
    }

    void NavigationAreaEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("BehaveAI_NavigationAreaService"));
    }

    void NavigationAreaEditorComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("PolygonPrismShapeService", 0x1cbc4ed4));
        required.push_back(AZ_CRC_CE("PhysXShapeColliderService"));
    }

    void NavigationAreaEditorComponent::Activate()
    {
        AZ::TransformNotificationBus::Handler::BusConnect(GetEntityId());
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusConnect(GetEntityId());
        NavigationAreaRequestBus::Handler::BusConnect(GetEntityId());

        UpdatePolygonPrism();
    }

    void NavigationAreaEditorComponent::Deactivate()
    {
        NavigationAreaRequestBus::Handler::BusDisconnect(GetEntityId());
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusDisconnect(GetEntityId());
        AZ::TransformNotificationBus::Handler::BusDisconnect(GetEntityId());
    }

    void NavigationAreaEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        gameEntity->CreateComponent<NavigationAreaComponent>(GetNavigationMeshArea(), GetNavigationMeshAreaPolygon());
    }

    bool NavigationAreaEditorComponent::IsNavigationMeshArea([[maybe_unused]] AZ::EntityId navigationMeshEntityId)
    {
        // TODO: Add entities exception list
        return true;
    }

    NavigationArea NavigationAreaEditorComponent::GetNavigationMeshArea()
    {
        BehaveNavigationMeshAreaVector areas;
        EBUS_EVENT(NavigationAreaProviderRequestBus, GetRegisteredNavigationAreas, areas);

        for (const auto& area : areas)
        {
            if (area.GetId() == _areaId)
            {
                return area;
            }
        }

        return NavigationArea::Default();
    }

    AZ::PolygonPrism NavigationAreaEditorComponent::GetNavigationMeshAreaPolygon()
    {
        return _polygonPrism;
    }

    void NavigationAreaEditorComponent::OnShapeChanged(ShapeChangeReasons changeReason)
    {
        if (changeReason == ShapeChangeReasons::ShapeChanged)
        {
            UpdatePolygonPrism();
        }
    }

    void NavigationAreaEditorComponent::OnTransformChanged(
        [[maybe_unused]] const AZ::Transform& transform, [[maybe_unused]] const AZ::Transform& transform1)
    {
        UpdatePolygonPrism();
    }

    void NavigationAreaEditorComponent::UpdatePolygonPrism()
    {
        AZ::PolygonPrismPtr polygonPrismPtr = nullptr;
        EBUS_EVENT_ID_RESULT(polygonPrismPtr, GetEntityId(), LmbrCentral::PolygonPrismShapeComponentRequestBus, GetPolygonPrism);

        if (polygonPrismPtr)
        {
            AZ::Vector3 start = AZ::Vector3::CreateZero();
            EBUS_EVENT_ID_RESULT(start, GetEntityId(), AZ::TransformBus, GetWorldTranslation);

            _polygonPrism = *polygonPrismPtr;
        }
    }

    NavigationAreaEditorComponent::NavigationAreaComboBoxEntries NavigationAreaEditorComponent::BuildSelectableNavigationMeshAreaList()
        const
    {
        AZ_PROFILE_FUNCTION(Entity);

        BehaveNavigationMeshAreaVector areas;
        EBUS_EVENT(NavigationAreaProviderRequestBus, GetRegisteredNavigationAreas, areas);

        NavigationAreaComboBoxEntries selectableAreas;
        selectableAreas.reserve(areas.size());
        for (const auto& area : areas)
        {
            selectableAreas.push_back({ area.GetId(), area.GetName() });
        }

        AZStd::sort(
            selectableAreas.begin(), selectableAreas.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.second < rhs.second;
            });

        return selectableAreas;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

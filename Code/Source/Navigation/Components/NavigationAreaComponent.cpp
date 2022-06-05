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
#include <AzCore/Serialization/SerializeContext.h>

#include <LmbrCentral/Shape/PolygonPrismShapeComponentBus.h>

#include <Navigation/NavigationAreaProviderRequestBus.h>
#include <Navigation/Components/NavigationAreaComponent.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationAreaComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // We may have been reflected by NavigationAreaEditorComponent already, so check first
            if (sc->FindClassData(azrtti_typeid<NavigationAreaComponent>()) == nullptr)
            {
                sc->Class<NavigationAreaComponent, Component>()
                    ->Version(0)
                    ->Field("Area", &NavigationAreaComponent::_area)
                    ->Field("Volume", &NavigationAreaComponent::_polygonPrism);
            }
        }
    }

    NavigationAreaComponent::NavigationAreaComponent(NavigationArea area, AZ::PolygonPrism polygon)
        : _area(AZStd::move(area))
        , _polygonPrism(AZStd::move(polygon))
    {
    }

    void NavigationAreaComponent::Init()
    {
        Component::Init();
    }

    void NavigationAreaComponent::Activate()
    {
        NavigationAreaRequestBus::Handler::BusConnect(GetEntityId());
    }

    void NavigationAreaComponent::Deactivate()
    {
        NavigationAreaRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    bool NavigationAreaComponent::IsNavigationMeshArea([[maybe_unused]] AZ::EntityId navigationMeshEntityId)
    {
        // TODO: Add entities exception list
        return true;
    }

    NavigationArea NavigationAreaComponent::GetNavigationMeshArea()
    {
        return _area;
    }

    AZ::PolygonPrism NavigationAreaComponent::GetNavigationMeshAreaPolygon()
    {
        return _polygonPrism;
    }
} // namespace SparkyStudios::AI::Behave::Navigation

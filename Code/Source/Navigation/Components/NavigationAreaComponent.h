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

#pragma once

#include <AzCore/Component/Component.h>

#include <Navigation/Utils/RecastMath.h>

#include <SparkyStudios/AI/Behave/Navigation/NavigationArea.h>
#include <SparkyStudios/AI/Behave/Navigation/NavigationMeshBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class NavigationAreaComponent
        : public AZ::Component
        , public NavigationAreaRequestBus::Handler
    {
    public:
        AZ_COMPONENT(NavigationAreaComponent, "{2D8C08DA-9A0B-4644-AA1E-D71C5A08F46D}");

        static void Reflect(AZ::ReflectContext* rc);

        NavigationAreaComponent() = default;
        explicit NavigationAreaComponent(NavigationArea area, AZ::PolygonPrism polygon);

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // NavigationAreaRequestBus
        bool IsNavigationMeshArea(AZ::EntityId navigationMeshEntityId) override;
        NavigationArea GetNavigationMeshArea() override;
        AZ::PolygonPrism GetNavigationMeshAreaPolygon() override;

    private:
        NavigationArea _area;
        AZ::PolygonPrism _polygonPrism;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

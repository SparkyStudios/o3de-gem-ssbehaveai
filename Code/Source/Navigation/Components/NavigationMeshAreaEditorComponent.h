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

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

#include <LmbrCentral/Shape/ShapeComponentBus.h>

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshArea.h>
#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class NavigationMeshAreaEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , public BehaveNavigationMeshAreaRequestBus::Handler
        , private LmbrCentral::ShapeComponentNotificationsBus::Handler
        , private AZ::TransformNotificationBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(NavigationMeshAreaEditorComponent, "{0087E9C7-7C09-4C81-A489-D46A436F31EE}");

        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;

        // BehaveNavigationMeshAreaRequestBus
        bool IsNavigationMeshArea(AZ::EntityId navigationMeshEntityId) override;
        BehaveNavigationMeshArea GetNavigationMeshArea() override;
        AZ::PolygonPrism GetNavigationMeshAreaPolygon() override;

        // LmbrCentral::ShapeComponentNotificationsBus
        void OnShapeChanged(ShapeChangeReasons changeReason) override;

        // AZ::TransformNotificationBus
        void OnTransformChanged(const AZ::Transform& /*local*/, const AZ::Transform& /*world*/) override;

    private:
        typedef AZStd::vector<AZStd::pair<AZ::u8, AZStd::string>> NavigationAreaComboBoxEntries;

        void UpdatePolygonPrism();
        [[nodiscard]] NavigationAreaComboBoxEntries BuildSelectableNavigationMeshAreaList() const;

        AZ::u8 _areaId;
        AZ::PolygonPrism _polygonPrism;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

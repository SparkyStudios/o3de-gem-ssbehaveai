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

#include <Navigation/BehaveNavigationSystemComponent.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    /// System component for BehaveNavigation editor
    class BehaveNavigationEditorSystemComponent
        : public BehaveNavigationSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = BehaveNavigationSystemComponent;

    public:
        AZ_COMPONENT(BehaveNavigationEditorSystemComponent, "{2F22E936-57CC-4739-97A1-1175042179AD}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        BehaveNavigationEditorSystemComponent();
        ~BehaveNavigationEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
} // namespace SparkyStudios::AI::Behave::Navigation

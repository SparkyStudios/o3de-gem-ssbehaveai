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

namespace SparkyStudios::AI::Behave::Navigation
{
    class NavigationSystemComponent : public AZ::Component
    {
    public:
        AZ_COMPONENT(NavigationSystemComponent, "{45F8E5D4-D7DD-47F9-A9A7-2A65C9FE924A}");

        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        NavigationSystemComponent() = default;
        ~NavigationSystemComponent() override = default;

    protected:
        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

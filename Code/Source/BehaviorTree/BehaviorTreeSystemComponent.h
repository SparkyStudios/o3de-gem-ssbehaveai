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

#include <SparkyStudios/AI/Behave/BehaviorTree/BehaveBehaviorTreeBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class BehaviorTreeSystemComponent
        : public AZ::Component
        , protected BehaveBehaviorTreeRequestBus::Handler
    {
    public:
        AZ_COMPONENT(BehaviorTreeSystemComponent, "{4A9E985C-692E-47C3-9573-94DD1AA64DCE}");

        static void Reflect(AZ::ReflectContext* rc);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        BehaviorTreeSystemComponent();
        ~BehaviorTreeSystemComponent() override;

    protected:
        // BehaveBehaviorTreeRequestBus
        const Core::Factory& GetFactory() const override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

    private:
        void RegisterDefaultProperties() const;
        void RegisterDefaultNodes() const;

        Core::Factory _factory;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree

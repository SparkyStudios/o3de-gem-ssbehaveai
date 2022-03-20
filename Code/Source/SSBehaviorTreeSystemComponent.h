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
#include <AzCore/Component/TickBus.h>

#include <SparkyStudios/AI/Behave/SSBehaviorTreeBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class SSBehaviorTreeSystemComponent
        : public AZ::Component
        , protected SSBehaviorTreeRequestBus::Handler
    {
    public:
        AZ_COMPONENT(SSBehaviorTreeSystemComponent, "{4a9e985c-692e-47c3-9573-94dd1aa64dce}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        SSBehaviorTreeSystemComponent();
        ~SSBehaviorTreeSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // SSBehaviorTreeRequestBus interface implementation
        const Core::SSBehaviorTreeFactory& GetFactory() const override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

    private:
        void RegisterDefaultProperties();
        void RegisterDefaultNodes();

        Core::SSBehaviorTreeFactory m_factory;
    };

} // namespace SparkyStudios::AI::Behave::BehaviorTree

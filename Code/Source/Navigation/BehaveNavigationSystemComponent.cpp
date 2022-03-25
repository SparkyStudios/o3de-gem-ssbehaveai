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

#include <Navigation/BehaveNavigationSystemComponent.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void BehaveNavigationSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<BehaveNavigationSystemComponent, AZ::Component>()->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<BehaveNavigationSystemComponent>(
                      "BehaveNavigation", "2D navigation meshes and path finding powered by Recast Navigation.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void BehaveNavigationSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("BehaveNavigationService"));
    }

    void BehaveNavigationSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("BehaveNavigationService"));
    }

    void BehaveNavigationSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void BehaveNavigationSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    BehaveNavigationSystemComponent::BehaveNavigationSystemComponent()
    {
        if (BehaveNavigationInterface::Get() == nullptr)
        {
            BehaveNavigationInterface::Register(this);
        }
    }

    BehaveNavigationSystemComponent::~BehaveNavigationSystemComponent()
    {
        if (BehaveNavigationInterface::Get() == this)
        {
            BehaveNavigationInterface::Unregister(this);
        }
    }

    void BehaveNavigationSystemComponent::Init()
    {
    }

    void BehaveNavigationSystemComponent::Activate()
    {
        BehaveNavigationRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void BehaveNavigationSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        BehaveNavigationRequestBus::Handler::BusDisconnect();
    }

    void BehaveNavigationSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }
} // namespace SparkyStudios::AI::Behave::Navigation
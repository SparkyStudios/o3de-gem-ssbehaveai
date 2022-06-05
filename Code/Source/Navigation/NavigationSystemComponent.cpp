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

#include <Navigation/Assets/NavigationAgentsAsset.h>
#include <Navigation/Assets/NavigationAreasAsset.h>
#include <Navigation/Assets/NavigationMeshSettingsAsset.h>
#include <Navigation/NavigationSystemComponent.h>

#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationSystemComponent::Reflect(AZ::ReflectContext* rc)
    {
        NavigationAgentsAsset::Reflect(rc);
        NavigationAreasAsset::Reflect(rc);
        NavigationMeshSettingsAsset::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<NavigationSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void NavigationSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("BehaveAI_NavigationService"));
    }

    void NavigationSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("BehaveAI_NavigationService"));
    }

    void NavigationSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void NavigationSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void NavigationSystemComponent::Init()
    {
        Component::Init();
    }

    void NavigationSystemComponent::Activate()
    {
    }

    void NavigationSystemComponent::Deactivate()
    {
    }
} // namespace SparkyStudios::AI::Behave::Navigation

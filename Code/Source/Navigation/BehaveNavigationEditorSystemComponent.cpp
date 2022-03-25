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

#include <AzCore/Serialization/SerializeContext.h>

#include <Navigation/BehaveNavigationEditorSystemComponent.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void BehaveNavigationEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BehaveNavigationEditorSystemComponent, BehaveNavigationSystemComponent>()->Version(0);
        }
    }

    BehaveNavigationEditorSystemComponent::BehaveNavigationEditorSystemComponent() = default;

    BehaveNavigationEditorSystemComponent::~BehaveNavigationEditorSystemComponent() = default;

    void BehaveNavigationEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("BehaveNavigationEditorService"));
    }

    void BehaveNavigationEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("BehaveNavigationEditorService"));
    }

    void BehaveNavigationEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void BehaveNavigationEditorSystemComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void BehaveNavigationEditorSystemComponent::Activate()
    {
        BehaveNavigationSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void BehaveNavigationEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        BehaveNavigationSystemComponent::Deactivate();
    }
} // namespace SparkyStudios::AI::Behave::Navigation

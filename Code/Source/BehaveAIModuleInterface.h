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

#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTreeSystemComponent.h>

#include <Navigation/NavigationSystemComponent.h>
#include <Navigation/Components/DynamicNavigationMeshComponent.h>
#include <Navigation/Components/NavigationAreaComponent.h>
#include <Navigation/Components/WalkableComponent.h>

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

namespace SparkyStudios::AI::Behave
{
    class BehaveAIModuleInterface : public AZ::Module
    {
    public:
        AZ_RTTI(BehaveAIModuleInterface, "{0c335fb2-93a5-4f99-a8cd-8ff1c7763dc1}", AZ::Module);
        AZ_CLASS_ALLOCATOR(BehaveAIModuleInterface, AZ::SystemAllocator, 0);

        BehaveAIModuleInterface()
        {
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    BehaviorTree::BehaviorTreeSystemComponent::CreateDescriptor(),
                    BehaviorTree::BehaviorTreeComponent::CreateDescriptor(),

                    Navigation::NavigationSystemComponent::CreateDescriptor(),
                    Navigation::NavigationAreaComponent::CreateDescriptor(),
                    Navigation::DynamicNavigationMeshComponent::CreateDescriptor(),
                    Navigation::WalkableComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        [[nodiscard]] AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<BehaviorTree::BehaviorTreeSystemComponent>(),

                azrtti_typeid<Navigation::NavigationSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::Behave

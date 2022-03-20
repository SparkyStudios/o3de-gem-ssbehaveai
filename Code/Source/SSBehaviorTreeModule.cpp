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

#include <StdAfx.h>

#include <SSBehaviorTreeComponent.h>
#include <SSBehaviorTreeModuleInterface.h>
#include <SSBehaviorTreeSystemComponent.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class SSBehaviorTreeModule : public SSBehaviorTreeModuleInterface
    {
    public:
        AZ_RTTI(SSBehaviorTreeModule, "{2fc25376-c0a0-4b56-bfbe-5a65bad02586}", SSBehaviorTreeModuleInterface);
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeModule, AZ::SystemAllocator, 0);

        SSBehaviorTreeModule()
        {
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SSBehaviorTreeSystemComponent::CreateDescriptor(),
                    SSBehaviorTreeComponent::CreateDescriptor(),
                });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SSBehaviorTreeSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree

AZ_DECLARE_MODULE_CLASS(Gem_SSBehaviorTree, SparkyStudios::AI::Behave::BehaviorTree::SSBehaviorTreeModule)

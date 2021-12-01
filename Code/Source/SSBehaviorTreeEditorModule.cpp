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
#include <SSBehaviorTreeEditorComponent.h>
#include <SSBehaviorTreeEditorSystemComponent.h>
#include <SSBehaviorTreeModuleInterface.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeEditorModule : public SSBehaviorTreeModuleInterface
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeEditorModule, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeEditorModule, "{2fc25376-c0a0-4b56-bfbe-5a65bad02586}", SSBehaviorTreeModuleInterface);

        SSBehaviorTreeEditorModule()
        {
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SSBehaviorTreeEditorSystemComponent::CreateDescriptor(),
                    SSBehaviorTreeComponent::CreateDescriptor(),
                    SSBehaviorTreeEditorComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SSBehaviorTreeEditorSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::BehaviorTree

AZ_DECLARE_MODULE_CLASS(Gem_SSBehaviorTree, SparkyStudios::AI::BehaviorTree::SSBehaviorTreeEditorModule)

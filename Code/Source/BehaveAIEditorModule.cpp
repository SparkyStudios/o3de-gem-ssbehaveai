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

#include <BehaveAIModuleInterface.h>

#include <BehaviorTree/SSBehaviorTreeEditorComponent.h>
#include <BehaviorTree/SSBehaviorTreeEditorSystemComponent.h>

namespace SparkyStudios::AI::Behave
{
    class BehaveAIEditorModule : public BehaveAIModuleInterface
    {
    public:
        AZ_RTTI(BehaveAIEditorModule, "{2FC25376-C0A0-4B56-BFBE-5A65BAD02586}", BehaveAIModuleInterface);
        AZ_CLASS_ALLOCATOR(BehaveAIEditorModule, AZ::SystemAllocator, 0);

        BehaveAIEditorModule()
            : BehaveAIModuleInterface()
        {
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    BehaviorTree::SSBehaviorTreeEditorSystemComponent::CreateDescriptor(),
                    BehaviorTree::SSBehaviorTreeEditorComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<BehaviorTree::SSBehaviorTreeEditorSystemComponent>(),
            };
        }
    };
} // namespace SparkyStudios::AI::Behave

AZ_DECLARE_MODULE_CLASS(Gem_SSBehaveAI, SparkyStudios::AI::Behave::BehaveAIEditorModule)

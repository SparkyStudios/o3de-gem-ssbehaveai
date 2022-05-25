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

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
    BlackboardProperty::BlackboardProperty(const char* name)
        : mId(AZ::Crc32(name))
        , mName(name)
    {
    }

    void BlackboardProperty::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardProperty>()
                ->Version(0)
                ->PersistentId(
                    [](const void* instance) -> AZ::u64
                    {
                        return static_cast<const BlackboardProperty*>(instance)->mId;
                    })
                ->Field("Id", &BlackboardProperty::mId)
                ->Field("Name", &BlackboardProperty::mName)
                ->Field("Description", &BlackboardProperty::mDescription)
                ->Field("Visibility", &BlackboardProperty::mVisibility)
                ->Field("Suffix", &BlackboardProperty::mSuffix);
        }
    }

    bool BlackboardProperty::IsNil() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyNil>();
    }

    bool BlackboardProperty::IsBoolean() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyBoolean>();
    }

    bool BlackboardProperty::IsNumber() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyNumber>();
    }

    bool BlackboardProperty::IsString() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyString>();
    }

    bool BlackboardProperty::IsEntity() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyEntityRef>();
    }

    bool BlackboardProperty::IsVector2() const
    {
        return GetDataTypeUuid() == azrtti_typeid<BlackboardPropertyVector2>();
    }

    bool BlackboardProperty::IsVector3() const
    {
        // TODO: Implement a SSBehaviorTreeBlackboardPropertyVector3
        return false;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

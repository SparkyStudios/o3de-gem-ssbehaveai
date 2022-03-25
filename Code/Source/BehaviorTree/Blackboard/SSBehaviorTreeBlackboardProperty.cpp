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

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
    SSBehaviorTreeBlackboardProperty::SSBehaviorTreeBlackboardProperty(const char* name)
        : m_id(AZ::Crc32(name))
        , m_name(name)
    {
    }

    void SSBehaviorTreeBlackboardProperty::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<SSBehaviorTreeBlackboardProperty>()
                ->Version(0)
                ->PersistentId(
                    [](const void* instance) -> AZ::u64
                    {
                        return reinterpret_cast<const SSBehaviorTreeBlackboardProperty*>(instance)->m_id;
                    })
                ->Field("id", &SSBehaviorTreeBlackboardProperty::m_id)
                ->Field("name", &SSBehaviorTreeBlackboardProperty::m_name)
                ->Field("description", &SSBehaviorTreeBlackboardProperty::m_description)
                ->Field("visibility", &SSBehaviorTreeBlackboardProperty::m_visibility)
                ->Field("suffix", &SSBehaviorTreeBlackboardProperty::m_suffix);
        }
    }

    bool SSBehaviorTreeBlackboardProperty::IsNil() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyNil>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsBoolean() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyBoolean>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsNumber() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyNumber>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsString() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyString>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsEntity() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyEntityRef>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsVector2() const
    {
        return GetDataTypeUuid() == azrtti_typeid<SSBehaviorTreeBlackboardPropertyVector2>();
    }

    bool SSBehaviorTreeBlackboardProperty::IsVector3() const
    {
        // TODO: Implement a SSBehaviorTreeBlackboardPropertyVector3
        return false;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

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

#include <AzCore/RTTI/BehaviorContext.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
#pragma region Blackboard

    BlackboardProperty* Blackboard::GetProperty(const char* propertyName)
    {
        for (auto* prop : mProperties)
        {
            if (prop->mName == propertyName)
            {
                return prop;
            }
        }

        return nullptr;
    }

    void Blackboard::Clear()
    {
        for (const auto* prop : mProperties)
        {
            delete prop;
        }

        mProperties.clear();

        BT::Blackboard::Ptr bb = BT::Blackboard::create();
        mBlackboard.swap(bb);
    }

    Blackboard::Blackboard(Blackboard&& rhs) noexcept
    {
        *this = AZStd::move(rhs);
    }

    Blackboard::~Blackboard()
    {
        Clear();
    }

    Blackboard& Blackboard::operator=(Blackboard&& rhs) noexcept
    {
        mName.swap(rhs.mName);
        mProperties.swap(rhs.mProperties);
        mBlackboard.swap(rhs.mBlackboard);

        return *this;
    }

#pragma endregion

#pragma region BlackboardPropertyNil

    void BlackboardPropertyNil::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyNil, BlackboardProperty>()->Version(0)->SerializeWithNoData();
        }
    }

    BlackboardProperty* BlackboardPropertyNil::TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name)
    {
        BlackboardProperty* retVal = nullptr;

        if (const BT::Any* prop = context->getAny(name))
        {
            if (prop->empty())
            {
                retVal = aznew BlackboardPropertyNil(name);
            }
        }

        return retVal;
    }

    BlackboardPropertyNil::BlackboardPropertyNil(const char* name)
        : BlackboardProperty(name)
    {
    }

    const void* BlackboardPropertyNil::GetDataAddress() const
    {
        return nullptr;
    }

    const AZ::Uuid& BlackboardPropertyNil::GetDataTypeUuid() const
    {
        return azrtti_typeid<void*>();
    }

    BlackboardPropertyNil* BlackboardPropertyNil::Clone(const char* name) const
    {
        return aznew BlackboardPropertyNil(name ? name : mName.c_str());
    }

    void BlackboardPropertyNil::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        AZ_UNUSED(scriptProperty);
    }

    void BlackboardPropertyNil::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        AZ_UNUSED(blackboard);
    }

    void BlackboardPropertyNil::SetValueFromString(const char* value)
    {
        AZ_UNUSED(value);
    }

#pragma endregion

#pragma region BlackboardPropertyBoolean

    void BlackboardPropertyBoolean::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyBoolean, BlackboardProperty>()->Version(0)->Field("value", &BlackboardPropertyBoolean::mValue);
        }
    }

    BlackboardProperty* BlackboardPropertyBoolean::TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name)
    {
        BlackboardProperty* retVal = nullptr;

        if (const BT::Any* prop = context->getAny(name))
        {
            if (prop->type() == typeid(bool))
            {
                retVal = aznew BlackboardPropertyBoolean(name, prop->cast<bool>());
            }
        }

        return retVal;
    }

    BlackboardPropertyBoolean::BlackboardPropertyBoolean()
        : mValue(false)
    {
    }

    BlackboardPropertyBoolean::BlackboardPropertyBoolean(const char* name)
        : BlackboardProperty(name)
        , mValue(false)
    {
    }

    BlackboardPropertyBoolean::BlackboardPropertyBoolean(const char* name, bool value)
        : BlackboardProperty(name)
        , mValue(value)
    {
    }

    BlackboardPropertyBoolean* BlackboardPropertyBoolean::Clone(const char* name) const
    {
        return aznew BlackboardPropertyBoolean(name ? name : mName.c_str(), mValue);
    }

    const void* BlackboardPropertyBoolean::GetDataAddress() const
    {
        return &mValue;
    }

    const AZ::Uuid& BlackboardPropertyBoolean::GetDataTypeUuid() const
    {
        return azrtti_typeid<bool>();
    }

    void BlackboardPropertyBoolean::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        const auto* booleanProperty = azrtti_cast<const BlackboardPropertyBoolean*>(scriptProperty);

        AZ_Error(
            "BlackboardPropertyBoolean", booleanProperty, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (booleanProperty)
        {
            mValue = booleanProperty->mValue;
        }
    }

    void BlackboardPropertyBoolean::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<bool>(mName.c_str(), mValue);
    }

    void BlackboardPropertyBoolean::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<bool>(value);
    }

#pragma endregion

#pragma region BlackboardPropertyNumber

    void BlackboardPropertyNumber::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyNumber, BlackboardProperty>()->Version(0)->Field("value", &BlackboardPropertyNumber::mValue);
        }
    }

    BlackboardProperty* BlackboardPropertyNumber::TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name)
    {
        BlackboardProperty* retVal = nullptr;

        if (const BT::Any* prop = context->getAny(name))
        {
            if (prop->isNumber())
            {
                retVal = aznew BlackboardPropertyNumber(name, prop->cast<double>());
            }
        }

        return retVal;
    }

    BlackboardPropertyNumber::BlackboardPropertyNumber()
        : mValue(0.0)
    {
    }

    BlackboardPropertyNumber::BlackboardPropertyNumber(const char* name)
        : BlackboardProperty(name)
        , mValue(0.0)
    {
    }

    BlackboardPropertyNumber::BlackboardPropertyNumber(const char* name, double value)
        : BlackboardProperty(name)
        , mValue(value)
    {
    }

    const void* BlackboardPropertyNumber::GetDataAddress() const
    {
        return &mValue;
    }

    BlackboardPropertyNumber* BlackboardPropertyNumber::Clone(const char* name) const
    {
        return aznew BlackboardPropertyNumber(name ? name : mName.c_str(), mValue);
    }

    const AZ::Uuid& BlackboardPropertyNumber::GetDataTypeUuid() const
    {
        return azrtti_typeid<double>();
    }

    void BlackboardPropertyNumber::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        const auto* numberProperty = azrtti_cast<const BlackboardPropertyNumber*>(scriptProperty);

        AZ_Error("BlackboardPropertyNumber", numberProperty, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (numberProperty)
        {
            mValue = numberProperty->mValue;
        }
    }

    void BlackboardPropertyNumber::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<double>(mName.c_str(), mValue);
    }

    void BlackboardPropertyNumber::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<double>(value);
    }

#pragma endregion

#pragma region BlackboardPropertyString

    void BlackboardPropertyString::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyString, BlackboardProperty>()->Version(0)->Field("value", &BlackboardPropertyString::mValue);
        }
    }

    BlackboardProperty* BlackboardPropertyString::TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name)
    {
        BlackboardProperty* retVal = nullptr;

        if (const BT::Any* prop = context->getAny(name))
        {
            if (prop->isString())
            {
                retVal = aznew BlackboardPropertyString(name, prop->cast<std::string>().c_str());
            }
        }

        return retVal;
    }

    BlackboardPropertyString::BlackboardPropertyString(const char* name)
        : BlackboardProperty(name)
    {
    }

    BlackboardPropertyString::BlackboardPropertyString(const char* name, const char* value)
        : BlackboardProperty(name)
        , mValue(value)
    {
    }

    const void* BlackboardPropertyString::GetDataAddress() const
    {
        return &mValue;
    }

    BlackboardPropertyString* BlackboardPropertyString::Clone(const char* name) const
    {
        return aznew BlackboardPropertyString(name ? name : mName.c_str(), mValue.c_str());
    }

    const AZ::Uuid& BlackboardPropertyString::GetDataTypeUuid() const
    {
        return azrtti_typeid<AZStd::string>();
    }

    void BlackboardPropertyString::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        const auto* stringProperty = azrtti_cast<const BlackboardPropertyString*>(scriptProperty);

        AZ_Error("BlackboardPropertyString", stringProperty, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (stringProperty)
        {
            mValue = stringProperty->mValue;
        }
    }

    void BlackboardPropertyString::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<AZStd::string>(mName.c_str(), mValue);
    }

    void BlackboardPropertyString::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<AZStd::string>(value);
    }

#pragma endregion

#pragma region BlackboardPropertyEntityRef

    void BlackboardPropertyEntityRef::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyEntityRef, BlackboardProperty>()->Version(0)->Field("value", &BlackboardPropertyEntityRef::mValue);
        }
    }

    BlackboardPropertyEntityRef::BlackboardPropertyEntityRef(const char* name)
        : BlackboardProperty(name)
    {
    }

    const void* BlackboardPropertyEntityRef::GetDataAddress() const
    {
        return &mValue;
    }

    const AZ::Uuid& BlackboardPropertyEntityRef::GetDataTypeUuid() const
    {
        return azrtti_typeid<AZ::EntityId>();
    }

    BlackboardPropertyEntityRef* BlackboardPropertyEntityRef::Clone(const char* name) const
    {
        auto* clonedValue = aznew BlackboardPropertyEntityRef(name ? name : mName.c_str());
        clonedValue->mValue = mValue;
        return clonedValue;
    }

    void BlackboardPropertyEntityRef::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        const auto* entityProperty = azrtti_cast<const BlackboardPropertyEntityRef*>(scriptProperty);

        AZ_Error(
            "BlackboardPropertyEntityRef", entityProperty, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (entityProperty)
        {
            mValue = entityProperty->mValue;
        }
    }

    void BlackboardPropertyEntityRef::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<AZ::EntityId>(mName.c_str(), mValue);
    }

    void BlackboardPropertyEntityRef::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<AZ::EntityId>(value);
    }

#pragma endregion

#pragma region BlackboardPropertyVector2

    void BlackboardPropertyVector2::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<BlackboardPropertyVector2, BlackboardProperty>()->Version(0)->Field("value", &BlackboardPropertyVector2::mValue);
        }
    }

    BlackboardPropertyVector2::BlackboardPropertyVector2(const char* name)
        : BlackboardProperty(name)
    {
    }

    const void* BlackboardPropertyVector2::GetDataAddress() const
    {
        return &mValue;
    }

    const AZ::Uuid& BlackboardPropertyVector2::GetDataTypeUuid() const
    {
        return azrtti_typeid<AZ::Vector2>();
    }

    BlackboardPropertyVector2* BlackboardPropertyVector2::Clone(const char* name) const
    {
        auto* clonedValue = aznew BlackboardPropertyVector2(name ? name : mName.c_str());
        clonedValue->mValue = mValue;
        return clonedValue;
    }

    void BlackboardPropertyVector2::CloneDataFrom(const BlackboardProperty* scriptProperty)
    {
        const auto* entityProperty = azrtti_cast<const BlackboardPropertyVector2*>(scriptProperty);

        AZ_Error(
            "BlackboardPropertyVector2", entityProperty, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (entityProperty)
        {
            mValue = entityProperty->mValue;
        }
    }

    void BlackboardPropertyVector2::AddBlackboardEntry(const Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<AZ::Vector2>(mName.c_str(), mValue);
    }

    void BlackboardPropertyVector2::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<AZ::Vector2>(value);
    }

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

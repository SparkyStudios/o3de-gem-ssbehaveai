#include <StdAfx.h>

#include <Blackboard/SSBehaviorTreeBlackboard.h>

#include <AzCore/RTTI/BehaviorContext.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
#pragma region SSBehaviorTreeBlackboard

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeBlackboard::GetProperty(const char* propertyName)
    {
        for (SSBehaviorTreeBlackboardProperty* prop : m_properties)
        {
            if (prop->m_name == propertyName)
            {
                return prop;
            }
        }

        return nullptr;
    }

    void SSBehaviorTreeBlackboard::Clear()
    {
        for (SSBehaviorTreeBlackboardProperty* prop : m_properties)
        {
            delete prop;
        }

        m_properties.clear();
        // m_blackboard.swap(BT::Blackboard::create());
    }

    SSBehaviorTreeBlackboard::~SSBehaviorTreeBlackboard()
    {
        Clear();
    }

    SSBehaviorTreeBlackboard& SSBehaviorTreeBlackboard::operator=(SSBehaviorTreeBlackboard&& rhs) noexcept
    {
        m_name.swap(rhs.m_name);
        m_properties.swap(rhs.m_properties);
        m_blackboard.swap(rhs.m_blackboard);

        return *this;
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyNil

    void SSBehaviorTreeBlackboardPropertyNil::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyNil, SSBehaviorTreeBlackboardProperty>()
                ->Version(1)
                ->SerializeWithNoData();
        }
    }

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeBlackboardPropertyNil::TryCreateProperty(BT::Blackboard::Ptr context, const char* name)
    {
        SSBehaviorTreeBlackboardProperty* retVal = nullptr;

        if (BT::Any* prop = context->getAny(name))
        {
            if (prop->empty())
            {
                retVal = aznew SSBehaviorTreeBlackboardPropertyNil(name);
            }
        }

        return retVal;
    }

    const void* SSBehaviorTreeBlackboardPropertyNil::GetDataAddress() const
    {
        return nullptr;
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyNil::GetDataTypeUuid() const
    {
        return AZ::SerializeTypeInfo<void*>::GetUuid();
    }

    SSBehaviorTreeBlackboardPropertyNil* SSBehaviorTreeBlackboardPropertyNil::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyNil(name ? name : m_name.c_str());
    }

    void SSBehaviorTreeBlackboardPropertyNil::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        (void)scriptProperty;
    }

    void SSBehaviorTreeBlackboardPropertyNil::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        AZ_UNUSED(blackboard);
    }

    void SSBehaviorTreeBlackboardPropertyNil::SetValueFromString(const char* value)
    {
        AZ_UNUSED(value);
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyBoolean

    void SSBehaviorTreeBlackboardPropertyBoolean::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyBoolean, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyBoolean::m_value);
        }
    }

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeBlackboardPropertyBoolean::TryCreateProperty(
        BT::Blackboard::Ptr context, const char* name)
    {
        SSBehaviorTreeBlackboardProperty* retVal = nullptr;

        if (BT::Any* prop = context->getAny(name))
        {
            if (prop->type() == typeid(bool))
            {
                retVal = aznew SSBehaviorTreeBlackboardPropertyBoolean(name, prop->cast<bool>());
            }
        }

        return retVal;
    }

    SSBehaviorTreeBlackboardPropertyBoolean* SSBehaviorTreeBlackboardPropertyBoolean::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyBoolean(name ? name : m_name.c_str(), m_value);
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyBoolean::GetDataTypeUuid() const
    {
        return AZ::SerializeTypeInfo<bool>::GetUuid();
    }

    void SSBehaviorTreeBlackboardPropertyBoolean::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        const SSBehaviorTreeBlackboardPropertyBoolean* booleanProperty =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyBoolean*>(scriptProperty);
        AZ_Error(
            "SSBehaviorTreeBlackboardPropertyBoolean", booleanProperty,
            "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (booleanProperty)
        {
            m_value = booleanProperty->m_value;
        }
    }

    void SSBehaviorTreeBlackboardPropertyBoolean::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<bool>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyBoolean::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<bool>(value);
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyNumber

    void SSBehaviorTreeBlackboardPropertyNumber::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyNumber, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyNumber::m_value);
        }
    }

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeBlackboardPropertyNumber::TryCreateProperty(
        BT::Blackboard::Ptr context, const char* name)
    {
        SSBehaviorTreeBlackboardProperty* retVal = nullptr;

        if (BT::Any* prop = context->getAny(name))
        {
            if (prop->isNumber())
            {
                retVal = aznew SSBehaviorTreeBlackboardPropertyNumber(name, prop->cast<double>());
            }
        }

        return retVal;
    }

    SSBehaviorTreeBlackboardPropertyNumber* SSBehaviorTreeBlackboardPropertyNumber::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyNumber(name ? name : m_name.c_str(), m_value);
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyNumber::GetDataTypeUuid() const
    {
        return AZ::SerializeTypeInfo<double>::GetUuid();
    }

    void SSBehaviorTreeBlackboardPropertyNumber::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        const SSBehaviorTreeBlackboardPropertyNumber* numberProperty =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyNumber*>(scriptProperty);
        AZ_Error(
            "SSBehaviorTreeBlackboardPropertyNumber", numberProperty,
            "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (numberProperty)
        {
            m_value = numberProperty->m_value;
        }
    }

    void SSBehaviorTreeBlackboardPropertyNumber::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<double>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyNumber::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<double>(value);
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyString

    void SSBehaviorTreeBlackboardPropertyString::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyString, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyString::m_value);
        }
    }

    SSBehaviorTreeBlackboardProperty* SSBehaviorTreeBlackboardPropertyString::TryCreateProperty(
        BT::Blackboard::Ptr context, const char* name)
    {
        SSBehaviorTreeBlackboardProperty* retVal = nullptr;

        if (BT::Any* prop = context->getAny(name))
        {
            if (prop->isString())
            {
                retVal = aznew SSBehaviorTreeBlackboardPropertyString(name, prop->cast<std::string>().c_str());
            }
        }

        return retVal;
    }

    SSBehaviorTreeBlackboardPropertyString* SSBehaviorTreeBlackboardPropertyString::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyString(name ? name : m_name.c_str(), m_value.c_str());
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyString::GetDataTypeUuid() const
    {
        return AZ::SerializeGenericTypeInfo<AZStd::string>::GetClassTypeId();
    }

    void SSBehaviorTreeBlackboardPropertyString::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        const SSBehaviorTreeBlackboardPropertyString* stringProperty =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyString*>(scriptProperty);
        AZ_Error(
            "SSBehaviorTreeBlackboardPropertyString", stringProperty,
            "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (stringProperty)
        {
            m_value = stringProperty->m_value;
        }
    }

    void SSBehaviorTreeBlackboardPropertyString::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<AZStd::string>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyString::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<AZStd::string>(value);
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyEntityRef

    void SSBehaviorTreeBlackboardPropertyEntityRef::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyEntityRef, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyEntityRef::m_value);
        }
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyEntityRef::GetDataTypeUuid() const
    {
        return AZ::SerializeTypeInfo<AZ::EntityId>::GetUuid();
    }

    SSBehaviorTreeBlackboardPropertyEntityRef* SSBehaviorTreeBlackboardPropertyEntityRef::Clone(const char* name) const
    {
        SSBehaviorTreeBlackboardPropertyEntityRef* clonedValue =
            aznew SSBehaviorTreeBlackboardPropertyEntityRef(name ? name : m_name.c_str());
        clonedValue->m_value = m_value;
        return clonedValue;
    }

    void SSBehaviorTreeBlackboardPropertyEntityRef::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        const SSBehaviorTreeBlackboardPropertyEntityRef* entityProperty =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyEntityRef*>(scriptProperty);

        AZ_Error(
            "SSBehaviorTreeBlackboardPropertyEntityRef", entityProperty,
            "Invalid call to CloneData. Types must match before clone attempt is made.\n");
        if (entityProperty)
        {
            m_value = entityProperty->m_value;
        }
    }

    void SSBehaviorTreeBlackboardPropertyEntityRef::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<AZ::EntityId>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyEntityRef::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<AZ::EntityId>(value);
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyVector2

    void SSBehaviorTreeBlackboardPropertyVector2::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);

        if (serializeContext)
        {
            serializeContext->Class<SSBehaviorTreeBlackboardPropertyVector2, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyVector2::m_value);
        }
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyVector2::GetDataTypeUuid() const
    {
        return AZ::SerializeTypeInfo<AZ::Vector2>::GetUuid();
    }

    SSBehaviorTreeBlackboardPropertyVector2* SSBehaviorTreeBlackboardPropertyVector2::Clone(const char* name) const
    {
        SSBehaviorTreeBlackboardPropertyVector2* clonedValue = aznew SSBehaviorTreeBlackboardPropertyVector2(name ? name : m_name.c_str());
        clonedValue->m_value = m_value;
        return clonedValue;
    }

    void SSBehaviorTreeBlackboardPropertyVector2::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty)
    {
        const SSBehaviorTreeBlackboardPropertyVector2* entityProperty =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyVector2*>(scriptProperty);

        AZ_Error(
            "SSBehaviorTreeBlackboardPropertyVector2", entityProperty,
            "Invalid call to CloneData. Types must match before clone attempt is made.\n");
        if (entityProperty)
        {
            m_value = entityProperty->m_value;
        }
    }

    void SSBehaviorTreeBlackboardPropertyVector2::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<AZ::Vector2>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyVector2::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<AZ::Vector2>(value);
    }

#pragma endregion
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard
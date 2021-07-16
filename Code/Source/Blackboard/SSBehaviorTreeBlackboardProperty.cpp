#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    SSBehaviorTreeBlackboardProperty::SSBehaviorTreeBlackboardProperty()
    {
    }

    SSBehaviorTreeBlackboardProperty::SSBehaviorTreeBlackboardProperty(const char* name)
        : m_id(AZ::Crc32(name))
        , m_name(name)
    {
    }

    void SSBehaviorTreeBlackboardProperty::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<SSBehaviorTreeBlackboardProperty>()
                ->Version(1)
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

    bool SSBehaviorTreeBlackboardProperty::IsNil()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<void*>::GetUuid();
    }

    bool SSBehaviorTreeBlackboardProperty::IsBoolean()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<bool>::GetUuid();
    }

    bool SSBehaviorTreeBlackboardProperty::IsNumber()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<double>::GetUuid();
    }

    bool SSBehaviorTreeBlackboardProperty::IsString()
    {
        return GetDataTypeUuid() == AZ::SerializeGenericTypeInfo<AZStd::string>::GetClassTypeId();
    }

    bool SSBehaviorTreeBlackboardProperty::IsEntity()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::EntityId>::GetUuid();
    }

    bool SSBehaviorTreeBlackboardProperty::IsVector2()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::Vector2>::GetUuid();
    }

    bool SSBehaviorTreeBlackboardProperty::IsVector3()
    {
        return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::Vector3>::GetUuid();
    }
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

#include <Blackboard/SSBehaviorTreeBlackboard.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
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
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

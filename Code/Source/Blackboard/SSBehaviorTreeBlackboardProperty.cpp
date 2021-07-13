#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    SSBehaviorTreeBlackboardProperty::SSBehaviorTreeBlackboardProperty()
    {
    }

    SSBehaviorTreeBlackboardProperty::SSBehaviorTreeBlackboardProperty(const char* name)
        : id(AZ::Crc32(name))
        , name(name)
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
                        return reinterpret_cast<const SSBehaviorTreeBlackboardProperty*>(instance)->id;
                    })
                ->Field("ID", &SSBehaviorTreeBlackboardProperty::id)
                ->Field("Name", &SSBehaviorTreeBlackboardProperty::name)
                ->Field("Description", &SSBehaviorTreeBlackboardProperty::description)
                ->Field("Visibility", &SSBehaviorTreeBlackboardProperty::visibility)
                ->Field("Suffix", &SSBehaviorTreeBlackboardProperty::suffix);
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

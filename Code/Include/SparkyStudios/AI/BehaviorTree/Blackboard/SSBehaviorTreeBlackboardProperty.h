#pragma once

#include <behaviortree_cpp_v3/bt_factory.h>

#include <AzCore/Math/Uuid.h>
#include <AzCore/Math/Vector2.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <AzFramework/StringFunc/StringFunc.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    class SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_RTTI(SSBehaviorTreeBlackboardProperty, "{bf33f432-5c02-4380-8ace-28eda6371afd}");

        SSBehaviorTreeBlackboardProperty()
        {
        }

        SSBehaviorTreeBlackboardProperty(const char* name)
            : m_id(AZ::Crc32(name))
            , m_name(name)
        {
        }

        virtual ~SSBehaviorTreeBlackboardProperty()
        {
        }

        static void Reflect(AZ::ReflectContext* context)
        {
            if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<SSBehaviorTreeBlackboardProperty>()
                    ->Version(1)
                    ->PersistentId(
                        [](const void* instance) -> AZ::u64
                        {
                            return reinterpret_cast<const SSBehaviorTreeBlackboardProperty*>(instance)->m_id;
                        })
                    ->Field("ID", &SSBehaviorTreeBlackboardProperty::m_id)
                    ->Field("Name", &SSBehaviorTreeBlackboardProperty::m_name);
            }
        }

        virtual bool DoesTypeMatch(const AZ::Uuid& type) const
        {
            return type == GetDataTypeUuid();
        }

        bool IsNil()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<void*>::GetUuid();
        }

        bool IsBoolean()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<bool>::GetUuid();
        }

        bool IsNumber()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<double>::GetUuid();
        }

        bool IsString()
        {
            return GetDataTypeUuid() == AZ::SerializeGenericTypeInfo<AZStd::string>::GetClassTypeId();
        }

        bool IsEntity()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::EntityId>::GetUuid();
        }

        bool IsVector2()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::Vector2>::GetUuid();
        }

        bool IsVector3()
        {
            return GetDataTypeUuid() == AZ::SerializeTypeInfo<AZ::Vector3>::GetUuid();
        }

        virtual bool IsEnum() const
        {
            return false;
        }

        virtual const void* GetDataAddress() const = 0;
        virtual const AZ::Uuid& GetDataTypeUuid() const = 0;

        virtual SSBehaviorTreeBlackboardProperty* Clone(const char* name = nullptr) const = 0;

        virtual void AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const = 0;
        virtual void SetValueFromString(const char* value) = 0;

        AZ::u64 m_id;

        AZStd::string m_name;
        AZStd::string m_suffix;
        AZStd::string m_description;

        AZ::Crc32 m_visibility;

    protected:
        virtual void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) = 0;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

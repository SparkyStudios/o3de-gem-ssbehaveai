#pragma once

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

#include <Assets/SSBehaviorTreeAsset.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Math/Vector2.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/function/function_template.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace BT
{
    // ====================================================================================================
    // AZ::Vector2 conversion

    template<>
    inline std::string toStr<AZ::Vector2>(AZ::Vector2 value)
    {
        return (AZStd::to_string(value.GetX()) + "," + AZStd::to_string(value.GetY())).c_str();
    }

    template<>
    inline AZ::Vector2 convertFromString(StringView str)
    {
        // We expect real numbers separated by semicolons
        auto parts = splitString(str, ',');

        if (parts.size() != 2)
        {
            AZ_Warning("SSBehaviorTree", false, "Invalid AZ::Vector2 value given. Please format the value as \"float,float\"");
            return AZ::Vector2::CreateZero();
        }
        else
        {
            AZ::Vector2 output(0.0f);
            output.SetX(convertFromString<float>(parts[0]));
            output.SetY(convertFromString<float>(parts[1]));
            return output;
        }
    }

    // ====================================================================================================

    // ====================================================================================================
    // AZStd::string conversion

    template<>
    inline std::string toStr<AZStd::string>(AZStd::string value)
    {
        return value.c_str();
    }

    template<>
    inline AZStd::string convertFromString(StringView str)
    {
        return { str.cbegin(), str.cend() };
    }

    // ====================================================================================================
} // namespace BT

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
    class SSBehaviorTreeBlackboardPropertyNil : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyNil, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyNil, "{5d73a500-4c1a-4215-8097-2ef78a047fcd}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static SSBehaviorTreeBlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        SSBehaviorTreeBlackboardPropertyNil() = default;

        explicit SSBehaviorTreeBlackboardPropertyNil(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
        {
        }

        const void* GetDataAddress() const override;
        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyNil* Clone(const char* name) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };

    class SSBehaviorTreeBlackboardPropertyBoolean : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyBoolean, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyBoolean, "{07ca41eb-9ca2-4d89-90de-9bee15fd2be7}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static SSBehaviorTreeBlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        SSBehaviorTreeBlackboardPropertyBoolean()
            : m_value(false)
        {
        }

        explicit SSBehaviorTreeBlackboardPropertyBoolean(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
            , m_value(false)
        {
        }

        SSBehaviorTreeBlackboardPropertyBoolean(const char* name, bool value)
            : SSBehaviorTreeBlackboardProperty(name)
            , m_value(value)
        {
        }

        const void* GetDataAddress() const override
        {
            return &m_value;
        }

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyBoolean* Clone(const char* name) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        bool m_value;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };

    class SSBehaviorTreeBlackboardPropertyNumber : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyNumber, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyNumber, "{8dbed5bd-9bc0-4890-9d9d-63f42fc17351}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static SSBehaviorTreeBlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        SSBehaviorTreeBlackboardPropertyNumber()
            : m_value(0.0f)
        {
        }

        explicit SSBehaviorTreeBlackboardPropertyNumber(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
            , m_value(0.0f)
        {
        }

        SSBehaviorTreeBlackboardPropertyNumber(const char* name, double value)
            : SSBehaviorTreeBlackboardProperty(name)
            , m_value(value)
        {
        }

        const void* GetDataAddress() const override
        {
            return &m_value;
        }

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyNumber* Clone(const char* name) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        double m_value;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };

    class SSBehaviorTreeBlackboardPropertyString : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyString, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyString, "{89bcffdc-f977-40b3-bcbf-57660eb8dfa5}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static SSBehaviorTreeBlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        SSBehaviorTreeBlackboardPropertyString() = default;

        explicit SSBehaviorTreeBlackboardPropertyString(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
        {
        }

        SSBehaviorTreeBlackboardPropertyString(const char* name, const char* value)
            : SSBehaviorTreeBlackboardProperty(name)
            , m_value(value)
        {
        }

        const void* GetDataAddress() const override
        {
            return &m_value;
        }

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyString* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZStd::string m_value;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };

    class SSBehaviorTreeBlackboardPropertyEntityRef : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyEntityRef, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyEntityRef, "{4389616a-10f4-46e1-9e67-7b31b80b38af}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeBlackboardPropertyEntityRef() = default;

        explicit SSBehaviorTreeBlackboardPropertyEntityRef(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
        {
        }

        ~SSBehaviorTreeBlackboardPropertyEntityRef() override = default;

        const void* GetDataAddress() const override
        {
            return &m_value;
        }

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyEntityRef* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZ::EntityId m_value;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };

    class SSBehaviorTreeBlackboardPropertyVector2 : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyVector2, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeBlackboardPropertyVector2, "{3d9f3df3-c4b0-4361-a518-c7f066fe0042}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeBlackboardPropertyVector2() = default;

        explicit SSBehaviorTreeBlackboardPropertyVector2(const char* name)
            : SSBehaviorTreeBlackboardProperty(name)
        {
        }

        ~SSBehaviorTreeBlackboardPropertyVector2() override = default;

        const void* GetDataAddress() const override
        {
            return &m_value;
        }

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyVector2* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const SSBehaviorTreeBlackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZ::Vector2 m_value = AZ::Vector2::CreateZero();

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* scriptProperty) override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

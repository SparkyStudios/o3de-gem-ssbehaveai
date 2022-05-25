#pragma once

#include <StdAfx.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Math/Vector2.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
    /**
     * @brief The behavior tree blackboard.
     * This struct contains all the data used by the behavior tree,
     * as a map of key-value pairs.
     */
    struct Blackboard
    {
        AZ_TYPE_INFO(Blackboard, "{53B0FF7E-D508-4E7D-BA14-25F0099EAACA}");

        /**
         * @brief The blackboard name.
         */
        AZStd::string mName = "Blackboard";

        /**
         * @brief The blackboard properties.
         */
        AZStd::vector<BlackboardProperty*> mProperties;

        /**
         * @brief The native blackboard object.
         */
        BT::Blackboard::Ptr mBlackboard = BT::Blackboard::create();

        /**
         * @brief Get the pointer to the specified property. Returns nullptr if not found.
         */
        BlackboardProperty* GetProperty(const char* propertyName);

        /**
         * @brief Remove all properties.
         */
        void Clear();

        Blackboard() = default;
        Blackboard(const Blackboard& rhs) = delete;
        Blackboard(Blackboard&& rhs) noexcept;

        ~Blackboard();

        Blackboard& operator=(Blackboard&& rhs) noexcept;
        Blackboard& operator=(Blackboard&) = delete;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

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
        if (const auto parts = splitString(str, ','); parts.size() != 2)
        {
            AZ_Warning("BehaveAI [BehaviorTree]", false, "Invalid AZ::Vector2 value given. Please format the value as \"float,float\"");
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
    class BlackboardPropertyNil final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyNil, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyNil, "{5D73A500-4C1A-4215-8097-2EF78A047FCD}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static BlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        BlackboardPropertyNil() = default;
        explicit BlackboardPropertyNil(const char* name);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyNil* Clone(const char* name) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };

    class BlackboardPropertyBoolean final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyBoolean, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyBoolean, "{07CA41EB-9CA2-4D89-90DE-9BEE15FD2BE7}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static BlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        BlackboardPropertyBoolean();
        explicit BlackboardPropertyBoolean(const char* name);
        BlackboardPropertyBoolean(const char* name, bool value);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyBoolean* Clone(const char* name) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        bool mValue;

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };

    class BlackboardPropertyNumber final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyNumber, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyNumber, "{8DBED5BD-9BC0-4890-9D9D-63F42FC17351}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static BlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        BlackboardPropertyNumber();
        explicit BlackboardPropertyNumber(const char* name);
        BlackboardPropertyNumber(const char* name, double value);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyNumber* Clone(const char* name) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        double mValue;

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };

    class BlackboardPropertyString final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyString, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyString, "{89BCFFDC-F977-40B3-BCBF-57660EB8DFA5}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);
        static BlackboardProperty* TryCreateProperty(const BT::Blackboard::Ptr& context, const char* name);

        BlackboardPropertyString() = default;
        explicit BlackboardPropertyString(const char* name);
        BlackboardPropertyString(const char* name, const char* value);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyString* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZStd::string mValue;

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };

    class BlackboardPropertyEntityRef final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyEntityRef, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyEntityRef, "{4389616A-10F4-46E1-9E67-7B31B80B38AF}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);

        BlackboardPropertyEntityRef() = default;
        explicit BlackboardPropertyEntityRef(const char* name);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyEntityRef* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZ::EntityId mValue;

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };

    class BlackboardPropertyVector2 final : public BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyVector2, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyVector2, "{3D9F3DF3-C4B0-4361-A518-C7F066FE0042}", BlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);

        BlackboardPropertyVector2() = default;
        explicit BlackboardPropertyVector2(const char* name);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyVector2* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        AZ::Vector2 mValue = AZ::Vector2::CreateZero();

    protected:
        void CloneDataFrom(const BlackboardProperty* scriptProperty) override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

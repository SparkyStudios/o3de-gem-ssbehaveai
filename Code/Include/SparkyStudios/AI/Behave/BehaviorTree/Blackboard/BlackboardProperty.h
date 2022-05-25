#pragma once

#include <StdAfx.h>

#include <AzCore/Math/Uuid.h>
#include <AzCore/Math/Vector2.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard
{
    struct Blackboard;

    /**
     * @brief A wrapper around a value in the behavior tree blackboard.
     */
    class BlackboardProperty
    {
    public:
        AZ_RTTI(BlackboardProperty, "{BF33F432-5C02-4380-8ACE-28EDA6371AFD}");

        /**
         * @brief Construct a new BlackboardProperty. This is the default constructor,
         * mainly used for RTTI purposes.
         *
         * @warning Don't use this constructor directly. Use the BlackboardProperty(const char* name)
         * constructor instead.
         */
        BlackboardProperty() = default;

        /**
         * @brief Construct a new BlackboardProperty with the given name.
         *
         * @param name The property name in the blackboard.
         */
        explicit BlackboardProperty(const char* name);

        /**
         * @brief Destroy the BlackboardProperty.
         */
        virtual ~BlackboardProperty() = default;

        /**
         * @brief Reflect this class in the given O3DE ReflectContext.
         * Used for serialization purposes.
         *
         * @param context The ReflectContext in which describe this class.
         */
        static void Reflect(AZ::ReflectContext* context);

        /**
         * @brief Check if this property wraps a null value.
         *
         * @return true When this property wraps a null value.
         * @return false When this property does not wrap a null value.
         */
        [[nodiscard]] bool IsNil() const;

        /**
         * @brief Check if this property wraps a boolean value.
         *
         * @return true When this property wraps a boolean value.
         * @return false When this property does not wrap a boolean value.
         */
        [[nodiscard]] bool IsBoolean() const;

        /**
         * @brief Check if this property wraps a number value.
         *
         * @return true When this property wraps a number value.
         * @return false When this property does not wrap a number value.
         */
        [[nodiscard]] bool IsNumber() const;

        /**
         * @brief Check if this property wraps a string value.
         *
         * @return true When this property wraps a string value.
         * @return false When this property does not wrap a string value.
         */
        [[nodiscard]] bool IsString() const;

        /**
         * @brief Check if this property wraps an entity ID value.
         *
         * @return true When this property wraps an entity ID value.
         * @return false When this property does not wrap an entity ID value.
         */
        [[nodiscard]] bool IsEntity() const;

        /**
         * @brief Check if this property wraps a Vector2 value.
         *
         * @return true When this property wraps a Vector2 value.
         * @return false When this property does not wrap a Vector2 value.
         */
        [[nodiscard]] bool IsVector2() const;

        /**
         * @brief Check if this property wraps a Vector3 value.
         *
         * @return true When this property wraps a Vector3 value.
         * @return false When this property does not wrap a Vector3 value.
         */
        [[nodiscard]] bool IsVector3() const;

        /**
         * @brief Check if this property wraps an enumerated value.
         *
         * @return true When this property wraps an enumerated value.
         * @return false When this property does not wrap an enumerated value.
         */
        [[nodiscard]] virtual bool IsEnum() const
        {
            return false;
        }

        /**
         * @brief Check if the UUID of the wrapped value's type matches the given UUID.
         *
         * @param type The UUID to compare with the wrapped value's type UUID.
         * @return true When the type of the wrapped value matches the given one.
         * @return false When the type of the wrapped value does not matches the given one.
         */
        [[nodiscard]] virtual bool DoesTypeMatch(const AZ::Uuid& type) const
        {
            return type == GetDataTypeUuid();
        }

        /**
         * @brief Get a pointer to the wrapped value.
         *
         * @return const void*
         */
        [[nodiscard]] virtual const void* GetDataAddress() const = 0;

        /**
         * @brief Get the UUID of the wrapped value's type.
         *
         * @return const AZ::Uuid&
         */
        [[nodiscard]] virtual const AZ::Uuid& GetDataTypeUuid() const = 0;

        /**
         * @brief Clone this instance to create a new one with a possibly new name.
         *
         * @param name The name of the copied instance. If empty, the same name is kept.
         * @return BlackboardProperty*
         */
        virtual BlackboardProperty* Clone(const char* name = nullptr) const = 0;

        /**
         * @brief Add this property into the given blackboard.
         *
         * @param blackboard The blackboard in which add this property.
         */
        virtual void AddBlackboardEntry(const Blackboard& blackboard) const = 0;

        /**
         * @brief Try to convert the given string into a value supported by this property.
         * If the conversion succeeds, the result is stored in this property.
         *
         * @param value The value to convert and store, mostly coming from the behavior tree's blackboard.
         */
        virtual void SetValueFromString(const char* value) = 0;

        /**
         * @brief The unique ID of this property.
         */
        AZ::u64 mId;

        /**
         * @brief The name of this property in the behavior tree's blackboard.
         */
        AZStd::string mName;

        /**
         * @brief The suffix to display in the editor for this property.
         */
        AZStd::string mSuffix;

        /**
         * @brief The property's description. Also used by the editor.
         */
        AZStd::string mDescription;

        /**
         * @brief Defines if this property is visible in the editor or not.
         */
        AZ::Crc32 mVisibility;

        /**
         * @brief Defines the sort order of the property. Defaults to FLT_MAX.
         */
        float mOrder = FLT_MAX;

    protected:
        /**
         * @brief Clone the data from the given property, and store the copied value in this property.
         *
         * @param property The property to clone data from.
         */
        virtual void CloneDataFrom(const BlackboardProperty* property) = 0;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Blackboard

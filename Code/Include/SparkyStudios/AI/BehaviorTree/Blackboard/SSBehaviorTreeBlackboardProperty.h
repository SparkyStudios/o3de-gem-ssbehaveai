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
    /**
     * @brief A wrapper around a value in the behavior tree blackboard.
     */
    class SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_RTTI(SSBehaviorTreeBlackboardProperty, "{bf33f432-5c02-4380-8ace-28eda6371afd}");

        /**
         * @brief Construct a new SSBehaviorTreeBlackboardProperty. This is the default constructor,
         * mainly used for RTTI purposes.
         * @warning Doesn't use this constructor directly. Use the SSBehaviorTreeBlackboardProperty(const char* name)
         * constructor instead.
         */
        SSBehaviorTreeBlackboardProperty();

        /**
         * @brief Construct a new SSBehaviorTreeBlackboardProperty with the given name.
         *
         * @param name The property name in the blackboard.
         */
        SSBehaviorTreeBlackboardProperty(const char* name);

        /**
         * @brief Destroy the SSBehaviorTreeBlackboardProperty.
         */
        virtual ~SSBehaviorTreeBlackboardProperty()
        {
        }

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
        bool IsNil();

        /**
         * @brief Check if this property wraps a boolean value.
         *
         * @return true When this property wraps a boolean value.
         * @return false When this property does not wrap a boolean value.
         */
        bool IsBoolean();

        /**
         * @brief Check if this property wraps a number value.
         *
         * @return true When this property wraps a number value.
         * @return false When this property does not wrap a number value.
         */
        bool IsNumber();

        /**
         * @brief Check if this property wraps a string value.
         *
         * @return true When this property wraps a string value.
         * @return false When this property does not wrap a string value.
         */
        bool IsString();

        /**
         * @brief Check if this property wraps an entity ID value.
         *
         * @return true When this property wraps an entity ID value.
         * @return false When this property does not wrap an entity ID value.
         */
        bool IsEntity();

        /**
         * @brief Check if this property wraps a Vector2 value.
         *
         * @return true When this property wraps a Vector2 value.
         * @return false When this property does not wrap a Vector2 value.
         */
        bool IsVector2();

        /**
         * @brief Check if this property wraps a Vector3 value.
         *
         * @return true When this property wraps a Vector3 value.
         * @return false When this property does not wrap a Vector3 value.
         */
        bool IsVector3();

        /**
         * @brief Check if this property wraps an enumerated value.
         *
         * @return true When this property wraps an enumerated value.
         * @return false When this property does not wrap an enumerated value.
         */
        virtual bool IsEnum() const
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
        virtual bool DoesTypeMatch(const AZ::Uuid& type) const
        {
            return type == GetDataTypeUuid();
        }

        /**
         * @brief Get a pointer to the wrapped value.
         *
         * @return const void*
         */
        virtual const void* GetDataAddress() const = 0;

        /**
         * @brief Get the UUID of the wrapped value's type.
         *
         * @return const AZ::Uuid&
         */
        virtual const AZ::Uuid& GetDataTypeUuid() const = 0;

        /**
         * @brief Clone this instance to create a new one with a possibly new name.
         *
         * @param name The name of the copied instance. If empty, the same name is kept.
         * @return SSBehaviorTreeBlackboardProperty*
         */
        virtual SSBehaviorTreeBlackboardProperty* Clone(const char* name = nullptr) const = 0;

        /**
         * @brief Add this property into the given blackboard.
         *
         * @param blackboard The blackboard in which add this property.
         */
        virtual void AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const = 0;

        /**
         * @brief Try to convert the given string into a value supported by this property.
         * If the conversion succeeds, the result is stored in this property.
         *
         * @param value The value to convert and store, mostly comming from the behavior tree's blackboard.
         */
        virtual void SetValueFromString(const char* value) = 0;

        /**
         * @brief The unique ID of this property.
         */
        AZ::u64 id;

        /**
         * @brief The name of this property in the behavior tree's blackboard.
         */
        AZStd::string name;

        /**
         * @brief The suffix to display in the editor for this property.
         */
        AZStd::string suffix;

        /**
         * @brief The property's description. Also used by the editor.
         */
        AZStd::string description;

        /**
         * @brief Defines if this property is visible in the editor or not.
         */
        AZ::Crc32 visibility;

    protected:
        /**
         * @brief Clone the data from the given property, and store the copied value in this property.
         *
         * @param property The property to clone data from.
         */
        virtual void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property) = 0;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

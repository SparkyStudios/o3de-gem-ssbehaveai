#pragma once

#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>

#include <AzCore/Component/Component.h>

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    /**
     * @brief The behavior tree blackboard.
     * This struct contains all the data used by the behavior tree,
     * as a map of key-value pairs.
     */
    struct SSBehaviorTreeBlackboard
    {
        AZ_TYPE_INFO(SSBehaviorTreeBlackboard, "{53b0ff7e-d508-4e7d-ba14-25f0099eaaca}");

        AZStd::string m_name = "Blackboard";
        AZStd::vector<SSBehaviorTreeBlackboardProperty*> m_properties;
        BT::Blackboard::Ptr m_blackboard = BT::Blackboard::create();

        /**
         * @brief Get the pointer to the specified property. Returns nullptr if not found.
         */
        SSBehaviorTreeBlackboardProperty* GetProperty(const char* propertyName);

        /**
         * @brief Remove all properties.
         */
        void Clear();

        SSBehaviorTreeBlackboard() = default;
        ~SSBehaviorTreeBlackboard();

        SSBehaviorTreeBlackboard(const SSBehaviorTreeBlackboard& rhs) = delete;
        SSBehaviorTreeBlackboard& operator=(SSBehaviorTreeBlackboard&) = delete;

    public:
        SSBehaviorTreeBlackboard(SSBehaviorTreeBlackboard&& rhs)
        {
            *this = AZStd::move(rhs);
        }

        SSBehaviorTreeBlackboard& operator=(SSBehaviorTreeBlackboard&& rhs) noexcept;
    };

} // namespace SparkyStudios::AI::BehaviorTree::Blackboard

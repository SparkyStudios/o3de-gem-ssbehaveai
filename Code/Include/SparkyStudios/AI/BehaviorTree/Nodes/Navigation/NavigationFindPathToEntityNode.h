// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

#include <LmbrCentral/Ai/NavigationComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Navigation
{
#pragma region NavigationFindPathToEntityNode

    /**
     * @brief Make that entity navigate to the given target.
     *
     * Even if this class can be used itself, it should be subclassed to be
     * really usefull.
     */
    class NavigationFindPathToEntityNode
        : public Core::SSBehaviorTreeNode
        , public LmbrCentral::NavigationComponentNotificationBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(NavigationFindPathToEntityNode, AZ::SystemAllocator, 0);
        AZ_RTTI(NavigationFindPathToEntityNode, "{c8032d5e-dc02-42a8-868a-e1668c6a8eb1}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "NavigationFindPathToEntity";

        static constexpr const char* NODE_PORT_TARGET_NAME = "target";
        static constexpr const char* NODE_PORT_TARGET_DESCRIPTION = "The target entity.";

        static constexpr const char* NODE_PORT_STATE_NAME = "state";
        static constexpr const char* NODE_PORT_STATE_DESCRIPTION = "The blackboard entry in which store the navigation state.";

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        /**
         * @brief The current state of the navigation.
         *
         * - STATE_IDLE: The navigation has not started, is stopped or is cancelled.
         * - STATE_NAVIGATING: The entity is currently moving to the target.
         * - STATE_COMPLETE: The entity has successfully reached the target.
         */
        enum class NavigationState
        {
            STATE_IDLE = 0,
            STATE_NAVIGATING,
            STATE_COMPLETE
        };

        NavigationFindPathToEntityNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static Core::SSBehaviorTreePortsList providedPorts();

        const std::string NodeCategory() const override
        {
            return "Navigation";
        }

        void OnTraversalStarted(LmbrCentral::PathfindRequest::NavigationRequestId requestId) override;

        void OnTraversalPathUpdate(
            LmbrCentral::PathfindRequest::NavigationRequestId requestId,
            const AZ::Vector3& nextPathPosition,
            const AZ::Vector3& inflectionPosition) override;

        void OnTraversalInProgress(LmbrCentral::PathfindRequest::NavigationRequestId requestId, float distanceRemaining) override;

        void OnTraversalComplete(LmbrCentral::PathfindRequest::NavigationRequestId requestId) override;

        void OnTraversalCancelled(LmbrCentral::PathfindRequest::NavigationRequestId requestId) override;

    protected:
        void Start() override;

        Core::SSBehaviorTreeNodeStatus Tick() override;

        virtual void TraversalStarted();

        virtual void TraversalPathUpdate(const AZ::Vector3& nextPathPosition, const AZ::Vector3& inflectionPosition);

        virtual void TraversalInProgress(float distanceRemaining);

        virtual void TraversalComplete();

        virtual void TraversalCancelled();

        void SetNavigationState(NavigationState state);

        const AZ::EntityId& GetTarget() const;

    private:
        void ConnectBus();

        void DisconnectBus();

        void ReconnectBus();

        void StartNavigation();

        void StopNavigation();

        void RestartNavigation();

        NavigationState m_navigationState;
        LmbrCentral::PathfindRequest::NavigationRequestId m_requestId;
        AZ::EntityId m_lastTarget;
    };

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState

    class SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState : public Blackboard::SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState, AZ::SystemAllocator, 0);
        AZ_RTTI(
            SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState,
            "{a2b1a6e2-1737-4085-ad12-039908d71586}",
            Blackboard::SSBehaviorTreeBlackboardProperty);

        using NavigationState = NavigationFindPathToEntityNode::NavigationState;

        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState();

        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name);

        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name, const NavigationState& value);

        const void* GetDataAddress() const override;

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard::SSBehaviorTreeBlackboard& blackboard) const override;

        void SetValueFromString(const char* value);

        NavigationState m_value = NavigationState::STATE_IDLE;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property) override;
    };

#pragma endregion
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Navigation

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(
        SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState,
        "{ee3605c4-0a57-4642-b368-54b4f5bf96e7}");
} // namespace AZ

namespace BT
{
    template<>
    inline std::string toStr<SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState>(
        SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState value)
    {
        switch (value)
        {
        case SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_IDLE:
            return "idle";
        case SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_NAVIGATING:
            return "navigating";
        case SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_COMPLETE:
            return "complete";
        default:
            return "idle";
        }
    }

    template<>
    inline SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState convertFromString(
        StringView str)
    {
        if (str.compare("idle") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_IDLE;
        }
        else if (str.compare("navigating") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_NAVIGATING;
        }
        else if (str.compare("complete") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_COMPLETE;
        }

        return SparkyStudios::AI::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState::STATE_IDLE;
    }
} // namespace BT

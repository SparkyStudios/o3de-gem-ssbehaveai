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

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

#include <LmbrCentral/Ai/NavigationComponentBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation
{
#pragma region NavigationFindPathToEntityNode

    /**
     * @brief Make that entity navigate to the given target.
     *
     * Even if this class can be used itself, it should be inherited to be
     * really useful.
     *
     * @par Node Ports
     * - target: The target entity.
     * - state: The blackboard entry in which store the current navigation state.
     */
    class NavigationFindPathToEntityNode
        : public Core::Node
        , public LmbrCentral::NavigationComponentNotificationBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(NavigationFindPathToEntityNode, AZ::SystemAllocator, 0);
        AZ_RTTI(NavigationFindPathToEntityNode, "{C8032D5E-DC02-42A8-868A-E1668C6A8EB1}", Core::Node);

        static constexpr const char* NODE_NAME = "NavigationFindPathToEntity";

        static constexpr const char* NODE_PORT_TARGET_NAME = "target";
        static constexpr const char* NODE_PORT_TARGET_DESCRIPTION = "The target entity.";

        static constexpr const char* NODE_PORT_STATE_NAME = "state";
        static constexpr const char* NODE_PORT_STATE_DESCRIPTION = "The blackboard entry in which store the current navigation state.";

        static void Reflect(AZ::ReflectContext* rc);

        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        /**
         * @brief The current state of the navigation.
         *
         * - STATE_IDLE: The navigation has not started, is stopped or is cancelled.
         * - STATE_NAVIGATING: The entity is currently moving to the target.
         * - STATE_COMPLETE: The entity has successfully reached the target.
         */
        enum class NavigationState
        {
            Idle = 0,
            Navigating,
            Complete
        };

        NavigationFindPathToEntityNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        static Core::BehaviorTreePortsList providedPorts();

        std::string NodeCategory() const override
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

        Core::BehaviorTreeNodeStatus Tick() override;

        virtual void TraversalStarted();

        virtual void TraversalPathUpdate(const AZ::Vector3& nextPathPosition, const AZ::Vector3& inflectionPosition);

        virtual void TraversalInProgress(float distanceRemaining);

        virtual void TraversalComplete();

        virtual void TraversalCancelled();

        void SetNavigationState(const NavigationState& state);

        const AZ::EntityId& GetTarget() const;

    private:
        void ConnectBus();

        void DisconnectBus();

        void ReconnectBus();

        void StartNavigation();

        void StopNavigation();

        void RestartNavigation();

        NavigationState _navigationState;
        LmbrCentral::PathfindRequest::NavigationRequestId _requestId;
        AZ::EntityId _lastTarget;
    };

#pragma endregion

#pragma region BlackboardPropertyNavigationFindPathToEntityNavigationState

    class BlackboardPropertyNavigationFindPathToEntityNavigationState : public Blackboard::BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyNavigationFindPathToEntityNavigationState, AZ::SystemAllocator, 0);
        AZ_RTTI(
            BlackboardPropertyNavigationFindPathToEntityNavigationState,
            "{A2B1A6E2-1737-4085-AD12-039908D71586}",
            Blackboard::BlackboardProperty);

        using NavigationState = NavigationFindPathToEntityNode::NavigationState;

        static void Reflect(AZ::ReflectContext* rc);

        BlackboardPropertyNavigationFindPathToEntityNavigationState();
        explicit BlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name);
        BlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name, const NavigationState& value);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyNavigationFindPathToEntityNavigationState* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard::Blackboard& blackboard) const override;

        void SetValueFromString(const char* value) override;

        NavigationState m_value = NavigationState::Idle;

    protected:
        void CloneDataFrom(const BlackboardProperty* property) override;
    };

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(
        SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation::NavigationFindPathToEntityNode::NavigationState,
        "{EE3605C4-0A57-4642-B368-54B4F5BF96E7}");
} // namespace AZ

namespace BT
{
    using namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation;

    template<>
    inline std::string toStr<NavigationFindPathToEntityNode::NavigationState>(NavigationFindPathToEntityNode::NavigationState value)
    {
        switch (value)
        {
        default:
        case NavigationFindPathToEntityNode::NavigationState::Idle:
            return "idle";
        case NavigationFindPathToEntityNode::NavigationState::Navigating:
            return "navigating";
        case NavigationFindPathToEntityNode::NavigationState::Complete:
            return "complete";
        }
    }

    template<>
    inline NavigationFindPathToEntityNode::NavigationState convertFromString(StringView str)
    {
        if (str.compare("idle") == 0)
        {
            return NavigationFindPathToEntityNode::NavigationState::Idle;
        }
        if (str.compare("navigating") == 0)
        {
            return NavigationFindPathToEntityNode::NavigationState::Navigating;
        }
        if (str.compare("complete") == 0)
        {
            return NavigationFindPathToEntityNode::NavigationState::Complete;
        }

        return NavigationFindPathToEntityNode::NavigationState::Idle;
    }
} // namespace BT

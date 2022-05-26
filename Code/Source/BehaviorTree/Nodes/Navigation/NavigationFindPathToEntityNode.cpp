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

#include <StdAfx.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation
{
    using SSBehaviorTreeBlackboardProperty = Blackboard::BlackboardProperty;

#pragma region NavigationFindPathToEntityNode

    NavigationFindPathToEntityNode::NavigationFindPathToEntityNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : Node(name, config)
        , _navigationState(NavigationState::Idle)
        , _requestId(0)
    {
    }

    void NavigationFindPathToEntityNode::Reflect(AZ::ReflectContext* rc)
    {
        BlackboardPropertyNavigationFindPathToEntityNavigationState::Reflect(rc);
    }

    void NavigationFindPathToEntityNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register properties
        registry->RegisterProperty<BlackboardPropertyNavigationFindPathToEntityNavigationState>(
            "NavigationFindPathToEntityNode::NavigationState");

        // 2 - Register node
        registry->DelayNodeRegistration<NavigationFindPathToEntityNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList NavigationFindPathToEntityNode::providedPorts()
    {
        Core::BehaviorTreePortsList ports = Node::providedPorts();

        ports.merge(Core::BehaviorTreePortsList({
            BT::InputPort<AZ::EntityId>(NODE_PORT_TARGET_NAME, NODE_PORT_TARGET_DESCRIPTION),
            BT::OutputPort<NavigationState>(NODE_PORT_STATE_NAME, NODE_PORT_STATE_DESCRIPTION),
        }));

        return ports;
    }

    void NavigationFindPathToEntityNode::OnTraversalStarted(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == _requestId)
        {
            SetNavigationState(NavigationState::Navigating);
            TraversalStarted();
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalPathUpdate(
        LmbrCentral::PathfindRequest::NavigationRequestId requestId,
        const AZ::Vector3& nextPathPosition,
        const AZ::Vector3& inflectionPosition)
    {
        if (requestId == _requestId)
        {
            SetNavigationState(NavigationState::Navigating);
            TraversalPathUpdate(nextPathPosition, inflectionPosition);
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalInProgress(
        LmbrCentral::PathfindRequest::NavigationRequestId requestId, float distanceRemaining)
    {
        if (requestId == _requestId)
        {
            SetNavigationState(NavigationState::Navigating);
            TraversalInProgress(distanceRemaining);
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalComplete(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == _requestId)
        {
            SetNavigationState(NavigationState::Complete);
            TraversalComplete();
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalCancelled(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == _requestId)
        {
            SetNavigationState(NavigationState::Idle);
            TraversalCancelled();
        }
    }

    void NavigationFindPathToEntityNode::Start()
    {
        if (const AZ::EntityId& target = GetTarget(); target.IsValid())
        {
            if (!_requestId && target != _lastTarget)
            {
                _lastTarget = target;
                RestartNavigation();
                ReconnectBus();
            }
            else if (_requestId && _navigationState == NavigationState::Complete)
            {
                SetNavigationState(NavigationState::Idle);
                RestartNavigation();
            }
        }
        else if (!target.IsValid())
        {
            AZ_Warning("BehaveAI [BehaviorTree]", false, "[%s:%s]: No target attribute was supplied.", RegisteredNodeName(), NodeName());
        }
    }

    Core::BehaviorTreeNodeStatus NavigationFindPathToEntityNode::Tick()
    {
        if (_lastTarget.IsValid() && _requestId)
        {
            switch (_navigationState)
            {
            case NavigationState::Navigating:
                return Core::BehaviorTreeNodeStatus::RUNNING;

            case NavigationState::Complete:
                return Core::BehaviorTreeNodeStatus::SUCCESS;

            default:
                return Core::BehaviorTreeNodeStatus::FAILURE;
            }
        }

        return Core::BehaviorTreeNodeStatus::FAILURE;
    }

    void NavigationFindPathToEntityNode::TraversalStarted()
    {
    }

    void NavigationFindPathToEntityNode::TraversalPathUpdate(
        [[maybe_unused]] const AZ::Vector3& nextPathPosition, [[maybe_unused]] const AZ::Vector3& inflectionPosition)
    {
    }

    void NavigationFindPathToEntityNode::TraversalInProgress([[maybe_unused]] float distanceRemaining)
    {
    }

    void NavigationFindPathToEntityNode::TraversalComplete()
    {
    }

    void NavigationFindPathToEntityNode::TraversalCancelled()
    {
    }

    void NavigationFindPathToEntityNode::SetNavigationState(const NavigationState& state)
    {
        _navigationState = state;
        SetOutputValue(NODE_PORT_STATE_NAME, _navigationState);
    }

    const AZ::EntityId& NavigationFindPathToEntityNode::GetTarget() const
    {
        static const auto invalidEntityId = AZ::EntityId();

        Core::Optional<AZ::EntityId> id = GetInputValue<AZ::EntityId>(NODE_PORT_TARGET_NAME);
        return id.has_value() ? id.value() : invalidEntityId;
    }

    void NavigationFindPathToEntityNode::ConnectBus()
    {
        LmbrCentral::NavigationComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void NavigationFindPathToEntityNode::DisconnectBus()
    {
        LmbrCentral::NavigationComponentNotificationBus::Handler::BusDisconnect(GetEntityId());
    }

    void NavigationFindPathToEntityNode::ReconnectBus()
    {
        DisconnectBus();
        ConnectBus();
    }

    void NavigationFindPathToEntityNode::StartNavigation()
    {
        if (!_requestId)
        {
            EBUS_EVENT_ID_RESULT(_requestId, GetEntityId(), LmbrCentral::NavigationComponentRequestBus, FindPathToEntity, _lastTarget);
        }
    }

    void NavigationFindPathToEntityNode::StopNavigation()
    {
        if (_requestId)
        {
            EBUS_EVENT_ID(GetEntityId(), LmbrCentral::NavigationComponentRequestBus, Stop, _requestId);
            _requestId = 0;
        }
    }

    void NavigationFindPathToEntityNode::RestartNavigation()
    {
        StopNavigation();
        StartNavigation();
    }

#pragma endregion

#pragma region BlackboardPropertyNavigationFindPathToEntityNavigationState

    BlackboardPropertyNavigationFindPathToEntityNavigationState::BlackboardPropertyNavigationFindPathToEntityNavigationState()
        : BlackboardProperty()
    {
    }

    BlackboardPropertyNavigationFindPathToEntityNavigationState::BlackboardPropertyNavigationFindPathToEntityNavigationState(
        const char* name)
        : BlackboardProperty(name)
    {
    }

    BlackboardPropertyNavigationFindPathToEntityNavigationState::BlackboardPropertyNavigationFindPathToEntityNavigationState(
        const char* name, const NavigationState& value)
        : BlackboardProperty(name)
        , m_value(value)
    {
    }

    void BlackboardPropertyNavigationFindPathToEntityNavigationState::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Enum<NavigationState>()
                ->Version(0)
                ->Value("Idle", NavigationState::Idle)
                ->Value("Navigating", NavigationState::Navigating)
                ->Value("Complete", NavigationState::Complete);

            sc->Class<BlackboardPropertyNavigationFindPathToEntityNavigationState, BlackboardProperty>()->Version(0)->Field(
                "Value", &BlackboardPropertyNavigationFindPathToEntityNavigationState::m_value);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Enum<NavigationState>(
                      "[NavigationFindPathToEntityNode] Navigation State",
                      "The navigation state of the NavigationFindPathToEntityNode node.")
                    ->Value("Idle", NavigationState::Idle)
                    ->Value("Navigating", NavigationState::Navigating)
                    ->Value("Complete", NavigationState::Complete);

                ec->Class<BlackboardPropertyNavigationFindPathToEntityNavigationState>(
                      "[BehaveAI] BehaviorTree Blackboard Property (NavigationState)", "A blackboard property.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &BlackboardPropertyNavigationFindPathToEntityNavigationState::m_value, "Value",
                        "A navigation state.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);
            }
        }
    }

    const void* BlackboardPropertyNavigationFindPathToEntityNavigationState::GetDataAddress() const
    {
        return &m_value;
    }

    const AZ::Uuid& BlackboardPropertyNavigationFindPathToEntityNavigationState::GetDataTypeUuid() const
    {
        return azrtti_typeid<NavigationState>();
    }

    BlackboardPropertyNavigationFindPathToEntityNavigationState* BlackboardPropertyNavigationFindPathToEntityNavigationState::Clone(
        const char* name) const
    {
        return aznew BlackboardPropertyNavigationFindPathToEntityNavigationState(name ? name : mName.c_str(), m_value);
    }

    void BlackboardPropertyNavigationFindPathToEntityNavigationState::AddBlackboardEntry(const Blackboard::Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<NavigationState>(mName.c_str(), m_value);
    }

    void BlackboardPropertyNavigationFindPathToEntityNavigationState::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<NavigationState>(value);
    }

    void BlackboardPropertyNavigationFindPathToEntityNavigationState::CloneDataFrom(const BlackboardProperty* property)
    {
        const auto* p = azrtti_cast<const BlackboardPropertyNavigationFindPathToEntityNavigationState*>(property);
        AZ_Error("BehaveAI [BehaviorTree]", p, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (p)
        {
            m_value = p->m_value;
        }
    }

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Navigation

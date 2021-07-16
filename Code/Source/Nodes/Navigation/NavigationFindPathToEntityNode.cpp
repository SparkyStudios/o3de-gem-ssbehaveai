#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Navigation
{
    using SSBehaviorTreeBlackboardProperty = Blackboard::SSBehaviorTreeBlackboardProperty;

#pragma region NavigationFindPathToEntityNode

    NavigationFindPathToEntityNode::NavigationFindPathToEntityNode(
        const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
        , m_navigationState(NavigationState::STATE_IDLE)
        , m_requestId(0)
    {
    }

    void NavigationFindPathToEntityNode::Reflect(AZ::ReflectContext* context)
    {
        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::Reflect(context);
    }

    void NavigationFindPathToEntityNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register properties
        registry->RegisterProperty<SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState>(
            "NavigationFindPathToEntityNode::NavigationState");

        // 2 - Register node
        registry->DelayNodeRegistration<NavigationFindPathToEntityNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList NavigationFindPathToEntityNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<AZ::EntityId>(NODE_PORT_TARGET_NAME, NODE_PORT_TARGET_DESCRIPTION),
            BT::OutputPort<NavigationState>(NODE_PORT_STATE_NAME, NODE_PORT_STATE_DESCRIPTION),
        }));

        return ports;
    }

    void NavigationFindPathToEntityNode::OnTraversalStarted(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == m_requestId)
        {
            SetNavigationState(NavigationState::STATE_NAVIGATING);
            TraversalStarted();
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalPathUpdate(
        LmbrCentral::PathfindRequest::NavigationRequestId requestId,
        const AZ::Vector3& nextPathPosition,
        const AZ::Vector3& inflectionPosition)
    {
        if (requestId == m_requestId)
        {
            SetNavigationState(NavigationState::STATE_NAVIGATING);
            TraversalPathUpdate(nextPathPosition, inflectionPosition);
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalInProgress(
        LmbrCentral::PathfindRequest::NavigationRequestId requestId, float distanceRemaining)
    {
        if (requestId == m_requestId)
        {
            SetNavigationState(NavigationState::STATE_NAVIGATING);
            TraversalInProgress(distanceRemaining);
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalComplete(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == m_requestId)
        {
            SetNavigationState(NavigationState::STATE_COMPLETE);
            TraversalComplete();
        }
    }

    void NavigationFindPathToEntityNode::OnTraversalCancelled(LmbrCentral::PathfindRequest::NavigationRequestId requestId)
    {
        if (requestId == m_requestId)
        {
            SetNavigationState(NavigationState::STATE_IDLE);
            TraversalCancelled();
        }
    }

    void NavigationFindPathToEntityNode::Start()
    {
        const AZ::EntityId& target = GetTarget();

        if (target.IsValid())
        {
            if (!m_requestId && target != m_lastTarget)
            {
                m_lastTarget = target;
                RestartNavigation();
                ReconnectBus();
            }
            else if (m_requestId && m_navigationState == NavigationState::STATE_COMPLETE)
            {
                SetNavigationState(NavigationState::STATE_IDLE);
                RestartNavigation();
            }
        }
        else if (!target.IsValid())
        {
            AZ_Warning("SSBehaviorTree", false, "[%s:%s]: No target attribute was supplied.", RegisteredNodeName(), NodeName());
        }
    }

    Core::SSBehaviorTreeNodeStatus NavigationFindPathToEntityNode::Tick()
    {
        if (m_lastTarget.IsValid() && m_requestId)
        {
            switch (m_navigationState)
            {
            case NavigationState::STATE_NAVIGATING:
                return Core::SSBehaviorTreeNodeStatus::RUNNING;

            case NavigationState::STATE_COMPLETE:
                return Core::SSBehaviorTreeNodeStatus::SUCCESS;

            default:
                return Core::SSBehaviorTreeNodeStatus::FAILURE;
            }
        }

        return Core::SSBehaviorTreeNodeStatus::FAILURE;
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

    void NavigationFindPathToEntityNode::SetNavigationState(NavigationState state)
    {
        m_navigationState = state;
        Core::SSBehaviorTreeNode::SetOutputValue(NODE_PORT_STATE_NAME, m_navigationState);
    }

    const AZ::EntityId& NavigationFindPathToEntityNode::GetTarget() const
    {
        static const AZ::EntityId invalidEntityId = AZ::EntityId();

        Core::Optional<AZ::EntityId> id = Core::SSBehaviorTreeNode::GetInputValue<AZ::EntityId>(NODE_PORT_TARGET_NAME);
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
        if (!m_requestId)
        {
            EBUS_EVENT_ID_RESULT(m_requestId, GetEntityId(), LmbrCentral::NavigationComponentRequestBus, FindPathToEntity, m_lastTarget);
        }
    }

    void NavigationFindPathToEntityNode::StopNavigation()
    {
        if (m_requestId)
        {
            EBUS_EVENT_ID(GetEntityId(), LmbrCentral::NavigationComponentRequestBus, Stop, m_requestId);
            m_requestId = 0;
        }
    }

    void NavigationFindPathToEntityNode::RestartNavigation()
    {
        StopNavigation();
        StartNavigation();
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState

    SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::
        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState()
        : SSBehaviorTreeBlackboardProperty()
    {
    }

    SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::
        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name)
        : SSBehaviorTreeBlackboardProperty(name)
    {
    }

    SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::
        SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState(const char* name, const NavigationState& value)
        : SSBehaviorTreeBlackboardProperty(name)
        , m_value(value)
    {
    }

    void SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* rc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            rc->Enum<NavigationState>()
                ->Version(1)
                ->Value("idle", NavigationState::STATE_IDLE)
                ->Value("navigating", NavigationState::STATE_NAVIGATING)
                ->Value("complete", NavigationState::STATE_COMPLETE);

            rc->Class<SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState, SSBehaviorTreeBlackboardProperty>()
                ->Version(1)
                ->Field("value", &SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::m_value);

            if (AZ::EditContext* ec = rc->GetEditContext())
            {
                ec->Enum<NavigationState>(
                      "[NavigationFindPathToEntityNode] Navigation State",
                      "The navigation state of the NavigationFindPathToEntityNode node.")
                    ->Value("Idle", NavigationState::STATE_IDLE)
                    ->Value("Navigating", NavigationState::STATE_NAVIGATING)
                    ->Value("Complete", NavigationState::STATE_COMPLETE);

                ec->Class<SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState>(
                      "SS Behavior Tree Blackboard Property (NavigationState)", "A blackboard property.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::m_value,
                        "value", "A navigation state.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);
            }
        }
    }

    const void* SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::GetDataAddress() const
    {
        return &m_value;
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::GetDataTypeUuid() const
    {
        return azrtti_typeid<NavigationState>();
    }

    SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState*
    SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState(name ? name : m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::AddBlackboardEntry(
        const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<NavigationState>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<NavigationState>(value);
    }

    void SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState::CloneDataFrom(
        const SSBehaviorTreeBlackboardProperty* property)
    {
        const SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState* p =
            azrtti_cast<const SSBehaviorTreeBlackboardPropertyNavigationFindPathToEntityNavigationState*>(property);
        AZ_Error("SSBehaviorTree", p, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (p)
        {
            m_value = p->m_value;
        }
    }

#pragma endregion
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Navigation

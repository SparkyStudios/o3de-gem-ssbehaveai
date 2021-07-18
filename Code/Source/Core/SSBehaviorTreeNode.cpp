#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>

namespace SparkyStudios::AI::BehaviorTree::Core
{
    SSBehaviorTreeNode::SSBehaviorTreeNode(const std::string& name, const SSBehaviorTreeNodeConfiguration& config)
        : BT::StatefulActionNode(name, config)
        , m_started(false)
    {
    }

    SSBehaviorTreePortsList SSBehaviorTreeNode::providedPorts()
    {
        return {};
    }

    void SSBehaviorTreeNode::Start()
    {
    }

    SSBehaviorTreeNodeStatus SSBehaviorTreeNode::Tick()
    {
        return SSBehaviorTreeNodeStatus::SUCCESS;
    };

    void SSBehaviorTreeNode::Finish()
    {
    }

    AZ::EntityId SSBehaviorTreeNode::GetEntityId()
    {
        AZ::EntityId id = AZ::EntityId();
        BT::Blackboard::Ptr blackboard = GetBlackboard();
        blackboard->get<AZ::EntityId>("entityId", id);

        return id;
    }

    const BT::Blackboard::Ptr& SSBehaviorTreeNode::GetBlackboard() const
    {
        return config().blackboard;
    }

    const char* SSBehaviorTreeNode::RegisteredNodeName() const
    {
        return registrationName().c_str();
    }

    const char* SSBehaviorTreeNode::NodeName() const
    {
        return name().c_str();
    }

    SSBehaviorTreeNodeStatus SSBehaviorTreeNode::onStart()
    {
        if (!m_started)
        {
            Start();
            m_started = true;
        }

        return SSBehaviorTreeNodeStatus::RUNNING;
    }

    SSBehaviorTreeNodeStatus SSBehaviorTreeNode::onRunning()
    {
        if (!m_started)
        {
            Start();
            m_started = true;
        }

        SSBehaviorTreeNodeStatus tick_status = Tick();

        if (tick_status == SSBehaviorTreeNodeStatus::FAILURE || tick_status == SSBehaviorTreeNodeStatus::SUCCESS)
        {
            Finish();
            m_started = false; // Make possible to start the node again.
        }

        return tick_status;
    }

    void SSBehaviorTreeNode::onHalted()
    {
        if (m_started)
        {
            m_started = false;
            Finish();
        }
    }

    SSBehaviorTreeCondition::SSBehaviorTreeCondition(const std::string& name, const SSBehaviorTreeNodeConfiguration& config)
        : BT::ConditionNode(name, config)
    {
    }

    SSBehaviorTreeNodeStatus SSBehaviorTreeCondition::tick()
    {
        return Condition() ? SSBehaviorTreeNodeStatus::SUCCESS : SSBehaviorTreeNodeStatus::FAILURE;
    }

#if defined(SSBEHAVIORTREE_EDITOR)
    const std::string SSBehaviorTreeNode::NodeCategory() const
    {
        return std::string();
    }
#endif

} // namespace SparkyStudios::AI::BehaviorTree::Core
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

#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
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

    const std::string SSBehaviorTreeNode::NodeCategory() const
    {
        return std::string();
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

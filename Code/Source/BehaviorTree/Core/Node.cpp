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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    Node::Node(const std::string& name, const BehaviorTreeNodeConfiguration& config)
        : BT::StatefulActionNode(name, config)
        , _started(false)
    {
    }

    BehaviorTreePortsList Node::providedPorts()
    {
        return {};
    }

    void Node::Start()
    {
    }

    BehaviorTreeNodeStatus Node::Tick()
    {
        return BehaviorTreeNodeStatus::SUCCESS;
    };

    void Node::Finish()
    {
    }

    AZ::EntityId Node::GetEntityId() const
    {
        auto id = AZ::EntityId();
        const BT::Blackboard::Ptr blackboard = GetBlackboard();
        blackboard->get<AZ::EntityId>("entityId", id);

        return id;
    }

    const BT::Blackboard::Ptr& Node::GetBlackboard() const
    {
        return config().blackboard;
    }

    const char* Node::RegisteredNodeName() const
    {
        return registrationName().c_str();
    }

    const char* Node::NodeName() const
    {
        return name().c_str();
    }

    BehaviorTreeNodeStatus Node::onStart()
    {
        if (!_started)
        {
            Start();
            _started = true;
        }

        return BehaviorTreeNodeStatus::RUNNING;
    }

    BehaviorTreeNodeStatus Node::onRunning()
    {
        if (!_started)
        {
            Start();
            _started = true;
        }

        const BehaviorTreeNodeStatus status = Tick();

        if (status == BehaviorTreeNodeStatus::FAILURE || status == BehaviorTreeNodeStatus::SUCCESS)
        {
            Finish();
            _started = false; // Make possible to start the node again.
        }

        return status;
    }

    void Node::onHalted()
    {
        if (_started)
        {
            _started = false;
            Finish();
        }
    }

    BehaviorTreeConditionNode::BehaviorTreeConditionNode(const std::string& name, const BehaviorTreeNodeConfiguration& config)
        : BT::ConditionNode(name, config)
    {
    }

    BehaviorTreeNodeStatus BehaviorTreeConditionNode::tick()
    {
        return Condition() ? BehaviorTreeNodeStatus::SUCCESS : BehaviorTreeNodeStatus::FAILURE;
    }

    std::string Node::NodeCategory() const
    {
        return std::string();
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

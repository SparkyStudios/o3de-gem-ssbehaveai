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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/WaitNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
    WaitNode::WaitNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : Core::Node(name, config)
    {
    }

    void WaitNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void WaitNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Add node for delayed registration
        registry->DelayNodeRegistration<WaitNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList WaitNode::providedPorts()
    {
        Core::BehaviorTreePortsList ports = Core::Node::providedPorts();

        ports.merge(Core::BehaviorTreePortsList({
            BT::InputPort<float>(NODE_PORT_SECONDS_NAME, 0, NODE_PORT_SECONDS_DESCRIPTION),
        }));

        return ports;
    }

    void WaitNode::Start()
    {
        _countDown = GetSeconds();
    }

    Core::BehaviorTreeNodeStatus WaitNode::Tick()
    {
        const float seconds = GetSeconds();

        if (seconds == 0)
            return Core::BehaviorTreeNodeStatus::SUCCESS;

        float dt = -1;
        EBUS_EVENT_RESULT(dt, AZ::TickRequestBus, GetTickDeltaTime);

        if (dt > -1)
        {
            _countDown -= dt;

            if (_countDown < 0)
            {
                _countDown = seconds;
                return Core::BehaviorTreeNodeStatus::SUCCESS;
            }

            return Core::BehaviorTreeNodeStatus::RUNNING;
        }

        return Core::BehaviorTreeNodeStatus::FAILURE;
    }

    float WaitNode::GetSeconds() const
    {
        Core::Optional<float> s = GetInputValue<float>(NODE_PORT_SECONDS_NAME);
        return s.has_value() ? s.value() : 0;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common

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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    SimpleMotionSetRetargetMotionNode::SimpleMotionSetRetargetMotionNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : Node(name, config)
    {
    }

    void SimpleMotionSetRetargetMotionNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void SimpleMotionSetRetargetMotionNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionSetRetargetMotionNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList SimpleMotionSetRetargetMotionNode::providedPorts()
    {
        Core::BehaviorTreePortsList ports = Node::providedPorts();

        ports.merge(Core::BehaviorTreePortsList({
            BT::InputPort<bool>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::BehaviorTreeNodeStatus SimpleMotionSetRetargetMotionNode::Tick()
    {
        if (Core::Optional<bool> value = GetInputValue<bool>(NODE_PORT_VALUE_NAME); value.has_value())
        {
            EBUS_EVENT_ID(GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, RetargetMotion, value.value());
            return Core::BehaviorTreeNodeStatus::SUCCESS;
        }

        return Core::BehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

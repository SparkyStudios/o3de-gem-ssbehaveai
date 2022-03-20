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

#include <SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetMotionNode.h>

#include <AzCore/Asset/AssetCommon.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    SimpleMotionGetMotionNode::SimpleMotionGetMotionNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionGetMotionNode::Reflect(AZ::ReflectContext* reflection)
    {
        AZ_UNUSED(reflection);
    }

    void SimpleMotionGetMotionNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionGetMotionNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionGetMotionNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::OutputPort<AZ::Data::AssetId>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionGetMotionNode::Tick()
    {
        AZ::Data::AssetId value;
        EBUS_EVENT_ID_RESULT(value, GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, GetMotion);
        SetOutputValue<AZ::Data::AssetId>(NODE_PORT_VALUE_NAME, value);

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

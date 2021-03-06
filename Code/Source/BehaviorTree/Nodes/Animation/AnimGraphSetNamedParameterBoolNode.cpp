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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterBoolNode::AnimGraphSetNamedParameterBoolNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<bool>(name, config)
    {
    }

    void AnimGraphSetNamedParameterBoolNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphSetNamedParameterBoolNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterBoolNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphSetNamedParameterBoolNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<bool>::providedPorts();
    }

    void AnimGraphSetNamedParameterBoolNode::SetParameter()
    {
        if (Core::Optional<bool> value = GetInputValue<bool>(NODE_PORT_VALUE_NAME); value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterBool, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

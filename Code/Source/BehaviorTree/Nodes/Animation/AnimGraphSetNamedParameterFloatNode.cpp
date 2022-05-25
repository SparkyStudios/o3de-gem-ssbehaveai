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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterFloatNode::AnimGraphSetNamedParameterFloatNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<float>(name, config)
    {
    }

    void AnimGraphSetNamedParameterFloatNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphSetNamedParameterFloatNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterFloatNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphSetNamedParameterFloatNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<float>::providedPorts();
    }

    void AnimGraphSetNamedParameterFloatNode::SetParameter()
    {
        if (Core::Optional<float> value = GetInputValue<float>(NODE_PORT_VALUE_NAME); value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterFloat, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

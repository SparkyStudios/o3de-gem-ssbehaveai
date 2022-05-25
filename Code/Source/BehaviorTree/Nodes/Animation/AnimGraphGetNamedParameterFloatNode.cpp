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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterFloatNode::AnimGraphGetNamedParameterFloatNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<float>(name, config)
    {
    }

    void AnimGraphGetNamedParameterFloatNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphGetNamedParameterFloatNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterFloatNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphGetNamedParameterFloatNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<float>::providedPorts();
    }

    void AnimGraphGetNamedParameterFloatNode::GetParameter()
    {
        float value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterFloat, m_parameterIndex);
        SetOutputValue<float>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

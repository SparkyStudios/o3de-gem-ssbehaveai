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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterRotationEulerNode::AnimGraphGetNamedParameterRotationEulerNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphGetNamedParameterRotationEulerNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphGetNamedParameterRotationEulerNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterRotationEulerNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphGetNamedParameterRotationEulerNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphGetNamedParameterRotationEulerNode::GetParameter()
    {
        AZ::Vector3 value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterRotationEuler, m_parameterIndex);
        SetOutputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphGetNamedParameterRotationNode::AnimGraphGetNamedParameterRotationNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphGetNamedParameterNode<AZ::Quaternion>(name, config)
    {
    }

    void AnimGraphGetNamedParameterRotationNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphGetNamedParameterRotationNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphGetNamedParameterRotationNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphGetNamedParameterRotationNode::providedPorts()
    {
        return AnimGraphGetNamedParameterNode<AZ::Quaternion>::providedPorts();
    }

    void AnimGraphGetNamedParameterRotationNode::GetParameter()
    {
        AZ::Quaternion value;
        EBUS_EVENT_ID_RESULT(
            value, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, GetParameterRotation, m_parameterIndex);
        SetOutputValue<AZ::Quaternion>(NODE_PORT_VALUE_NAME, value);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

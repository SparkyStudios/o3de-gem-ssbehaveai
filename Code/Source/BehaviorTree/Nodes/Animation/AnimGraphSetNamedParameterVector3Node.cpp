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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterVector3Node::AnimGraphSetNamedParameterVector3Node(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZ::Vector3>(name, config)
    {
    }

    void AnimGraphSetNamedParameterVector3Node::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphSetNamedParameterVector3Node::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterVector3Node>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphSetNamedParameterVector3Node::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZ::Vector3>::providedPorts();
    }

    void AnimGraphSetNamedParameterVector3Node::SetParameter()
    {
        if (Core::Optional<AZ::Vector3> value = GetInputValue<AZ::Vector3>(NODE_PORT_VALUE_NAME); value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterVector3, m_parameterIndex, value.value());
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

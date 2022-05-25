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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    AnimGraphSetNamedParameterStringNode::AnimGraphSetNamedParameterStringNode(
        const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : AnimGraphSetNamedParameterNode<AZStd::string>(name, config)
    {
    }

    void AnimGraphSetNamedParameterStringNode::Reflect(AZ::ReflectContext* rc)
    {
        AZ_UNUSED(rc);
    }

    void AnimGraphSetNamedParameterStringNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register node
        registry->DelayNodeRegistration<AnimGraphSetNamedParameterStringNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList AnimGraphSetNamedParameterStringNode::providedPorts()
    {
        return AnimGraphSetNamedParameterNode<AZStd::string>::providedPorts();
    }

    void AnimGraphSetNamedParameterStringNode::SetParameter()
    {
        if (Core::Optional<AZStd::string> value = GetInputValue<AZStd::string>(NODE_PORT_VALUE_NAME); value.has_value())
        {
            EBUS_EVENT_ID(
                GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, SetParameterString, m_parameterIndex,
                value.value().c_str());
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

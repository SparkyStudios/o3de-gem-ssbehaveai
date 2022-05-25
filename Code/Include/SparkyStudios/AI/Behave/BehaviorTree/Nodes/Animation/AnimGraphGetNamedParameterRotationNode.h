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

#pragma once

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterRotationNode : public AnimGraphGetNamedParameterNode<AZ::Quaternion>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterRotationNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterRotationNode, "{1a04009d-28b1-45c6-bf95-52ffddb214d3}", AnimGraphGetNamedParameterNode<AZ::Quaternion>);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterRotation";

        AnimGraphGetNamedParameterRotationNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* rc);

        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        static Core::BehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

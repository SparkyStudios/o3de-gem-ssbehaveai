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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterVector2Node : public AnimGraphSetNamedParameterNode<AZ::Vector2>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterVector2Node, AZ::SystemAllocator, 0);
        AZ_RTTI(
            AnimGraphSetNamedParameterVector2Node, "{b5ab134c-c73a-41c1-b1cf-7aaf6aa715b2}", AnimGraphSetNamedParameterNode<AZ::Vector2>);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterVector2";

        AnimGraphSetNamedParameterVector2Node(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* rc);

        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        static Core::BehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

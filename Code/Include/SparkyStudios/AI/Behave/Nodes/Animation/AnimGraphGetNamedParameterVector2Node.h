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

#include <SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterVector2Node : public AnimGraphGetNamedParameterNode<AZ::Vector2>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterVector2Node, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterVector2Node, "{b1502554-025f-447d-96e9-abffbca21cde}", AnimGraphGetNamedParameterNode<AZ::Vector2>);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterVector2";

        AnimGraphGetNamedParameterVector2Node(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

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
    class AnimGraphGetNamedParameterStringNode : public AnimGraphGetNamedParameterNode<AZStd::string>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterStringNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterStringNode, "{b72eff30-3c72-465d-8ad4-c59ddd9f9e27}", AnimGraphGetNamedParameterNode<AZStd::string>);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterString";

        AnimGraphGetNamedParameterStringNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

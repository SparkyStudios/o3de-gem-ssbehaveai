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

#include <SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterRotationEulerNode : public AnimGraphSetNamedParameterNode<AZ::Vector3>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterRotationEulerNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterRotationEulerNode, "{8b375c4a-b061-4664-b015-153189760769}", AnimGraphSetNamedParameterNode<AZ::Vector3>);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterRotationEuler";

        AnimGraphSetNamedParameterRotationEulerNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

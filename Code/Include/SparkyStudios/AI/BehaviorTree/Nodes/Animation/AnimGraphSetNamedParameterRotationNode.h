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

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterRotationNode : public AnimGraphSetNamedParameterNode<AZ::Quaternion>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterRotationNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterRotationNode, "{dc051fe8-546c-4426-9f72-2b92ab7a1d9a}", AnimGraphSetNamedParameterNode<AZ::Quaternion>);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterRotation";

        AnimGraphSetNamedParameterRotationNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation

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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class SimpleMotionGetMotionNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionGetMotionNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionGetMotionNode, "{419fbbd4-145b-4db6-8e08-46288404c17c}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "SimpleMotionGetMotion";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blackboard entry in which the value should be set.";

        SimpleMotionGetMotionNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* reflection);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

        const std::string NodeCategory() const override
        {
            return "Animation";
        }

    protected:
        Core::SSBehaviorTreeNodeStatus Tick();
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
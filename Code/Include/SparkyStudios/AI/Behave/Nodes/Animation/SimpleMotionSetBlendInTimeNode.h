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

#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class SimpleMotionSetBlendInTimeNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionSetBlendInTimeNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionSetBlendInTimeNode, "{a4dc8b71-70cb-4b68-b6c2-88fdd1f9f6e6}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "SimpleMotionSetBlendInTime";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blend in time value to define to the simple motion.";

        SimpleMotionSetBlendInTimeNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

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

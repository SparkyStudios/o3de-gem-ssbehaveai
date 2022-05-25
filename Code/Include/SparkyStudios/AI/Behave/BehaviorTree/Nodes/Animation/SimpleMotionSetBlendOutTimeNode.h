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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    class SimpleMotionSetBlendOutTimeNode : public Core::Node
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionSetBlendOutTimeNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionSetBlendOutTimeNode, "{e2aae638-5d2f-42b3-9bcc-14d02cb1eda5}", Core::Node);

        static constexpr const char* NODE_NAME = "SimpleMotionSetBlendOutTime";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blend out time value to define to the simple motion.";

        SimpleMotionSetBlendOutTimeNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* rc);

        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        static Core::BehaviorTreePortsList providedPorts();

        std::string NodeCategory() const override
        {
            return "Animation";
        }

    protected:
        Core::BehaviorTreeNodeStatus Tick() override;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

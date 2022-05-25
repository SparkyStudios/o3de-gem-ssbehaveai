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
    class SimpleMotionSetReverseMotionNode : public Core::Node
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionSetReverseMotionNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionSetReverseMotionNode, "{1b03295f-298b-4bb3-885b-46ffc8aa7e4a}", Core::Node);

        static constexpr const char* NODE_NAME = "SimpleMotionSetReverseMotion";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The reverse state to define to the simple motion.";

        SimpleMotionSetReverseMotionNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

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

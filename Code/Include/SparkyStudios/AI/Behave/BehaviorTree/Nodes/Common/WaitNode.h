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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
    /**
     * @brief Waits for a given number of seconds before returning.
     * During this time the behavior tree will be paused.
     *
     * @par Node Ports
     * - seconds: The number of seconds to wait before to continue the execution.
     */
    class WaitNode : public Core::Node
    {
    public:
        AZ_CLASS_ALLOCATOR(WaitNode, AZ::SystemAllocator, 0);
        AZ_RTTI(WaitNode, "{E7F66E3A-7B08-4DA9-8FA2-A5F95355590A}", Core::Node);

        WaitNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        /**
         * @brief The name of the node in the behavior tree file.
         */
        static constexpr const char* NODE_NAME = "Wait";

        static constexpr const char* NODE_PORT_SECONDS_NAME = "seconds";
        static constexpr const char* NODE_PORT_SECONDS_DESCRIPTION = "The number of seconds to wait before to continue the execution.";

        /**
         * @brief Reflect this class in the given ReflectContext.
         *
         * @param rc The ReflectContext.
         */
        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief Register this node in the nodes registry.
         *
         * @param registry The registry to register this node in.
         */
        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        /**
         * @brief Returns the list of ports returned by this node.
         *
         * @return Core::SSBehaviorTreePortsList
         */
        static Core::BehaviorTreePortsList providedPorts();

        std::string NodeCategory() const override
        {
            return "Common";
        }

    protected:
        void Start() override;

        Core::BehaviorTreeNodeStatus Tick() override;

    private:
        float _countDown = 0;

        float GetSeconds() const;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common

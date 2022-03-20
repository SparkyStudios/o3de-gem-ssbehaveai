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

#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeFactory.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>

#include <AzCore/Component/TickBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
    /**
     * @brief Waits for a given number of seconds before returning.
     * During this time the behavior tree will be paused.
     *
     * @par Node Ports
     * - seconds: The number of seconds to wait before to continue the execution.
     */
    class WaitNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(WaitNode, AZ::SystemAllocator, 0);
        AZ_RTTI(WaitNode, "{e7f66e3a-7b08-4da9-8fa2-a5f95355590a}", Core::SSBehaviorTreeNode);

        WaitNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        /**
         * @brief The name of the node in the behavior tree file.
         */
        static constexpr const char* NODE_NAME = "Wait";

        static constexpr const char* NODE_PORT_SECONDS_NAME = "seconds";
        static constexpr const char* NODE_PORT_SECONDS_DESCRIPTION = "The number of seconds to wait before to continue the execution.";

        /**
         * @brief Reflect this class in the given O3DE ReflectContext.
         *
         * @param reflection The O3DE ReflectContext.
         */
        static void Reflect(AZ::ReflectContext* reflection);

        /**
         * @brief Register this node in the nodes registry.
         *
         * @param registry The registry to register this node in.
         */
        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        /**
         * @brief Returns the list of ports returned by this node.
         *
         * @return Core::SSBehaviorTreePortsList
         */
        static Core::SSBehaviorTreePortsList providedPorts();

        const std::string NodeCategory() const override
        {
            return "Common";
        }

    protected:
        void Start() override;

        Core::SSBehaviorTreeNodeStatus Tick() override;

    private:
        float m_countDown = 0;

        float GetSeconds() const;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common

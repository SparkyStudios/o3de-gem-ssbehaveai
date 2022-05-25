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

#include <Integration/AnimGraphComponentBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation
{
    /**
     * @brief Base class for all AnimGraphSetNamedParameter nodes.
     *
     * @tparam T The type of the value the child class should set.
     */
    template<typename T>
    class AnimGraphSetNamedParameterNode : public Core::Node
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterNode, AZ::SystemAllocator, 0);
        AZ_RTTI(((AnimGraphSetNamedParameterNode<T>), "{8d202ec2-61ef-48ee-a083-a9ffc4362a97}", T), Core::Node);

        AnimGraphSetNamedParameterNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
            : Node(name, config)
        {
        }

        static constexpr AZ::u64 INVALID_PARAMETER_INDEX = 0xFFFFFFFFFFFFFFFF;

        static constexpr const char* NODE_PORT_PARAMETER_NAME = "parameter";
        static constexpr const char* NODE_PORT_PARAMETER_DESCRIPTION = "The name of the parameter";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The value to set to the parameter.";

        static Core::BehaviorTreePortsList providedPorts()
        {
            Core::BehaviorTreePortsList ports = Core::Node::providedPorts();

            ports.merge(Core::BehaviorTreePortsList({
                BT::InputPort<AZStd::string>(NODE_PORT_PARAMETER_NAME, NODE_PORT_PARAMETER_DESCRIPTION),
                BT::InputPort<T>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
            }));

            return ports;
        }

        std::string NodeCategory() const override
        {
            return "Animation";
        }

    protected:
        void Start() override
        {
            Core::Optional<AZStd::string> parameter = GetInputValue<AZStd::string>(NODE_PORT_PARAMETER_NAME);
            if (parameter.has_value())
            {
                EBUS_EVENT_ID_RESULT(
                    m_parameterIndex, GetEntityId(), EMotionFX::Integration::AnimGraphComponentRequestBus, FindParameterIndex,
                    parameter.value().c_str());

                if (m_parameterIndex == INVALID_PARAMETER_INDEX)
                {
                    AZ_Warning(
                        "SSBehaviorTree", false, "[%s:%s] The named parameter \"%s\" was not found in the animation graph.",
                        RegisteredNodeName(), NodeName(), parameter.value().c_str());
                }
            }
        }

        Core::BehaviorTreeNodeStatus Tick() override
        {
            if (m_parameterIndex == INVALID_PARAMETER_INDEX)
            {
                return Core::BehaviorTreeNodeStatus::FAILURE;
            }

            SetParameter();
            return Core::BehaviorTreeNodeStatus::SUCCESS;
        }

        /**
         * @brief Sets the value of the parameter. This method need to be implemented in
         * child classes.
         */
        virtual void SetParameter() = 0;

        /**
         * @brief The index of the paramter in the animation graph.
         */
        AZ::u64 m_parameterIndex = INVALID_PARAMETER_INDEX;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Animation

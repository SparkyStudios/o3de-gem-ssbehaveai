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

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
#pragma region DebugMessageNode

    /**
     * @brief Display a debug message in the O3DE console.
     *
     * @par Node Ports
     * - message: The message to display in the debug console.
     * - level: The level of the debug message. Allowed values are "info", "warning", and "error".
     */
    class DebugMessageNode : public Core::Node
    {
    public:
        AZ_CLASS_ALLOCATOR(DebugMessageNode, AZ::SystemAllocator, 0);
        AZ_RTTI(DebugMessageNode, "{A0059957-67F9-43CB-AE02-1576835B0C73}", Core::Node);

        static constexpr const char* NODE_NAME = "DebugMessage";

        static constexpr const char* NODE_PORT_MESSAGE_NAME = "message";
        static constexpr const char* NODE_PORT_MESSAGE_DESCRIPTION = "The message to display in the debug console.";

        static constexpr const char* NODE_PORT_LEVEL_NAME = "level";
        static constexpr const char* NODE_PORT_LEVEL_DESCRIPTION =
            R"(The level of the debug message. Allowed values are "info", "warning", and "error".)";

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry);

        enum class DebugMessageLevel
        {
            Error = 0,
            Warning,
            Info,
            Silent,
        };

        DebugMessageNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config);

        static Core::BehaviorTreePortsList providedPorts();

        std::string NodeCategory() const override
        {
            return "Common";
        }

    protected:
        Core::BehaviorTreeNodeStatus Tick() override;
    };

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyDebugMessageLevel

    class BlackboardPropertyDebugMessageLevel : public Blackboard::BlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(BlackboardPropertyDebugMessageLevel, AZ::SystemAllocator, 0);
        AZ_RTTI(BlackboardPropertyDebugMessageLevel, "{09EF7645-63E9-40AC-88F2-2E489CAF101D}", Blackboard::BlackboardProperty);

        using DebugMessageLevel = DebugMessageNode::DebugMessageLevel;

        static void Reflect(AZ::ReflectContext* context);

        BlackboardPropertyDebugMessageLevel();
        explicit BlackboardPropertyDebugMessageLevel(const char* name);
        BlackboardPropertyDebugMessageLevel(const char* name, const DebugMessageLevel& value);

        [[nodiscard]] const void* GetDataAddress() const override;
        [[nodiscard]] const AZ::Uuid& GetDataTypeUuid() const override;

        BlackboardPropertyDebugMessageLevel* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard::Blackboard& blackboard) const override;
        void SetValueFromString(const char* value) override;

        DebugMessageLevel mValue = DebugMessageLevel::Info;

    protected:
        void CloneDataFrom(const BlackboardProperty* property) override;
    };

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(
        SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel,
        "{57840eec-7a59-4b6e-9c24-66f947280ca8}");
} // namespace AZ

namespace BT
{
    using namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common;

    template<>
    inline std::string toStr<DebugMessageNode::DebugMessageLevel>(DebugMessageNode::DebugMessageLevel value)
    {
        switch (value)
        {
        case DebugMessageNode::DebugMessageLevel::Error:
            return "error";
        case DebugMessageNode::DebugMessageLevel::Warning:
            return "warning";
        default:
        case DebugMessageNode::DebugMessageLevel::Info:
            return "info";
        case DebugMessageNode::DebugMessageLevel::Silent:
            return "silent";
        }
    }

    template<>
    inline DebugMessageNode::DebugMessageLevel convertFromString(StringView str)
    {
        if (str.compare("error") == 0)
        {
            return DebugMessageNode::DebugMessageLevel::Error;
        }

        if (str.compare("warning") == 0)
        {
            return DebugMessageNode::DebugMessageLevel::Warning;
        }

        if (str.compare("info") == 0)
        {
            return DebugMessageNode::DebugMessageLevel::Info;
        }

        if (str.compare("silent") == 0)
        {
            return DebugMessageNode::DebugMessageLevel::Silent;
        }

        return DebugMessageNode::DebugMessageLevel::Info;
    }
} // namespace BT

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

#include <StdAfx.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    void SSBehaviorTreeRegistry::RegisterProperty(
        const AZStd::string& type, const AZ::Uuid& uuid, const SSBehaviorTreeBlackboardPropertyBuilder& builder)
    {
        m_registeredTypeBuilders.insert(AZStd::make_pair(type, builder));
        m_registeredTypeUuids.insert(AZStd::make_pair(type, uuid));
    }

    const SSBehaviorTreeBlackboardPropertyBuilder& SSBehaviorTreeRegistry::GetPropertyBuilder(const AZStd::string& type) const
    {
        if (auto findIt = m_registeredTypeBuilders.find(type); findIt != m_registeredTypeBuilders.end())
            return findIt->second;

        AZ_Error("SSBehaviorTree", false, "Error: Cannot find a property builder for the given type. Did you forget to register?");
        return m_invalidBuilder;
    }

    const AZ::Uuid& SSBehaviorTreeRegistry::GetPropertyUuid(const AZStd::string& type) const
    {
        if (auto findIt = m_registeredTypeUuids.find(type); findIt != m_registeredTypeUuids.end())
            return findIt->second;

        return m_invalidUuid;
    }

    const AZ::Uuid& SSBehaviorTreeRegistry::GetNodeUuid(const AZStd::string& type) const
    {
        if (auto findIt = m_registeredNodeUuids.find(type); findIt != m_registeredNodeUuids.end())
            return findIt->second;

        return m_invalidUuid;
    }

    void SSBehaviorTreeRegistry::EnableNodes(const AZStd::vector<AZStd::string>& nodes)
    {
        AZ::SerializeContext* serializeContext = nullptr;
        EBUS_EVENT_RESULT(serializeContext, AZ::ComponentApplicationBus, GetSerializeContext);

        if (!serializeContext)
        {
            AZ_Error("SSBehaviorTree", false, "Can't get serialize context from component application.");
            return;
        }

        for (const auto& node : nodes)
        {
            AZ_Printf("SSBehaviorTree", "Enabling node: %s\n", node.c_str());
            RegisterNode(node, serializeContext);
        }

        // TODO: Should I need to clear the delayed nodes?
    }

    void SSBehaviorTreeRegistry::EnableNodes()
    {
        AZ::SerializeContext* serializeContext = nullptr;
        EBUS_EVENT_RESULT(serializeContext, AZ::ComponentApplicationBus, GetSerializeContext);

        if (!serializeContext)
        {
            AZ_Error("SSBehaviorTree", false, "Can't get serialize context from component application.");
            return;
        }

        for (const auto& node : m_delayedRegisterers)
        {
            AZ_Printf("SSBehaviorTree", "Enabling node: %s\n", node.first.c_str());
            RegisterNode(node.first, serializeContext);
        }
    }

    AZStd::vector<AZStd::string> SSBehaviorTreeRegistry::GetRegisteredProperties() const
    {
        AZStd::vector<AZStd::string> properties;
        for (const auto& pair : m_registeredTypeBuilders)
        {
            properties.push_back(pair.first);
        }
        return properties;
    }

    AZStd::vector<AZStd::string> SSBehaviorTreeRegistry::GetRegisteredNodes() const
    {
        AZStd::vector<AZStd::string> nodes;
        for (const auto& pair : m_registeredNodeBuilders)
        {
            nodes.push_back(pair.first);
        }
        return nodes;
    }

    void SSBehaviorTreeRegistry::RegisterNode(const AZStd::string& name, AZ::ReflectContext* context)
    {
        if (auto findIt = m_delayedRegisterers.find(name); findIt != m_delayedRegisterers.end())
        {
            if (auto notFoundIt = m_registeredNodeBuilders.find(name); notFoundIt == m_registeredNodeBuilders.end())
            {
                if (context)
                {
                    // If the node is found in the delayed registration list, its reflector is also registered.
                    m_delayedReflectors[name](context);
                }

                // Register the node in the BT factory.
                m_factory->registerBuilder(findIt->second.first, findIt->second.second);

                // Mark the node as registered.
                m_registeredNodeBuilders.insert(AZStd::make_pair(name, findIt->second.second));
            }
        }
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

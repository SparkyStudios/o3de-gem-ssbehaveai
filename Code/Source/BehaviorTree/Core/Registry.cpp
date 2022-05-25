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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

#include <AzCore/Component/ComponentApplicationBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    void Registry::RegisterProperty(const AZStd::string& type, const AZ::Uuid& uuid, const BlackboardPropertyBuilder& builder)
    {
        _registeredTypeBuilders.insert(AZStd::make_pair(type, builder));
        _registeredTypeUuid.insert(AZStd::make_pair(type, uuid));
    }

    const BlackboardPropertyBuilder& Registry::GetPropertyBuilder(const AZStd::string& type) const
    {
        if (const auto findIt = _registeredTypeBuilders.find(type); findIt != _registeredTypeBuilders.end())
            return findIt->second;

        AZ_Error("BehaveAI [BehaviorTree]", false, "Error: Cannot find a property builder for the given type. Did you forget to register?");
        return InvalidBuilder;
    }

    const AZ::Uuid& Registry::GetPropertyUuid(const AZStd::string& type) const
    {
        if (const auto findIt = _registeredTypeUuid.find(type); findIt != _registeredTypeUuid.end())
            return findIt->second;

        return InvalidUuid;
    }

    const AZ::Uuid& Registry::GetNodeUuid(const AZStd::string& type) const
    {
        if (const auto findIt = _registeredNodeUuid.find(type); findIt != _registeredNodeUuid.end())
            return findIt->second;

        return InvalidUuid;
    }

    void Registry::EnableNodes(const AZStd::vector<AZStd::string>& nodes)
    {
        AZ::SerializeContext* serializeContext = nullptr;
        EBUS_EVENT_RESULT(serializeContext, AZ::ComponentApplicationBus, GetSerializeContext);

        if (!serializeContext)
        {
            AZ_Error("BehaveAI [BehaviorTree]", false, "Can't get serialize context from component application.");
            return;
        }

        for (const auto& node : nodes)
        {
            AZ_Printf("BehaveAI [BehaviorTree]", "Enabling node: %s\n", node.c_str());
            RegisterNode(node, serializeContext);
        }

        // TODO: Should I need to clear the delayed nodes?
    }

    void Registry::EnableNodes()
    {
        AZ::SerializeContext* serializeContext = nullptr;
        EBUS_EVENT_RESULT(serializeContext, AZ::ComponentApplicationBus, GetSerializeContext);

        if (!serializeContext)
        {
            AZ_Error("BehaveAI [BehaviorTree]", false, "Can't get serialize context from component application.");
            return;
        }

        for (const auto& node : _delayedRegisterers)
        {
            AZ_Printf("BehaveAI [BehaviorTree]", "Enabling node: %s\n", node.first.c_str());
            RegisterNode(node.first, serializeContext);
        }
    }

    AZStd::vector<AZStd::string> Registry::GetRegisteredProperties() const
    {
        AZStd::vector<AZStd::string> properties;
        for (const auto& pair : _registeredTypeBuilders)
        {
            properties.push_back(pair.first);
        }
        return properties;
    }

    AZStd::vector<AZStd::string> Registry::GetRegisteredNodes() const
    {
        AZStd::vector<AZStd::string> nodes;
        for (const auto& pair : _registeredNodeBuilders)
        {
            nodes.push_back(pair.first);
        }
        return nodes;
    }

    const AZStd::unique_ptr<BT::BehaviorTreeFactory>& Registry::GetNativeFactory() const
    {
        return _factory;
    }

    void Registry::RegisterNode(const AZStd::string& name, AZ::ReflectContext* context)
    {
        if (const auto findIt = _delayedRegisterers.find(name); findIt != _delayedRegisterers.end())
        {
            if (const auto notFoundIt = _registeredNodeBuilders.find(name); notFoundIt == _registeredNodeBuilders.end())
            {
                if (context)
                {
                    // If the node is found in the delayed registration list, its reflector is also registered.
                    _delayedReflectors[name](context);
                }

                // Register the node in the BT factory.
                _factory->registerBuilder(findIt->second.first, findIt->second.second);

                // Mark the node as registered.
                _registeredNodeBuilders.insert(AZStd::make_pair(name, findIt->second.second));
            }
        }
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

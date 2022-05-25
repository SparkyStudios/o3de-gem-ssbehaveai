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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h>

#include <AzCore/std/smart_ptr/make_shared.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    Factory::Factory(Registry* registry)
    {
        if (registry)
            _registry = AZStd::shared_ptr<Registry>(registry);
        else
            _registry = AZStd::make_shared<Registry>();
    }

    Factory::~Factory()
    {
        _registry.reset();
    }

    const AZStd::shared_ptr<Registry>& Factory::GetRegistry() const
    {
        return _registry;
    }

    AZStd::unique_ptr<Blackboard::BlackboardProperty> Factory::CreateProperty(const AZStd::string& type, const char* name) const
    {
        if (const auto findIt = _registry->_registeredTypeBuilders.find(type); findIt != _registry->_registeredTypeBuilders.end())
            return findIt->second(name);

        AZ_Error("BehaveAI [BehaviorTree]", false, "Unable to create the property. Did you forget to register?");
        return nullptr;
    }

    AZStd::unique_ptr<Node> Factory::CreateNode(
        const AZStd::string& name, const AZStd::string& instanceName, const BehaviorTreeNodeConfiguration& config) const
    {
        if (const auto findIt = _registry->_registeredNodeBuilders.find(name); findIt != _registry->_registeredNodeBuilders.end())
        {
            AZStd::unique_ptr<Node> node = findIt->second(instanceName.c_str(), config);
            node->setRegistrationID(name.c_str());
            return node;
        }

        AZ_Error("BehaveAI [BehaviorTree]", false, "Unable to create the node. Did you forget to register?");
        return nullptr;
    }

    BT::Tree Factory::CreateTreeFromText(const AZStd::string& text, const Blackboard::Blackboard& blackboard) const
    {
        return _registry->_factory->createTreeFromText(text.c_str(), blackboard.mBlackboard);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

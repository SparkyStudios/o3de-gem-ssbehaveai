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

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeFactory.h>

#include <AzCore/std/smart_ptr/make_shared.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    SSBehaviorTreeFactory::SSBehaviorTreeFactory(SSBehaviorTreeRegistry* registry)
    {
        if (registry)
            m_registry = AZStd::shared_ptr<SSBehaviorTreeRegistry>(registry);
        else
            m_registry = AZStd::make_shared<SSBehaviorTreeRegistry>();
    }

    SSBehaviorTreeFactory::~SSBehaviorTreeFactory()
    {
        m_registry.reset();
    }

    const AZStd::shared_ptr<SSBehaviorTreeRegistry>& SSBehaviorTreeFactory::GetRegistry() const
    {
        return m_registry;
    }

    AZStd::unique_ptr<Blackboard::SSBehaviorTreeBlackboardProperty> SSBehaviorTreeFactory::CreateProperty(
        const AZStd::string& type, const char* name) const
    {
        if (auto findIt = m_registry->m_registeredTypeBuilders.find(type); findIt != m_registry->m_registeredTypeBuilders.end())
            return findIt->second(name);

        AZ_Error("SSBehaviorTree", false, "Unable to create the property. Did you forget to register?");
        return nullptr;
    }

    AZStd::unique_ptr<SSBehaviorTreeNode> SSBehaviorTreeFactory::CreateNode(
        const AZStd::string& name, const AZStd::string& instanceName, const SSBehaviorTreeNodeConfiguration& config) const
    {
        if (auto findIt = m_registry->m_registeredNodeBuilders.find(name); findIt != m_registry->m_registeredNodeBuilders.end())
        {
            AZStd::unique_ptr<SSBehaviorTreeNode> node = findIt->second(instanceName.c_str(), config);
            node->setRegistrationID(name.c_str());
            return node;
        }

        AZ_Error("SSBehaviorTree", false, "Unable to create the node. Did you forget to register?");
        return nullptr;
    }

    BT::Tree SSBehaviorTreeFactory::CreateTreeFromText(
        const AZStd::string& text, const Blackboard::SSBehaviorTreeBlackboard& blackboard) const
    {
        return m_registry->m_factory->createTreeFromText(text.c_str(), blackboard.m_blackboard);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core

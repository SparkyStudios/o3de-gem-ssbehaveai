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

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes.h>

#include <BehaviorTree/BehaviorTreeSystemComponent.h>

#include <AzCore/Module/DynamicModuleHandle.h>
#include <AzCore/Module/ModuleManager.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Settings/SettingsRegistry.h>

#include <AzFramework/Gem/GemInfo.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    using namespace Blackboard;

    BehaviorTreeSystemComponent::BehaviorTreeSystemComponent()
    {
        if (BehaveBehaviorTreeInterface::Get() == nullptr)
        {
            BehaveBehaviorTreeInterface::Register(this);
        }
    }

    BehaviorTreeSystemComponent::~BehaviorTreeSystemComponent()
    {
        if (BehaveBehaviorTreeInterface::Get() == this)
        {
            BehaveBehaviorTreeInterface::Unregister(this);
        }
    }

    void BehaviorTreeSystemComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const serialize = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            serialize->Class<BehaviorTreeSystemComponent, AZ::Component>()->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<BehaviorTreeSystemComponent>(
                      "Sparky Studios - Behave AI - BehaviorTree", "The Game System Component for the Sparky Studios BehaviorTree engine.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void BehaviorTreeSystemComponent::RegisterDefaultNodes() const
    {
        Nodes::RegisterDefaultNodes(_factory.GetRegistry());
    }

    void BehaviorTreeSystemComponent::RegisterDefaultProperties() const
    {
        Nodes::RegisterDefaultProperties(_factory.GetRegistry());
    }

    void BehaviorTreeSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("BehaveAI_BehaviorTreeService"));
    }

    void BehaviorTreeSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("BehaveAI_BehaviorTreeService"));
    }

    void BehaviorTreeSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void BehaviorTreeSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void BehaviorTreeSystemComponent::Init()
    {
        RegisterDefaultProperties();
        RegisterDefaultNodes();
    }

    void BehaviorTreeSystemComponent::Activate()
    {
        BehaveBehaviorTreeRequestBus::Handler::BusConnect();

        AZStd::vector<AZStd::string> superchargedGems{ "EMotionFX", "LmbrCentral" };

        auto settingsRegistry = AZ::SettingsRegistry::Get();
        if (AZStd::vector<AzFramework::GemInfo> gemInfoList; AzFramework::GetGemsInfo(gemInfoList, *settingsRegistry))
        {
            // Get path to the SSBehaveAI gem root
            auto findBehaviorTreeGem = [](const AzFramework::GemInfo& gemInfo) -> bool
            {
                return gemInfo.m_gemName.starts_with("SSBehaveAI");
            };

            AZ::IO::FixedMaxPath behaviorTreeGemRegistryPath;
            if (const auto behaviorTreeGemFoundIt = AZStd::find_if(gemInfoList.begin(), gemInfoList.end(), findBehaviorTreeGem);
                behaviorTreeGemFoundIt != gemInfoList.end())
            {
                behaviorTreeGemRegistryPath = behaviorTreeGemFoundIt->m_absoluteSourcePaths.front().Append("Registry");
            }

            if (!behaviorTreeGemRegistryPath.empty())
            {
                AZ::SettingsRegistryInterface::Specializations specializationTags;

                auto gemEnumeratedCallback = [&superchargedGems, &specializationTags](const AZ::ModuleData& moduleData) -> bool
                {
                    if (const AZ::DynamicModuleHandle* moduleHandle = moduleData.GetDynamicModuleHandle(); moduleHandle != nullptr)
                    {
                        auto findGemName = [&moduleHandle](const AZStd::string& gemName) -> bool
                        {
                            const AZ::IO::FixedMaxPathString moduleFileStem =
                                AZ::IO::PathView(moduleHandle->GetFilename()).Stem().FixedMaxPathString();

                            return moduleFileStem.starts_with(gemName);
                        };

                        if (const auto gemFoundIt = AZStd::find_if(superchargedGems.begin(), superchargedGems.end(), findGemName);
                            gemFoundIt != superchargedGems.end() && !specializationTags.Contains(*gemFoundIt))
                        {
                            specializationTags.Append(*gemFoundIt);
                        }
                    }

                    return true;
                };

                AZ::ModuleManagerRequestBus::Broadcast(&AZ::ModuleManagerRequests::EnumerateModules, gemEnumeratedCallback);

                settingsRegistry->MergeSettingsFolder(
                    behaviorTreeGemRegistryPath.Native(), specializationTags, AZ_TRAIT_OS_PLATFORM_CODENAME);
            }
        }

        AZStd::vector<AZStd::string> enabledNodes;

        auto availableNodesVisitor = [&enabledNodes, &settingsRegistry](
                                         AZStd::string_view path, AZStd::string_view valueName,
                                         AZ::SettingsRegistryInterface::VisitAction action,
                                         AZ::SettingsRegistryInterface::Type type) -> AZ::SettingsRegistryInterface::VisitResponse
        {
            AZ_UNUSED(valueName);
            AZ_UNUSED(action);
            AZ_UNUSED(type);

            if (AZStd::string nodeName; settingsRegistry->Get(nodeName, path))
                enabledNodes.push_back(nodeName);

            return AZ::SettingsRegistryInterface::VisitResponse::Continue;
        };

        // Collect the list of nodes to enable.
        if (settingsRegistry->Visit(availableNodesVisitor, "/SparkyStudios/AI/Behave/BehaviorTree/AvailableNodes"))
        {
            // Enable all the available nodes.
            _factory.GetRegistry()->EnableNodes(enabledNodes);
        }
    }

    void BehaviorTreeSystemComponent::Deactivate()
    {
        BehaveBehaviorTreeRequestBus::Handler::BusDisconnect();
    }

    const Core::Factory& BehaviorTreeSystemComponent::GetFactory() const
    {
        return _factory;
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree

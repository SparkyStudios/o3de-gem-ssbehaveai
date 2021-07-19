#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes.h>

#include <Blackboard/SSBehaviorTreeBlackboard.h>
#include <SSBehaviorTreeSystemComponent.h>

#include <AzCore/Module/DynamicModuleHandle.h>
#include <AzCore/Module/ModuleManager.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Settings/SettingsRegistry.h>

#include <AzFramework/Gem/GemInfo.h>

namespace SparkyStudios::AI::BehaviorTree
{
    using namespace SparkyStudios::AI::BehaviorTree::Blackboard;

    SSBehaviorTreeSystemComponent::SSBehaviorTreeSystemComponent()
    {
        if (SSBehaviorTreeInterface::Get() == nullptr)
        {
            SSBehaviorTreeInterface::Register(this);
        }
    }

    SSBehaviorTreeSystemComponent::~SSBehaviorTreeSystemComponent()
    {
        if (SSBehaviorTreeInterface::Get() == this)
        {
            SSBehaviorTreeInterface::Unregister(this);
        }
    }

    void SSBehaviorTreeSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<SSBehaviorTreeSystemComponent, AZ::Component>()->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<SSBehaviorTreeSystemComponent>(
                      "Sparky Studios - AI - BehaviorTree", "The Game System Component for the Sparky Studios BehaviorTree engine.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void SSBehaviorTreeSystemComponent::RegisterDefaultNodes()
    {
        // Navigation
        Nodes::Navigation::NavigationFindPathToEntityNode::RegisterNode(m_factory.GetRegistry());

        // Animation
        Nodes::Animation::AnimGraphGetNamedParameterBoolNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterFloatNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterRotationEulerNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterRotationNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterStringNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterVector2Node::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphGetNamedParameterVector3Node::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterBoolNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterFloatNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterRotationEulerNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterRotationNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterStringNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterVector2Node::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::AnimGraphSetNamedParameterVector3Node::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetBlendInTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetBlendOutTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetLoopMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetPlaySpeedNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionGetPlayTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionPlayNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetBlendInTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetBlendOutTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetLoopMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetMirrorMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetPlaySpeedNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetPlayTimeNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetRetargetMotionNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Animation::SimpleMotionSetReverseMotionNode::RegisterNode(m_factory.GetRegistry());

        // Common
        Nodes::Common::DebugMessageNode::RegisterNode(m_factory.GetRegistry());
        Nodes::Common::WaitNode::RegisterNode(m_factory.GetRegistry());
    }

    void SSBehaviorTreeSystemComponent::RegisterDefaultProperties()
    {
        // Register properties
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyNumber>("float");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyNumber>("double");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyNumber>("long");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyNumber>("int");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyString>("AZStd::string");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyEntityRef>("AZ::EntityId");
        m_factory.GetRegistry()->RegisterProperty<SSBehaviorTreeBlackboardPropertyVector2>("AZ::Vector2");
    }

    void SSBehaviorTreeSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("SSBehaviorTreeService"));
    }

    void SSBehaviorTreeSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("SSBehaviorTreeService"));
    }

    void SSBehaviorTreeSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void SSBehaviorTreeSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void SSBehaviorTreeSystemComponent::Init()
    {
        RegisterDefaultProperties();
        RegisterDefaultNodes();
    }

    void SSBehaviorTreeSystemComponent::Activate()
    {
        SSBehaviorTreeRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();

        AZStd::vector<AZStd::string> superchargedGems{ "EMotionFX", "LmbrCentral" };

        auto settingsRegistry = AZ::SettingsRegistry::Get();
        AZStd::vector<AzFramework::GemInfo> gemInfoList;
        if (AzFramework::GetGemsInfo(gemInfoList, *settingsRegistry))
        {
            // Get path to the SSBehaviorTree gem root
            auto FindBehaviorTreeGem = [](const AzFramework::GemInfo& gemInfo)
            {
                return gemInfo.m_gemName.starts_with("SSBehaviorTree");
            };

            AZ::IO::FixedMaxPath behaviorTreeGemRegistryPath;
            if (auto behaviorTreeGemFoundIt = AZStd::find_if(gemInfoList.begin(), gemInfoList.end(), FindBehaviorTreeGem);
                behaviorTreeGemFoundIt != gemInfoList.end())
            {
                behaviorTreeGemRegistryPath = behaviorTreeGemFoundIt->m_absoluteSourcePaths.front().Append("Registry");
            }

            if (!behaviorTreeGemRegistryPath.empty())
            {
                AZ::SettingsRegistryInterface::Specializations specializationTags;

                auto GemEnumeratedCallback = [&gemInfoList, &settingsRegistry, &behaviorTreeGemRegistryPath, &superchargedGems,
                                              &specializationTags](const AZ::ModuleData& moduleData) -> bool
                {
                    if (AZ::DynamicModuleHandle* moduleHandle = moduleData.GetDynamicModuleHandle(); moduleHandle != nullptr)
                    {
                        auto FindGemName = [&moduleHandle, &superchargedGems](const AZStd::string& gemName)
                        {
                            AZ::IO::FixedMaxPathString moduleFileStem =
                                AZ::IO::PathView(moduleHandle->GetFilename()).Stem().FixedMaxPathString();

                            return gemName == moduleFileStem;
                        };

                        if (auto gemFoundIt = AZStd::find_if(superchargedGems.begin(), superchargedGems.end(), FindGemName);
                            gemFoundIt != superchargedGems.end() && !specializationTags.Contains(*gemFoundIt))
                        {
                            specializationTags.Append(*gemFoundIt);
                        }
                    }

                    return true;
                };

                AZ::ModuleManagerRequestBus::Broadcast(&AZ::ModuleManagerRequests::EnumerateModules, GemEnumeratedCallback);

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

            AZStd::string nodeName;
            if (settingsRegistry->Get(nodeName, path))
                enabledNodes.push_back(nodeName);

            return AZ::SettingsRegistryInterface::VisitResponse::Continue;
        };

        // Collect the list of nodes to enable
        settingsRegistry->Visit(availableNodesVisitor, "/SparkyStudios/Gems/AI/BehaviorTree/AvailableNodes");

        // Enable all the available nodes.
        m_factory.GetRegistry()->EnableNodes(enabledNodes);
    }

    void SSBehaviorTreeSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        SSBehaviorTreeRequestBus::Handler::BusDisconnect();
    }

    void SSBehaviorTreeSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

    const Core::SSBehaviorTreeFactory& SSBehaviorTreeSystemComponent::GetFactory() const
    {
        return m_factory;
    }
} // namespace SparkyStudios::AI::BehaviorTree

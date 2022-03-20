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

#include <SSBehaviorTreeComponent.h>

#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/Behave/SSBehaviorTreeBus.h>

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    SSBehaviorTreeComponent::SSBehaviorTreeComponent()
        : m_behaviorTreeAsset(AZ::Data::AssetLoadBehavior::PreLoad)
        , m_running(false)
        , m_tree(BT::Tree())
        , m_btBlackboard()
    {
    }

    SSBehaviorTreeComponent::~SSBehaviorTreeComponent()
    {
        //! Needed to avoid memory read access violation
        m_tree.nodes.clear();
        m_tree.blackboard_stack.clear();
        m_tree.manifests.clear();
    }

    void SSBehaviorTreeComponent::SetBehaviorTree(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& asset)
    {
        UpdateAsset(asset, !m_running);
    }

    void SSBehaviorTreeComponent::Init()
    {
    }

    void SSBehaviorTreeComponent::Activate()
    {
        AZ::TickBus::Handler::BusConnect();

        // If we have valid asset listen for script asset events, like reload
        if (m_behaviorTreeAsset.GetId().IsValid())
        {
            AZ::Data::AssetBus::Handler::BusConnect(m_behaviorTreeAsset.GetId());
            m_behaviorTreeAsset.QueueLoad();
        }

        // Set the attached entity's ID in the tree blackboard
        m_btBlackboard.m_blackboard->set<AZ::EntityId>("entityId", GetEntityId());
    }

    void SSBehaviorTreeComponent::Deactivate()
    {
        UnloadBehaviorTree();

        AZ::Data::AssetBus::Handler::BusDisconnect(m_behaviorTreeAsset.GetId());
        AZ::TickBus::Handler::BusDisconnect();
    }

    void SSBehaviorTreeComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        UpdateAsset(asset);
    }

    void SSBehaviorTreeComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        UpdateAsset(asset, true);
    }

    void SSBehaviorTreeComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
    {
        AZ_UNUSED(time);
        AZ_UNUSED(deltaTime);

        // If the behavior tree is paused
        if (!m_running)
            return;

        // If the behavior tree is empty
        if (!m_tree.rootNode())
        {
            AZ_Warning("SSBehaviorTree", false, "The behavior tree is empty for entity with ID: %s", GetEntityId());
            return;
        }

        // Run the behavior tree
        m_tree.tickRoot();
    }

    int SSBehaviorTreeComponent::GetTickOrder()
    {
        return AZ::TICK_GAME;
    }

    void SSBehaviorTreeComponent::LoadBehaviorTree()
    {
        auto* asset = m_behaviorTreeAsset.GetAs<Assets::SSBehaviorTreeAsset>();
        const AZStd::vector<char>& buffer = asset->GetBuffer();

        if (!buffer.empty())
        {
            AZ_Printf("SSBehaviorTree", "Loading  %s", m_behaviorTreeAsset.GetHint().c_str());

            Core::SSBehaviorTreeFactory factory;
            EBUS_EVENT_RESULT(factory, SSBehaviorTreeRequestBus, GetFactory);

            LoadBlackboard(asset);

            AZStd::string data = AZStd::string(buffer.cbegin(), buffer.cend());
            m_tree = factory.CreateTreeFromText(data, m_btBlackboard);

            AZ_Printf("SSBehaviorTree", "Loaded  %s", m_behaviorTreeAsset.GetHint().c_str());

            m_running = m_tree.rootNode() != nullptr;
        }
    }

    void SSBehaviorTreeComponent::UpdateAsset(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& asset, bool force)
    {
        if (asset && (asset.GetId() != m_behaviorTreeAsset.GetId() || force))
        {
            if (m_behaviorTreeAsset.GetId().IsValid())
            {
                UnloadBehaviorTree();
                AZ::Data::AssetBus::Handler::BusDisconnect(m_behaviorTreeAsset.GetId());
            }

            m_behaviorTreeAsset = asset;

            if (m_behaviorTreeAsset.GetId().IsValid())
            {
                AZ::Data::AssetBus::Handler::BusConnect(m_behaviorTreeAsset.GetId());
            }
        }

        if (!m_running && m_behaviorTreeAsset.IsReady())
        {
            LoadBehaviorTree();
        }
    }

    void SSBehaviorTreeComponent::LoadBlackboard(const Assets::SSBehaviorTreeAsset* asset) const
    {
        AZ_UNUSED(asset);

        for (auto* prop : m_btBlackboard.m_properties)
        {
            prop->AddBlackboardEntry(m_btBlackboard);
        }
    }

    void SSBehaviorTreeComponent::UnloadBehaviorTree()
    {
        m_tree.haltTree();
        m_running = false;
    }

    void SSBehaviorTreeComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            // We may have been reflected by SSBehaviorTreeEditorComponent already, so check first
            if (sc->FindClassData(azrtti_typeid<SSBehaviorTreeComponent>()) == nullptr)
            {
                // Expose component fields
                sc->Class<SSBehaviorTreeComponent, AZ::Component>()
                    ->Version(0)
                    ->Field("blackboard", &SSBehaviorTreeComponent::m_btBlackboard)
                    ->Field("behaviorTree", &SSBehaviorTreeComponent::m_behaviorTreeAsset);

                sc->Class<Blackboard::SSBehaviorTreeBlackboard>()
                    ->Field("name", &Blackboard::SSBehaviorTreeBlackboard::m_name)
                    ->Field("properties", &Blackboard::SSBehaviorTreeBlackboard::m_properties);

                // Reflect all properties
                Blackboard::SSBehaviorTreeBlackboardProperty::Reflect(context);

                Blackboard::SSBehaviorTreeBlackboardPropertyBoolean::Reflect(context);
                Blackboard::SSBehaviorTreeBlackboardPropertyNumber::Reflect(context);
                Blackboard::SSBehaviorTreeBlackboardPropertyString::Reflect(context);

                Blackboard::SSBehaviorTreeBlackboardPropertyEntityRef::Reflect(context);

                Blackboard::SSBehaviorTreeBlackboardPropertyVector2::Reflect(context);
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree

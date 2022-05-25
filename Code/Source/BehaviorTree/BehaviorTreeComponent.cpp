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

#include <BehaviorTree/BehaviorTreeComponent.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/BehaveBehaviorTreeBus.h>

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    BehaviorTreeComponent::BehaviorTreeComponent()
        : _behaviorTreeAsset(AZ::Data::AssetLoadBehavior::PreLoad)
        , _running(false)
        , _tree(BT::Tree())
        , _btBlackboard()
    {
    }

    BehaviorTreeComponent::~BehaviorTreeComponent()
    {
        //! Needed to avoid memory read access violation
        _tree.nodes.clear();
        _tree.blackboard_stack.clear();
        _tree.manifests.clear();
    }

    const AZ::Data::Asset<Assets::BehaviorTreeAsset>& BehaviorTreeComponent::GetBehaviorTree() const
    {
        return _behaviorTreeAsset;
    }

    void BehaviorTreeComponent::SetBehaviorTree(const AZ::Data::Asset<Assets::BehaviorTreeAsset>& asset)
    {
        UpdateAsset(asset, !_running);
    }

    void BehaviorTreeComponent::Init()
    {
    }

    void BehaviorTreeComponent::Activate()
    {
        AZ::TickBus::Handler::BusConnect();

        // If we have valid asset listen for script asset events, like reload
        if (_behaviorTreeAsset.GetId().IsValid())
        {
            AZ::Data::AssetBus::Handler::BusConnect(_behaviorTreeAsset.GetId());
            _behaviorTreeAsset.QueueLoad();
        }

        // Set the attached entity's ID in the tree blackboard
        _btBlackboard.mBlackboard->set<AZ::EntityId>("entityId", GetEntityId());
    }

    void BehaviorTreeComponent::Deactivate()
    {
        UnloadBehaviorTree();

        AZ::Data::AssetBus::Handler::BusDisconnect(_behaviorTreeAsset.GetId());
        AZ::TickBus::Handler::BusDisconnect();
    }

    void BehaviorTreeComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        UpdateAsset(asset);
    }

    void BehaviorTreeComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        UpdateAsset(asset, true);
    }

    void BehaviorTreeComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
    {
        AZ_UNUSED(time);
        AZ_UNUSED(deltaTime);

        // If the behavior tree is paused
        if (!_running)
            return;

        // If the behavior tree is empty
        if (!_tree.rootNode())
        {
            AZ_Warning("BehaveAI [BehaviorTree]", false, "The behavior tree is empty for entity with ID: %s", GetEntityId());
            return;
        }

        // Run the behavior tree
        _tree.tickRoot();
    }

    int BehaviorTreeComponent::GetTickOrder()
    {
        return AZ::TICK_GAME;
    }

    void BehaviorTreeComponent::LoadBehaviorTree()
    {
        const auto* asset = _behaviorTreeAsset.GetAs<Assets::BehaviorTreeAsset>();

        if (const AZStd::vector<char>& buffer = asset->GetBuffer(); !buffer.empty())
        {
            AZ_Printf("BehaveAI [BehaviorTree]", "Loading  %s", _behaviorTreeAsset.GetHint().c_str());

            Core::Factory factory;
            EBUS_EVENT_RESULT(factory, BehaveBehaviorTreeRequestBus, GetFactory);

            LoadBlackboard(asset);

            const auto data = AZStd::string(buffer.cbegin(), buffer.cend());
            _tree = factory.CreateTreeFromText(data, _btBlackboard);

            AZ_Printf("BehaveAI [BehaviorTree]", "Loaded  %s", _behaviorTreeAsset.GetHint().c_str());

            _running = _tree.rootNode() != nullptr;
        }
    }

    void BehaviorTreeComponent::UpdateAsset(const AZ::Data::Asset<Assets::BehaviorTreeAsset>& asset, bool force)
    {
        if (asset && (asset.GetId() != _behaviorTreeAsset.GetId() || force))
        {
            if (_behaviorTreeAsset.GetId().IsValid())
            {
                UnloadBehaviorTree();
                AZ::Data::AssetBus::Handler::BusDisconnect(_behaviorTreeAsset.GetId());
            }

            _behaviorTreeAsset = asset;

            if (_behaviorTreeAsset.GetId().IsValid())
            {
                AZ::Data::AssetBus::Handler::BusConnect(_behaviorTreeAsset.GetId());
            }
        }

        if (!_running && _behaviorTreeAsset.IsReady())
        {
            LoadBehaviorTree();
        }
    }

    void BehaviorTreeComponent::LoadBlackboard(const Assets::BehaviorTreeAsset* asset) const
    {
        AZ_UNUSED(asset);

        for (const auto* prop : _btBlackboard.mProperties)
        {
            prop->AddBlackboardEntry(_btBlackboard);
        }
    }

    void BehaviorTreeComponent::UnloadBehaviorTree()
    {
        _tree.haltTree();
        _running = false;
    }

    void BehaviorTreeComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // We may have been reflected by BehaviorTreeEditorComponent already, so check first
            if (sc->FindClassData(azrtti_typeid<BehaviorTreeComponent>()) == nullptr)
            {
                // Expose component fields
                sc->Class<BehaviorTreeComponent, AZ::Component>()
                    ->Version(0)
                    ->Field("Blackboard", &BehaviorTreeComponent::_btBlackboard)
                    ->Field("BehaviorTree", &BehaviorTreeComponent::_behaviorTreeAsset);

                sc->Class<Blackboard::Blackboard>()
                    ->Field("Name", &Blackboard::Blackboard::mName)
                    ->Field("Properties", &Blackboard::Blackboard::mProperties);

                // Reflect all properties
                Blackboard::BlackboardProperty::Reflect(rc);

                Blackboard::BlackboardPropertyBoolean::Reflect(rc);
                Blackboard::BlackboardPropertyNumber::Reflect(rc);
                Blackboard::BlackboardPropertyString::Reflect(rc);

                Blackboard::BlackboardPropertyEntityRef::Reflect(rc);

                Blackboard::BlackboardPropertyVector2::Reflect(rc);
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree

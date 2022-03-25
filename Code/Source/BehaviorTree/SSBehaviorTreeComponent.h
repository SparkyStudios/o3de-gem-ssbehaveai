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

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>

#include <BehaviorTree/Assets/SSBehaviorTreeAsset.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Memory/SystemAllocator.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class SSBehaviorTreeComponent
        : public AZ::Component
        , public AZ::TickBus::Handler
        , private AZ::Data::AssetBus::Handler
    {
    public:
        AZ_COMPONENT(SSBehaviorTreeComponent, "{428c8a8e-5767-46c7-b320-659debad4f1a}");

        SSBehaviorTreeComponent();
        ~SSBehaviorTreeComponent() override;

        const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& GetBehaviorTree() const
        {
            return m_behaviorTreeAsset;
        }

        void SetBehaviorTree(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& behaviorTree);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        static void Reflect(AZ::ReflectContext* reflection);

    protected:
        //////////////////////////////////////////////////////////////////////////
        // AZ::Data::AssetBus
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::TickBuss
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        int GetTickOrder() override;
        //////////////////////////////////////////////////////////////////////////

        void LoadBehaviorTree();
        void UnloadBehaviorTree();

    private:
        void UpdateAsset(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& asset, bool force = false);
        void LoadBlackboard(const Assets::SSBehaviorTreeAsset* asset) const;

        AZ::Data::Asset<Assets::SSBehaviorTreeAsset> m_behaviorTreeAsset;
        bool m_running;

        BT::Tree m_tree;
        Blackboard::SSBehaviorTreeBlackboard m_btBlackboard;

        friend class SSBehaviorTreeEditorComponent;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree

#pragma once

#include <Assets/SSBehaviorTreeAsset.h>
#include <Blackboard/SSBehaviorTreeBlackboard.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Memory/SystemAllocator.h>

namespace SparkyStudios::AI::BehaviorTree
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
} // namespace SparkyStudios::AI::BehaviorTree

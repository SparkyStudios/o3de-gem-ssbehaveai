#pragma once

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetTypeInfoBus.h>
#include <AzCore/Serialization/ObjectStream.h>

#include <Assets/SSBehaviorTreeAsset.h>

namespace SparkyStudios::AI::BehaviorTree::Assets
{
    class SSBehaviorTreeAssetHandler
        : public AZ::Data::AssetHandler
        , protected AZ::AssetTypeInfoBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeAssetHandler, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeAssetHandler, "{60d37814-1307-4b69-a802-c26aa4b642a1}", AZ::Data::AssetHandler);

        SSBehaviorTreeAssetHandler();
        ~SSBehaviorTreeAssetHandler() override;

        // Inherited via AZ::Data::AssetHandler
        AZ::Data::AssetHandler::LoadResult LoadAssetData(
            const AZ::Data::Asset<AZ::Data::AssetData>& asset, const char* assetPath, const AZ::Data::AssetFilterCB& assetLoadFilterCB);
        AZ::Data::AssetHandler::LoadResult LoadAssetData(
            const AZ::Data::Asset<AZ::Data::AssetData>& asset,
            AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
            const AZ::Data::AssetFilterCB& assetLoadFilterCB) override;
        bool SaveAssetData(const AZ::Data::Asset<AZ::Data::AssetData>& asset, AZ::IO::GenericStream* stream) override;
        bool SaveAssetData(const SSBehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream);
        bool SaveAssetData(const SSBehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream, AZ::DataStream::StreamType streamType);
        AZ::Data::AssetPtr CreateAsset(const AZ::Data::AssetId& id, const AZ::Data::AssetType& type) override;
        void DestroyAsset(AZ::Data::AssetPtr ptr) override;
        void GetHandledAssetTypes(AZStd::vector<AZ::Data::AssetType>& assetTypes) override;

        // Inherited from AZ::AssetTypeInfoBus::Handler
        AZ::Data::AssetType GetAssetType() const override;
        const char* GetAssetTypeDisplayName() const override;
        const char* GetGroup() const override;
        AZ::Uuid GetComponentTypeId() const override;
        void GetAssetTypeExtensions(AZStd::vector<AZStd::string>& extensions) override;

        // Public Methods
        AZStd::string GetExtension() const
        {
            return m_extension;
        }

        // Static Methods
        static AZ::Data::AssetType GetAssetTypeStatic();

    private:
        AZStd::string m_extension = ".ssbt";
    };
} // namespace SparkyStudios::AI::BehaviorTree::Assets

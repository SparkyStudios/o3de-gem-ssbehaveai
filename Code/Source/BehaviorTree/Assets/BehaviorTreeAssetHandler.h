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

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetTypeInfoBus.h>
#include <AzCore/Serialization/ObjectStream.h>

#include <BehaviorTree/Assets/BehaviorTreeAsset.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
{
    class BehaviorTreeAssetHandler
        : public AZ::Data::AssetHandler
        , protected AZ::AssetTypeInfoBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(BehaviorTreeAssetHandler, AZ::SystemAllocator, 0);
        AZ_RTTI(BehaviorTreeAssetHandler, "{60D37814-1307-4B69-A802-C26AA4B642A1}", AZ::Data::AssetHandler);

        BehaviorTreeAssetHandler();
        ~BehaviorTreeAssetHandler() override;

        // AZ::Data::AssetHandler
        LoadResult LoadAssetData(
            const AZ::Data::Asset<AZ::Data::AssetData>& asset, const char* assetPath, const AZ::Data::AssetFilterCB& assetLoadFilterCB);
        LoadResult LoadAssetData(
            const AZ::Data::Asset<AZ::Data::AssetData>& asset,
            AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
            const AZ::Data::AssetFilterCB& assetLoadFilterCB) override;
        bool SaveAssetData(const AZ::Data::Asset<AZ::Data::AssetData>& asset, AZ::IO::GenericStream* stream) override;
        bool SaveAssetData(const BehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream);
        bool SaveAssetData(const BehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream, AZ::DataStream::StreamType streamType);
        AZ::Data::AssetPtr CreateAsset(const AZ::Data::AssetId& id, const AZ::Data::AssetType& type) override;
        void DestroyAsset(AZ::Data::AssetPtr ptr) override;
        void GetHandledAssetTypes(AZStd::vector<AZ::Data::AssetType>& assetTypes) override;

        // AZ::AssetTypeInfoBus::Handler
        AZ::Data::AssetType GetAssetType() const override;
        const char* GetAssetTypeDisplayName() const override;
        const char* GetGroup() const override;
        AZ::Uuid GetComponentTypeId() const override;
        void GetAssetTypeExtensions(AZStd::vector<AZStd::string>& extensions) override;

        // Public Methods
        [[nodiscard]] const AZStd::string& GetExtension() const
        {
            return _extension;
        }

        // Static Methods
        static AZ::Data::AssetType GetAssetTypeStatic();

    private:
        AZStd::string _extension = ".bhbtree";
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Assets

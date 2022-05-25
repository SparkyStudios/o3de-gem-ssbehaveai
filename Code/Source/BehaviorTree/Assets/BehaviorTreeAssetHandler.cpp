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

#include <BehaviorTree/Assets/BehaviorTreeAsset.h>
#include <BehaviorTree/Assets/BehaviorTreeAssetHandler.h>

#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTreeEditorComponent.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/IO/FileIO.h>
#include <AzCore/IO/GenericStreams.h>
#include <AzCore/IO/IOUtils.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/Utils.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/std/string/string_view.h>

#include <AzFramework/StringFunc/StringFunc.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
{
    BehaviorTreeAssetHandler::BehaviorTreeAssetHandler()
    {
        AZ::AssetTypeInfoBus::Handler::BusConnect(GetAssetTypeStatic());
    }

    BehaviorTreeAssetHandler::~BehaviorTreeAssetHandler()
    {
        AZ::AssetTypeInfoBus::Handler::BusDisconnect();
    }

    AZ::Data::AssetHandler::LoadResult BehaviorTreeAssetHandler::LoadAssetData(
        [[maybe_unused]] const AZ::Data::Asset<AZ::Data::AssetData>& asset,
        [[maybe_unused]] AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
        [[maybe_unused]] const AZ::Data::AssetFilterCB& assetLoadFilterCB)
    {
        auto* behaviorTreeAsset = asset.GetAs<BehaviorTreeAsset>();
        AZ_Assert(behaviorTreeAsset, "This should be a .bhbtree file, as this is the only type we process!");

        if (behaviorTreeAsset)
        {
            stream->Seek(0U, AZ::IO::GenericStream::ST_SEEK_BEGIN);

            size_t dataLength = stream->GetLength();
            if (dataLength == 0)
            {
                AZ_Error("SSBehaviorTree", false, "Error loading asset file. The file is empty.");
                return AZ::Data::AssetHandler::LoadResult::Error;
            }

            behaviorTreeAsset->_buffer.clear();
            behaviorTreeAsset->_buffer.resize(dataLength);
            stream->Read(dataLength, behaviorTreeAsset->_buffer.data());

            return AZ::Data::AssetHandler::LoadResult::LoadComplete;
        }

        AZ_Error("SSBehaviorTree", false, "Error loading asset file.");
        return AZ::Data::AssetHandler::LoadResult::Error;
    }

    AZ::Data::AssetHandler::LoadResult BehaviorTreeAssetHandler::LoadAssetData(
        const AZ::Data::Asset<AZ::Data::AssetData>& asset, const char* assetPath, const AZ::Data::AssetFilterCB& assetLoadFilterCB)
    {
        // SSBT files are source assets and should be placed in a source asset directory
        AZStd::shared_ptr<AZ::Data::AssetDataStream> assetDataStream = AZStd::make_shared<AZ::Data::AssetDataStream>();

        AZStd::string fullAssetPath;

        if (AzFramework::StringFunc::Path::IsRelative(assetPath))
        {
            AZStd::string watchFolder;
            bool sourceInfoFound{};
            AZ::Data::AssetInfo assetInfo;
            EBUS_EVENT_RESULT(
                sourceInfoFound, AzToolsFramework::AssetSystemRequestBus, GetSourceInfoBySourcePath, assetPath, assetInfo, watchFolder);

            if (sourceInfoFound)
            {
                AzFramework::StringFunc::Path::Join(watchFolder.data(), assetInfo.m_relativePath.data(), fullAssetPath);
            }
        }
        else
        {
            fullAssetPath = assetPath;
        }

        AZ::IO::FileIOStream stream(fullAssetPath.c_str(), AZ::IO::OpenMode::ModeRead);
        if (!AZ::IO::RetryOpenStream(stream))
        {
            AZ_Warning(
                "SSBehaviorTree", false, "Asset loading for \"%s\" failed because the source file could not be opened.",
                fullAssetPath.data());

            return AZ::Data::AssetHandler::LoadResult::Error;
        }

        // Read the asset into a memory buffer, then hand ownership of the buffer to assetDataStream
        {
            AZ::IO::FileIOStream ioStream;
            if (!ioStream.Open(fullAssetPath.data(), AZ::IO::OpenMode::ModeRead))
            {
                AZ_Warning(
                    "SSBehaviorTree", false, "Asset loading for \"%s\" failed because the source file could not be opened.",
                    fullAssetPath.data());

                return AZ::Data::AssetHandler::LoadResult::Error;
            }

            AZStd::vector<AZ::u8> fileBuffer(ioStream.GetLength());
            size_t bytesRead = ioStream.Read(fileBuffer.size(), fileBuffer.data());
            if (bytesRead != ioStream.GetLength())
            {
                AZ_Warning(
                    "SSBehaviorTree", false, AZStd::string::format("File failed to read completely: %s", fullAssetPath.data()).c_str());

                return AZ::Data::AssetHandler::LoadResult::Error;
            }

            assetDataStream->Open(AZStd::move(fileBuffer));
        }

        if (assetDataStream->IsOpen())
        {
            return LoadAssetData(asset, assetDataStream, assetLoadFilterCB);
        }

        AZ_Error("SSBehaviorTree", false, "Unable to open behavior tree asset with relative path %s", assetPath);
        return AZ::Data::AssetHandler::LoadResult::Error;
    }

    bool BehaviorTreeAssetHandler::SaveAssetData(const AZ::Data::Asset<AZ::Data::AssetData>& asset, AZ::IO::GenericStream* stream)
    {
        return SaveAssetData(asset.GetAs<BehaviorTreeAsset>(), stream);
    }

    bool BehaviorTreeAssetHandler::SaveAssetData(const BehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream)
    {
        return SaveAssetData(assetData, stream, AZ::DataStream::ST_XML);
    }

    bool BehaviorTreeAssetHandler::SaveAssetData(
        const BehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream, AZ::DataStream::StreamType streamType)
    {
        AZ_UNUSED(streamType);

        if (assetData)
        {
            const AZStd::vector<char>& byteBuffer = assetData->GetBuffer();
            bool behaviorTreeAssetSaved = stream->Write(byteBuffer.size(), byteBuffer.data()) == byteBuffer.size();
            return behaviorTreeAssetSaved;
        }

        return false;
    }

    AZ::Data::AssetPtr BehaviorTreeAssetHandler::CreateAsset(const AZ::Data::AssetId& id, const AZ::Data::AssetType& type)
    {
        AZ_UNUSED(type);

        return aznew BehaviorTreeAsset(id);
    }

    void BehaviorTreeAssetHandler::DestroyAsset(AZ::Data::AssetPtr ptr)
    {
        delete ptr;
    }

    void BehaviorTreeAssetHandler::GetHandledAssetTypes(AZStd::vector<AZ::Data::AssetType>& assetTypes)
    {
        assetTypes.push_back(GetAssetType());
    }

    AZ::Data::AssetType BehaviorTreeAssetHandler::GetAssetType() const
    {
        return BehaviorTreeAssetHandler::GetAssetTypeStatic();
    }

    const char* BehaviorTreeAssetHandler::GetAssetTypeDisplayName() const
    {
        return "SS BehaviorTree";
    }

    const char* BehaviorTreeAssetHandler::GetGroup() const
    {
        return "Sparky Studios";
    }

    void BehaviorTreeAssetHandler::GetAssetTypeExtensions(AZStd::vector<AZStd::string>& extensions)
    {
        extensions.push_back(_extension);
    }

    AZ::Uuid BehaviorTreeAssetHandler::GetComponentTypeId() const
    {
        return azrtti_typeid<BehaviorTreeEditorComponent>();
    }

    AZ::Data::AssetType BehaviorTreeAssetHandler::GetAssetTypeStatic()
    {
        return azrtti_typeid<Assets::BehaviorTreeAsset>();
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
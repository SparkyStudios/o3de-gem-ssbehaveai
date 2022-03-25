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

#include <BehaviorTree/Assets/SSBehaviorTreeAsset.h>
#include <BehaviorTree/Assets/SSBehaviorTreeAssetHandler.h>

#include <BehaviorTree/SSBehaviorTreeComponent.h>
#include <BehaviorTree/SSBehaviorTreeEditorComponent.h>

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
    SSBehaviorTreeAssetHandler::SSBehaviorTreeAssetHandler()
    {
        AZ::AssetTypeInfoBus::Handler::BusConnect(GetAssetTypeStatic());
    }

    SSBehaviorTreeAssetHandler::~SSBehaviorTreeAssetHandler()
    {
        AZ::AssetTypeInfoBus::Handler::BusDisconnect();
    }

    AZ::Data::AssetHandler::LoadResult SSBehaviorTreeAssetHandler::LoadAssetData(
        [[maybe_unused]] const AZ::Data::Asset<AZ::Data::AssetData>& asset,
        [[maybe_unused]] AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
        [[maybe_unused]] const AZ::Data::AssetFilterCB& assetLoadFilterCB)
    {
        auto* behaviorTreeAsset = asset.GetAs<SSBehaviorTreeAsset>();
        AZ_Assert(behaviorTreeAsset, "This should be a .ssbt file, as this is the only type we process!");

        if (behaviorTreeAsset)
        {
            stream->Seek(0U, AZ::IO::GenericStream::ST_SEEK_BEGIN);

            size_t dataLength = stream->GetLength();
            if (dataLength == 0)
            {
                AZ_Error("SSBehaviorTree", false, "Error loading asset file. The file is empty.");
                return AZ::Data::AssetHandler::LoadResult::Error;
            }

            behaviorTreeAsset->m_buffer.clear();
            behaviorTreeAsset->m_buffer.resize(dataLength);
            stream->Read(dataLength, behaviorTreeAsset->m_buffer.data());

            return AZ::Data::AssetHandler::LoadResult::LoadComplete;
        }

        AZ_Error("SSBehaviorTree", false, "Error loading asset file.");
        return AZ::Data::AssetHandler::LoadResult::Error;
    }

    AZ::Data::AssetHandler::LoadResult SSBehaviorTreeAssetHandler::LoadAssetData(
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

    bool SSBehaviorTreeAssetHandler::SaveAssetData(const AZ::Data::Asset<AZ::Data::AssetData>& asset, AZ::IO::GenericStream* stream)
    {
        return SaveAssetData(asset.GetAs<SSBehaviorTreeAsset>(), stream);
    }

    bool SSBehaviorTreeAssetHandler::SaveAssetData(const SSBehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream)
    {
        return SaveAssetData(assetData, stream, AZ::DataStream::ST_XML);
    }

    bool SSBehaviorTreeAssetHandler::SaveAssetData(
        const SSBehaviorTreeAsset* assetData, AZ::IO::GenericStream* stream, AZ::DataStream::StreamType streamType)
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

    AZ::Data::AssetPtr SSBehaviorTreeAssetHandler::CreateAsset(const AZ::Data::AssetId& id, const AZ::Data::AssetType& type)
    {
        AZ_UNUSED(type);

        return aznew SSBehaviorTreeAsset(id);
    }

    void SSBehaviorTreeAssetHandler::DestroyAsset(AZ::Data::AssetPtr ptr)
    {
        delete ptr;
    }

    void SSBehaviorTreeAssetHandler::GetHandledAssetTypes(AZStd::vector<AZ::Data::AssetType>& assetTypes)
    {
        assetTypes.push_back(GetAssetType());
    }

    AZ::Data::AssetType SSBehaviorTreeAssetHandler::GetAssetType() const
    {
        return SSBehaviorTreeAssetHandler::GetAssetTypeStatic();
    }

    const char* SSBehaviorTreeAssetHandler::GetAssetTypeDisplayName() const
    {
        return "SS BehaviorTree";
    }

    const char* SSBehaviorTreeAssetHandler::GetGroup() const
    {
        return "Sparky Studios";
    }

    void SSBehaviorTreeAssetHandler::GetAssetTypeExtensions(AZStd::vector<AZStd::string>& extensions)
    {
        extensions.push_back(m_extension);
    }

    AZ::Uuid SSBehaviorTreeAssetHandler::GetComponentTypeId() const
    {
        return azrtti_typeid<SSBehaviorTreeEditorComponent>();
    }

    AZ::Data::AssetType SSBehaviorTreeAssetHandler::GetAssetTypeStatic()
    {
        return azrtti_typeid<Assets::SSBehaviorTreeAsset>();
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
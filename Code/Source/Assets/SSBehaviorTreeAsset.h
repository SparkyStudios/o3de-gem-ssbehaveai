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

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/IO/GenericStreams.h>
#include <AzCore/std/containers/vector.h>

namespace SparkyStudios::AI::BehaviorTree::Assets
{
    /**
     * @brief SS BehaviorTree Asset - contains the source code of a BehaviorTree XML file
     */
    class SSBehaviorTreeAsset : public AZ::Data::AssetData
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeAsset, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeAsset, "{7f72ee8b-0b82-40ae-bd9b-cf34eaf35377}", AZ::Data::AssetData);

        SSBehaviorTreeAsset(const AZ::Data::AssetId& assetId = AZ::Data::AssetId());
        ~SSBehaviorTreeAsset() override = default;

        SSBehaviorTreeAsset(const SSBehaviorTreeAsset& rhs) = delete;
        SSBehaviorTreeAsset& operator=(const SSBehaviorTreeAsset& rhs) = delete;

        char* GetDataPointer()
        {
            return m_buffer.data();
        }

        const AZStd::vector<char>& GetBuffer() const
        {
            return m_buffer;
        }

        AZ::IO::MemoryStream CreateMemoryStream() const
        {
            return { m_buffer.data(), m_buffer.size() };
        }

        const char* GetDebugName()
        {
            return m_debugName.empty() ? nullptr : m_debugName.c_str();
        }

    private:
        AZStd::vector<char> m_buffer;
        AZStd::string m_debugName;

        friend class SSBehaviorTreeAssetHandler;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Assets

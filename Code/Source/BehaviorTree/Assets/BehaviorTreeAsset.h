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

namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
{
    /**
     * @brief SS BehaviorTree Asset - contains the source code of a BehaviorTree XML file
     */
    class BehaviorTreeAsset : public AZ::Data::AssetData
    {
    public:
        AZ_CLASS_ALLOCATOR(BehaviorTreeAsset, AZ::SystemAllocator, 0);
        AZ_RTTI(BehaviorTreeAsset, "{7F72EE8B-0B82-40AE-BD9B-CF34EAF35377}", AZ::Data::AssetData);

        explicit BehaviorTreeAsset(const AZ::Data::AssetId& assetId = AZ::Data::AssetId());
        ~BehaviorTreeAsset() override = default;

        BehaviorTreeAsset(const BehaviorTreeAsset& rhs) = delete;
        BehaviorTreeAsset& operator=(const BehaviorTreeAsset& rhs) = delete;

        char* GetDataPointer()
        {
            return _buffer.data();
        }

        const AZStd::vector<char>& GetBuffer() const
        {
            return _buffer;
        }

        AZ::IO::MemoryStream CreateMemoryStream() const
        {
            return { _buffer.data(), _buffer.size() };
        }

        [[nodiscard]] const char* GetDebugName() const
        {
            return _debugName.empty() ? nullptr : _debugName.c_str();
        }

    private:
        AZStd::vector<char> _buffer;
        AZStd::string _debugName;

        friend class BehaviorTreeAssetHandler;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Assets

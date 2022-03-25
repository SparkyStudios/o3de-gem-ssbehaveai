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

namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
{
    static constexpr const char* _default_content =
        "<root main_tree_to_execute=\"MainTree\">\n"
        "    <BehaviorTree ID=\"MainTree\">\n"
        "        <DebugMessage type=\"warning\" message=\"No valid behavior tree asset provided.\" />"
        "    </BehaviorTree>\n"
        "</root>";

    /**
     * @brief Construct a new SSBehaviorTreeAsset::SSBehaviorTreeAsset object
     *
     * @param assetId The unique ID of the asset.
     */
    SSBehaviorTreeAsset::SSBehaviorTreeAsset(const AZ::Data::AssetId& assetId)
        : AZ::Data::AssetData(assetId)
        , m_buffer(_default_content, _default_content + strlen(_default_content))
    {
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Assets
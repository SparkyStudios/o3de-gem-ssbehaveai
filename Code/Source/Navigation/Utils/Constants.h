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

namespace SparkyStudios::AI::Behave::Navigation
{
    static constexpr char kDefaultNavigationMeshAgentName[] = "Default Agent";

    /**
     * @brief The ID of the default navigation mesh area.
     */
    static constexpr AZ::u8 kDefaultNavigationMeshAreaId = 0;

    /**
     * @brief The name of the default navigation mesh area.
     */
    static constexpr char kDefaultNavigationMeshAreaName[] = "Default Area";

    /**
     * @brief The cost of the default navigation mesh area.
     */
    static constexpr float kDefaultNavigationMeshAreaCost = 1.0f;
} // namespace SparkyStudios::AI::Behave::Navigation

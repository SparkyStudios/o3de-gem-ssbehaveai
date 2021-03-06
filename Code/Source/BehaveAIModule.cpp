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

#include <BehaveAIModuleInterface.h>

namespace SparkyStudios::AI::Behave
{
    class BehaveAIModule : public BehaveAIModuleInterface
    {
    public:
        AZ_RTTI(BehaveAIModule, "{29519363-7A4E-4601-B854-3AC69D83DBD0}", BehaveAIModuleInterface);
        AZ_CLASS_ALLOCATOR(BehaveAIModule, AZ::SystemAllocator, 0);
    };
} // namespace SparkyStudios::AI::Behave

AZ_DECLARE_MODULE_CLASS(Gem_Behave, SparkyStudios::AI::Behave::BehaveAIModule)

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

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>
#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorFlowScene.h>
#endif

namespace SparkyStudios::AI::BehaviorTree::Editor::Core
{
    class EditorFlowView : public QtNodes::FlowView
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(EditorFlowView, AZ::SystemAllocator, 0);
        AZ_RTTI(EditorFlowView, "{a5514cc2-b6a1-4435-83d7-5fb914b763ea}", QtNodes::FlowView);

        EditorFlowView(EditorFlowScene* scene, QWidget* parent = Q_NULLPTR);
        virtual ~EditorFlowView() = default;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core

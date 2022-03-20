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

#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorFlowView.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core
{
    EditorFlowView::EditorFlowView(EditorFlowScene* scene, QWidget* parent)
        : QtNodes::FlowView(scene, parent)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        setDragMode(QGraphicsView::RubberBandDrag);
        setAlignment(Qt::AlignTop | Qt::AlignLeft);
        setCacheMode(QGraphicsView::CacheBackground);
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core

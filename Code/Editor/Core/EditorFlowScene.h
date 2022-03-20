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
#endif

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core
{
    class EditorFlowScene : public QtNodes::FlowScene
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(EditorFlowScene, AZ::SystemAllocator, 0);
        AZ_RTTI(EditorFlowScene, "{83cb2aa3-a4cd-4061-9163-035a457cd75d}", QtNodes::FlowScene);

        EditorFlowScene(std::shared_ptr<QtNodes::DataModelRegistry> registry, QObject* parent = Q_NULLPTR);
        virtual ~EditorFlowScene() = default;

        bool isLocked() const
        {
            return _editor_locked;
        }

        void lock(bool lock_editor)
        {
            _editor_locked = lock_editor;
        }

        QtNodes::Node& CreateNodeAtPosition(const QString& id, const QString& instanceName, QPointF scenePosition);

    private:
        void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
        void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
        void dropEvent(QGraphicsSceneDragDropEvent* event) override;
        void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;

        bool _editor_locked;
        AbstractBehaviorTreeNode _clipboard_node;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core

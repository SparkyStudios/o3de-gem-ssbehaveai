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

#include <QLineEdit>
#include <QObject>
#include <QWidget>

#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorFlowScene.h>
#include <Editor/Core/EditorFlowView.h>
#endif

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    using Core::AbstractBehaviorTree;
    using Core::AbstractBehaviorTreeNode;
    using Core::EditorFlowScene;
    using Core::EditorFlowView;
    using Core::GraphicMode;
    using Core::NodeModel;
    using Core::NodeType;

    class GraphicContainer : public QObject
    {
        Q_OBJECT

    public:
        explicit GraphicContainer(std::shared_ptr<QtNodes::DataModelRegistry> registry, QWidget* parent = nullptr);

        EditorFlowScene* scene()
        {
            return _scene;
        }

        EditorFlowView* view()
        {
            return _view;
        }

        const EditorFlowScene* scene() const
        {
            return _scene;
        }

        const EditorFlowView* view() const
        {
            return _view;
        }

        void lockEditing(bool locked);

        void lockSubtreeEditing(QtNodes::Node& node, bool locked, bool change_style);

        void nodeReorder();

        void zoomHomeView();

        bool containsValidTree() const;

        void clearScene();

        AbstractBehaviorTree loadedTree() const;

        void loadSceneFromTree(const AbstractBehaviorTree& tree);

        void appendTreeToNode(QtNodes::Node& node, AbstractBehaviorTree& subtree);

        void loadFromJson(const QByteArray& data);

        QtNodes::Node* substituteNode(QtNodes::Node* old_node, const QString& new_node_ID);

        void deleteSubTreeRecursively(QtNodes::Node& node);

        std::set<QtNodes::Node*> getSubtreeNodesRecursively(QtNodes::Node& root_node);

        void createSubtree(QtNodes::Node& root_node, QString subtree_name = QString());

    public slots:
        void onNodeDoubleClicked(QtNodes::Node& root_node);
        void onPortValueDoubleClicked(QLineEdit* edit_value);
        void onNodeCreated(QtNodes::Node& node);
        void onNodeContextMenu(QtNodes::Node& node, const QPointF& pos);
        void onConnectionContextMenu(QtNodes::Connection& connection, const QPointF&);
        void onSmartRemove(QtNodes::Node* node);
        void onNodesSelectionChanged();

    signals:
        void addNewModel(const NodeModel& new_model);
        void undoableChange();
        void requestSubTreeExpand(GraphicContainer& container, QtNodes::Node& node);
        void requestSubTreeCreate(AbstractBehaviorTree tree, QString name);

    private:
        EditorFlowScene* _scene;
        EditorFlowView* _view;

        void createMorphSubMenu(QtNodes::Node& node, QMenu* nodeMenu);

        void createSmartRemoveAction(QtNodes::Node& node, QMenu* nodeMenu);

        void insertNodeInConnection(QtNodes::Connection& connection, QString node_name);

        void recursiveLoadStep(
            QPointF& cursor, AbstractBehaviorTree& tree, AbstractBehaviorTreeNode* abs_node, QtNodes::Node* parent_node, int nest_level);

        std::shared_ptr<QtNodes::DataModelRegistry> _model_registry;

        bool _signal_was_blocked;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

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

#include <QFile>
#include <QTreeWidgetItem>
#include <QWidget>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/containers/vector.h>

#include <Editor/Core/EditorBase.h>
#endif

namespace Ui
{
    class NodesSidePanel;
}

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Widgets
{
    using Core::NodeModel;
    using Core::NodeModels;
    using Core::NodeType;

    class NodesSidePanel : public QWidget
    {
        Q_OBJECT

    public:
        explicit NodesSidePanel(const std::shared_ptr<QtNodes::DataModelRegistry>& registry, NodeModels& models, QWidget* parent = nullptr);
        ~NodesSidePanel();

        void updateTreeView();
        void clear();

    public slots:
        void onRemoveModel(QString selected_name);
        void onReplaceModel(const QString& old_name, const NodeModel& new_model);

    private slots:
        void on_nodesListTreeWidget_itemSelectionChanged();
        void on_nodesListFilter_textChanged(const QString& arg1);

    signals:
        void addNewModel(const NodeModel& new_model);
        void modelRemoveRequested(QString ID);
        void nodeModelEdited(QString prev_ID, QString new_ID);
        void addSubtree(QString ID);
        void renameSubtree(QString prev_ID, QString new_ID);
        void destroySubtree(QString ID);

    private:
        Ui::NodesSidePanel* ui;

        NodeModels& _tree_nodes_model;
        std::shared_ptr<QtNodes::DataModelRegistry> _model_registry;
        std::map<QString, QTreeWidgetItem*> _tree_view_category_items;
        std::map<std::pair<QString, QString>, QTreeWidgetItem*> _node_category_items;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Widgets

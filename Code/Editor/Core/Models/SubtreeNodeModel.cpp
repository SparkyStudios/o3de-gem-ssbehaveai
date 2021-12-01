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

#include <Editor/Core/Models/SubtreeNodeModel.h>

#include <QLineEdit>
#include <QVBoxLayout>

namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models
{
    SubtreeNodeModel::SubtreeNodeModel(const NodeModel& model)
        : SSBehaviorTreeNodeDataModel(model)
        , _expanded(false)
    {
        _expand_button = new QPushButton(_expanded ? "Collapse" : "Expand");
        _expand_button->setMaximumWidth(100);

        _expand_button->setStyleSheet("QPushButton{"
                                      "  color: black; background-color: white; "
                                      "  border: 0px rgb(115, 210, 22);"
                                      "  padding: 4px; border-radius: 3px;}\n"
                                      "QPushButton:disabled { color: #303030; background-color: #a0a0a0; }");
        _expand_button->setFlat(false);
        _expand_button->setFocusPolicy(Qt::NoFocus);
        _expand_button->adjustSize();

        connect(
            _expand_button, &QPushButton::clicked, this,
            [this]()
            {
                emit expandButtonPushed();
            });

        updateNodeSize();
    }

    void SubtreeNodeModel::setExpanded(bool expand)
    {
        _expanded = expand;
        _expand_button->setText(_expanded ? "Collapse" : "Expand");
        _expand_button->adjustSize();
    }

    void SubtreeNodeModel::setInstanceName(const QString& name)
    {
        SSBehaviorTreeNodeDataModel::setInstanceName(name);
    }

    QJsonObject SubtreeNodeModel::save() const
    {
        QJsonObject modelJson;
        modelJson["name"] = registrationName();
        modelJson["alias"] = instanceName();
        modelJson["expanded"] = _expanded;

        return modelJson;
    }

    void SubtreeNodeModel::restore(const QJsonObject& modelJson)
    {
        if (registrationName() != modelJson["name"].toString())
        {
            AZ_Error("SSBehaviorTreeEditor", false, "error restoring: different registration_name");
            return;
        }

        QString alias = modelJson["alias"].toString();
        setInstanceName(alias);
        setExpanded(modelJson["expanded"].toBool());
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models

#include <Editor/Core/Models/moc_SubtreeNodeModel.cpp>

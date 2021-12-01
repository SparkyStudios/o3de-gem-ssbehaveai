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

#include <QListWidgetItem>
#include <QWidget>

#include <Editor/Core/EditorBase.h>
#endif

namespace Ui
{
    class BlackboardProperties;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    class BlackboardProperties : public QWidget
    {
        Q_OBJECT

    public:
        explicit BlackboardProperties(Core::BlackboardPropertyModels& models, QWidget* parent);
        ~BlackboardProperties() override;

        void UpdateProperties();

    public slots:
        void OnAddProperty();
        void OnPropertyDoubleClicked(QListWidgetItem* item);

    private:
        Ui::BlackboardProperties* ui;
        Core::BlackboardPropertyModels& _blackboard_properties_model;

        QWidget* _main_window;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

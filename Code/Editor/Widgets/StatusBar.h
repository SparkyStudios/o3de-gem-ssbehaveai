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

#include <QWidget>
#endif

namespace Ui
{
    class StatusBar;
}

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Widgets
{
    class StatusBar : public QWidget
    {
        Q_OBJECT

    public:
        explicit StatusBar(QWidget* parent);
        ~StatusBar();

        void SetOpenedFileName(const QString& filename);

    public slots:
        void OnValidTree();
        void OnInvalidTree();

    private:
        Ui::StatusBar* ui;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Widgets

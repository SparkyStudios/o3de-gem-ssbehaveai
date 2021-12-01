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
#include <AzFramework/Application/Application.h>
#include <QCoreApplication>
#include <QSharedPointer>

#include <Editor/Windows/MainWindow.h>
#endif

namespace AZ
{
    class Entity;
}

namespace SparkyStudios::AI::BehaviorTree::Editor::Application
{
    class Application : public AzFramework::Application
    {
    public:
        using AzFramework::Application::Application;
        virtual ~Application();

        bool Init(bool interactive = true);
        bool Run();
        void TearDown();

    private:
        bool InitLog(const char* logName);

        QSharedPointer<QCoreApplication> m_app;
        QSharedPointer<Windows::MainWindow> m_mainWindow;

        AZ::Entity* m_entity = nullptr;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Application

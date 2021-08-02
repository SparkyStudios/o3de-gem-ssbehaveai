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

#include <Editor/Application/Application.h>

#include <AzCore/IO/FileIO.h>
#include <AzCore/Settings/SettingsRegistryMergeUtils.h>
#include <AzCore/Utils/Utils.h>

#include <AzFramework/Logging/LoggingComponent.h>

#include <AzQtComponents/Components/StyleManager.h>
#include <AzQtComponents/Components/WindowDecorationWrapper.h>
#include <AzQtComponents/Utilities/HandleDpiAwareness.h>

#include <QApplication>
#include <QDir>
#include <QMessageBox>

#include <SSBehaviorTreeSystemComponent.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Application
{
    Application::~Application()
    {
        TearDown();
    }

    bool Application::Init([[maybe_unused]] bool interactive)
    {
        constexpr const char* applicationName{ "Sparky Studios - BehaviorTree Editor" };
        constexpr const char* logName{ "SSBTEditorEventLog" };

        QApplication::setOrganizationName("Sparky Studios");
        QApplication::setOrganizationDomain("sparky-studios.com");

        QCoreApplication::setApplicationName(applicationName);
        QCoreApplication::setApplicationVersion("1.0");

        // Use the LogComponent for non-dev logging log
        RegisterComponentDescriptor(AzFramework::LogComponent::CreateDescriptor());
        // Use the BehaviorTree system component
        RegisterComponentDescriptor(SparkyStudios::AI::BehaviorTree::SSBehaviorTreeSystemComponent::CreateDescriptor());

        // Set the log alias to .o3de/Logs instead of the default user/logs
        AZ::IO::FixedMaxPath path = AZ::Utils::GetO3deLogsDirectory();

        // DevWriteStorage is where the event log is written during development
        m_settingsRegistry->Set(AZ::SettingsRegistryMergeUtils::FilePathKey_DevWriteStorage, path.LexicallyNormal().Native());

        // Save event logs to .o3de/Logs/eventlogger/EventLogO3DE.azel
        m_settingsRegistry->Set(AZ::SettingsRegistryMergeUtils::BuildTargetNameKey, logName);

        Start(AzFramework::Application::Descriptor());

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

        QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        AzQtComponents::Utilities::HandleDpiAwareness(AzQtComponents::Utilities::SystemDpiAware);

        // Create the actual Qt Application - this needs to happen before using QMessageBox
        m_app.reset(new QApplication(*GetArgC(), *GetArgV()));

        if (!InitLog(logName))
        {
            AZ_Warning("SSBehaviorTreeEditor", false, "Failed to init logging.");
        }

        const AZ::CommandLine* commandLine = GetCommandLine();
        AZ_Assert(commandLine, "Failed to get command line");

        AZ::IO::FixedMaxPath projectPath;
        if (size_t projectSwitchCount = commandLine->GetNumSwitchValues("project-path"); projectSwitchCount > 0)
        {
            projectPath = commandLine->GetSwitchValue("project-path", projectSwitchCount - 1).c_str();
        }

        m_mainWindow.reset(new Windows::MainWindow(nullptr, projectPath));

        return true;
    }

    bool Application::InitLog(const char* logName)
    {
        if (!m_entity)
        {
            // override the log alias to the O3de Logs directory instead of the default project user/Logs folder
            AZ::IO::FixedMaxPath path = AZ::Utils::GetO3deLogsDirectory();
            AZ::IO::FileIOBase* fileIO = AZ::IO::FileIOBase::GetInstance();
            AZ_Assert(fileIO, "Failed to get FileIOBase instance");

            fileIO->SetAlias("@log@", path.LexicallyNormal().Native().c_str());

            // this entity exists because we need a home for LogComponent
            // and cannot use the system entity because we need to be able to call SetLogFileBaseName
            // so the log will be named O3DE.log
            m_entity = aznew AZ::Entity("Application Entity");

            if (m_entity)
            {
                AzFramework::LogComponent* logger = aznew AzFramework::LogComponent();
                AZ_Assert(logger, "Failed to create LogComponent");
                logger->SetLogFileBaseName(logName);
                m_entity->AddComponent(logger);
                m_entity->Init();
                m_entity->Activate();
            }
        }

        return m_entity != nullptr;
    }

    void Application::TearDown()
    {
        if (m_entity)
        {
            m_entity->Deactivate();
            delete m_entity;
            m_entity = nullptr;
        }

        m_mainWindow.reset();
        m_app.reset();
    }

    bool Application::Run()
    {
        // Set up the Style Manager
        AzQtComponents::StyleManager styleManager(qApp);
        styleManager.initialize(qApp, GetEngineRoot());

        // the decoration wrapper is intended to remember window positioning and sizing
        auto wrapper = new AzQtComponents::WindowDecorationWrapper();
        wrapper->setGuest(m_mainWindow.data());
        wrapper->show();
        m_mainWindow->show();

        qApp->setQuitOnLastWindowClosed(true);

        // Run the application
        return qApp->exec();
    }

} // namespace SparkyStudios::AI::BehaviorTree::Editor::Application

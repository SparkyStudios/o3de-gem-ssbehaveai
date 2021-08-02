#include <Editor/Application/Application.h>

#include <AzQtComponents/Utilities/QtPluginPaths.h>

int main(int argc, char* argv[])
{
    int runSuccess = 0;

    // Call before using any Qt, or the app may not be able to locate Qt libs
    AzQtComponents::PrepareQtPaths();

    SparkyStudios::AI::BehaviorTree::Editor::Application::Application application(&argc, &argv);
    if (!application.Init())
    {
        AZ_Error("SSBehaviorTreeVisualEditor", false, "Failed to initialize");
        runSuccess = 1;
    }
    else
    {
        runSuccess = application.Run() ? 0 : 1;
    }

    return runSuccess;
}

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

#include <Editor/Application/Application.h>

#include <AzQtComponents/Utilities/QtPluginPaths.h>

int main(int argc, char* argv[])
{
    int runSuccess = 0;

    // Call before using any Qt, or the app may not be able to locate Qt libs
    AzQtComponents::PrepareQtPaths();

    SparkyStudios::AI::Behave::BehaviorTree::Editor::Application::Application application(&argc, &argv);
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

# Copyright (c) 2021-present Sparky Studios. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Currently we are in the Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: ly_get_list_relative_pal_filename will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here in our gem : Gems/SSBehaveAI/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Gems/SSBehaveAI/Code
o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" "${gem_path}" "${gem_name}")

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this gem
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

if(NOT PAL_TRAIT_SSBEHAVEAI_SUPPORTED)
    include(ssbehaveai_unsuported.cmake)
    return()
endif()

# Add the Behave.Core.Static target
# Note: We include the common files and the platform specific files which are set in ssbehaveai_files.cmake
# and in ${pal_dir}/ssbehaveai_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME Behave.Core.Static STATIC
    NAMESPACE SparkyStudios::AI
    OUTPUT_NAME SparkyStudios.AI.Behave.Static
    FILES_CMAKE
        ssbehaveai_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
            ../External/RecastNavigation/Detour/Include
            ../External/RecastNavigation/DetourCrowd/Include
            ../External/RecastNavigation/DetourTileCache/Include
            ../External/RecastNavigation/Recast/Include
        PRIVATE
            .
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::LmbrCentral.Static
            Gem::EMotionFX.Static
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
            3rdParty::BehaviorTreeCPP
)

# Here add Behave.Core target, it depends on the Behave.Core.Static
ly_add_target(
    NAME Behave.Core ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE SparkyStudios::AI
    OUTPUT_NAME SparkyStudios.AI.Behave
    FILES_CMAKE
        ssbehaveai_shared_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            .
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            SparkyStudios::AI::Behave.Core.Static
)

# By default, we will specify that the above target SSBehaveAI would be used by
# Client and Server type targets when this gem is enabled.  If you don't want it
# active in Clients or Servers by default, delete one of both of the following lines:
ly_create_alias(NAME SSBehaveAI.Clients NAMESPACE Gem TARGETS SparkyStudios::AI::Behave.Core)
ly_create_alias(NAME SSBehaveAI.Servers NAMESPACE Gem TARGETS SparkyStudios::AI::Behave.Core)

# If we are on a host platform, we want to add the host tools targets like the Behave.Editor target which
# will also depend on Behave.Core.Static
if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME Behave.Editor.Static STATIC
        NAMESPACE SparkyStudios::AI
        OUTPUT_NAME SparkyStudios.AI.Behave.Editor.Static
        FILES_CMAKE
            ssbehaveai_editor_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                .
                Source
            PUBLIC
                Include
                ../External/RecastNavigation/DebugUtils/Include
        BUILD_DEPENDENCIES
            PUBLIC
                AZ::AzToolsFramework
                AZ::AzFramework
                AZ::AzQtComponents
                SparkyStudios::AI::Behave.Core.Static
                3rdParty::Qt::Widgets
                3rdParty::QtNodeEditor
        COMPILE_DEFINITIONS
            PRIVATE
                SSBEHAVEAI_EDITOR
                NODE_EDITOR_STATIC
    )

    ly_add_target(
        NAME Behave.Editor GEM_MODULE
        NAMESPACE SparkyStudios::AI
        AUTOMOC
        AUTORCC
        OUTPUT_NAME SparkyStudios.AI.Behave.Editor
        FILES_CMAKE
            ssbehaveai_editor_shared_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                .
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                SparkyStudios::AI::Behave.Editor.Static
        COMPILE_DEFINITIONS
            PRIVATE
                SSBEHAVEAI_EDITOR
                NODE_EDITOR_STATIC
    )

    ly_add_target(
        NAME Behave.Editor.App EXECUTABLE
        OUTPUT_NAME BehaviorTree.Editor
        NAMESPACE SparkyStudios::AI
        AUTOMOC
        AUTOUIC
        AUTORCC
        FILES_CMAKE
            ssbehaveai_app_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Include
                .
                Source
        BUILD_DEPENDENCIES
            PRIVATE
                SparkyStudios::AI::Behave.Editor.Static
                3rdParty::QtNodeEditor
                3rdParty::Qt::Core
                3rdParty::Qt::Concurrent
                3rdParty::Qt::Widgets
                AZ::AzCore
                AZ::AzFramework
                AZ::AzQtComponents
                AZ::AzToolsFramework
                Gem::LmbrCentral.Static
                Gem::EMotionFX.Static
        COMPILE_DEFINITIONS
            PRIVATE
                SSBEHAVEAI_EDITOR
                NODE_EDITOR_STATIC
    )

    add_definitions(-DSS_PROJECT_RUNTIME_OUTPUT_DIRECTORY="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    ly_set_gem_variant_to_load(TARGETS Behave.Editor.App VARIANTS Tools)

    # By default, we will specify that the above target SSBehaveAI would be used by
    # Tool and Builder type targets when this gem is enabled.  If you don't want it
    # active in Tools or Builders by default, delete one of both of the following lines:
    ly_create_alias(NAME SSBehaveAI.Tools    NAMESPACE Gem TARGETS SparkyStudios::AI::Behave.Editor)
    ly_create_alias(NAME SSBehaveAI.Builders NAMESPACE Gem TARGETS SparkyStudios::AI::Behave.Editor)

    # Add build dependency to Editor for the SSBehaveAI Editor application since
    # Editor opens up the Behave.Editor executable
    ly_add_dependencies(Editor SparkyStudios::AI::Behave.Editor.App)

    # Inject the project path into the Behave.Editor VS debugger command arguments if the build system being invoked
    # in a project centric view
    if(NOT PROJECT_NAME STREQUAL "O3DE")
        set_property(TARGET Behave.Editor.App APPEND PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "--project-path=\"${CMAKE_SOURCE_DIR}\"")
    endif()
endif()

################################################################################
# Tests
################################################################################
# See if globally, tests are supported
if(PAL_TRAIT_BUILD_TESTS_SUPPORTED)
    # We globally support tests, see if we support tests on this platform for Behave.Core.Static
    if(PAL_TRAIT_SSBEHAVEAI_TEST_SUPPORTED)
        # We support Behaviore.Tests on this platform, add Behave.Tests target which depends on Behave.Core.Static
        ly_add_target(
            NAME Behave.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
            NAMESPACE SparkyStudios::AI
            OUTPUT_NAME SparkyStudios.AI.BehaviorTree.Tests
            FILES_CMAKE
                ssbehaveai_files.cmake
                ssbehaveai_tests_files.cmake
            INCLUDE_DIRECTORIES
                PRIVATE
                    Tests
                    .
                    Source
            BUILD_DEPENDENCIES
                PRIVATE
                    AZ::AzTest
                    AZ::AzFramework
                    Gem::LmbrCentral.Static
                    Gem::EMotionFX.Static
                    SparkyStudios::AI::Behave.Core.Static
        )

        # Add Behave.Tests to googletest
        ly_add_googletest(
            NAME SparkyStudios::AI::Behave.Tests
        )
    endif()

    # If we are a host platform we want to add tools test like editor tests here
    if(PAL_TRAIT_BUILD_HOST_TOOLS)
        # We are a host platform, see if Editor tests are supported on this platform
        if(PAL_TRAIT_SSBEHAVEAI_EDITOR_TEST_SUPPORTED)
            # We support Behave.Editor.Tests on this platform, add Behave.Editor.Tests target which depends on Behave.Editor
            ly_add_target(
                NAME Behave.Editor.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
                NAMESPACE SparkyStudios::AI
                OUTPUT_NAME SparkyStudios.AI.BehaviorTree.Editor.Tests
                FILES_CMAKE
                    ssbehaveai_editor_tests_files.cmake
                INCLUDE_DIRECTORIES
                    PRIVATE
                        Tests
                        .
                        Source
                BUILD_DEPENDENCIES
                    PRIVATE
                        AZ::AzTest
                        SparkyStudios::AI::Behave.Editor
            )

            # Add Behave.Editor.Tests to googletest
            ly_add_googletest(
                NAME SparkyStudios::AI::Behave.Editor.Tests
            )
        endif()
    endif()
endif()

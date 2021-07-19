
# Currently we are in the Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: ly_get_list_relative_pal_filename will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here in our gem : Gems/SSBehaviorTree/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Gems/SSBehaviorTree/Code
ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} ${o3de_gem_restricted_path} ${o3de_gem_path} ${o3de_gem_name})

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this gem
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

if(NOT PAL_TRAIT_SSBEHAVIORTREE_SUPPORTED)
    include(ssbehaviortree_unsuported.cmake)
    return()
endif()

# Add the SSBehaviorTree.Static target
# Note: We include the common files and the platform specific files which are set in ssbehaviortree_common_files.cmake
# and in ${pal_dir}/ssbehaviortree_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME SSBehaviorTree.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        ssbehaviortree_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
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

# Here add SSBehaviorTree target, it depends on the SSBehaviorTree.Static
ly_add_target(
    NAME SSBehaviorTree ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        ssbehaviortree_shared_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            .
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::SSBehaviorTree.Static
)

# By default, we will specify that the above target SSBehaviorTree would be used by
# Client and Server type targets when this gem is enabled.  If you don't want it
# active in Clients or Servers by default, delete one of both of the following lines:
ly_create_alias(NAME SSBehaviorTree.Clients NAMESPACE Gem TARGETS Gem::SSBehaviorTree)
ly_create_alias(NAME SSBehaviorTree.Servers NAMESPACE Gem TARGETS Gem::SSBehaviorTree)

# If we are on a host platform, we want to add the host tools targets like the SSBehaviorTree.Editor target which
# will also depend on SSBehaviorTree.Static
if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME SSBehaviorTree.Editor.Static STATIC
        NAMESPACE Gem
        FILES_CMAKE
            ssbehaviortree_editor_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                .
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                AZ::AzToolsFramework
                Gem::SSBehaviorTree.Static
        COMPILE_DEFINITIONS
            PRIVATE
                SSBEHAVIORTREE_EDITOR
    )

    ly_add_target(
        NAME SSBehaviorTree.Editor GEM_MODULE
        NAMESPACE Gem
        AUTOMOC
        OUTPUT_NAME Gem.SSBehaviorTree.Editor
        FILES_CMAKE
            ssbehaviortree_editor_shared_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                .
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                Gem::SSBehaviorTree.Editor.Static
    )

    # By default, we will specify that the above target SSBehaviorTree would be used by
    # Tool and Builder type targets when this gem is enabled.  If you don't want it
    # active in Tools or Builders by default, delete one of both of the following lines:
    ly_create_alias(NAME SSBehaviorTree.Tools    NAMESPACE Gem TARGETS Gem::SSBehaviorTree.Editor)
    ly_create_alias(NAME SSBehaviorTree.Builders NAMESPACE Gem TARGETS Gem::SSBehaviorTree.Editor)


endif()

################################################################################
# Tests
################################################################################
# See if globally, tests are supported
if(PAL_TRAIT_BUILD_TESTS_SUPPORTED)
    # We globally support tests, see if we support tests on this platform for SSBehaviorTree.Static
    if(PAL_TRAIT_SSBEHAVIORTREE_TEST_SUPPORTED)
        # We support SSBehaviorTree.Tests on this platform, add SSBehaviorTree.Tests target which depends on SSBehaviorTree.Static
        ly_add_target(
            NAME SSBehaviorTree.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
            NAMESPACE Gem
            FILES_CMAKE
                ssbehaviortree_files.cmake
                ssbehaviortree_tests_files.cmake
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
                    Gem::SSBehaviorTree.Static
        )

        # Add SSBehaviorTree.Tests to googletest
        ly_add_googletest(
            NAME Gem::SSBehaviorTree.Tests
        )
    endif()

    # If we are a host platform we want to add tools test like editor tests here
    if(PAL_TRAIT_BUILD_HOST_TOOLS)
        # We are a host platform, see if Editor tests are supported on this platform
        if(PAL_TRAIT_SSBEHAVIORTREE_EDITOR_TEST_SUPPORTED)
            # We support SSBehaviorTree.Editor.Tests on this platform, add SSBehaviorTree.Editor.Tests target which depends on SSBehaviorTree.Editor
            ly_add_target(
                NAME SSBehaviorTree.Editor.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
                NAMESPACE Gem
                FILES_CMAKE
                    ssbehaviortree_editor_tests_files.cmake
                INCLUDE_DIRECTORIES
                    PRIVATE
                        Tests
                        .
                        Source
                BUILD_DEPENDENCIES
                    PRIVATE
                        AZ::AzTest
                        Gem::SSBehaviorTree.Editor
            )

            # Add SSBehaviorTree.Editor.Tests to googletest
            ly_add_googletest(
                NAME Gem::SSBehaviorTree.Editor.Tests
            )
        endif()
    endif()
endif()

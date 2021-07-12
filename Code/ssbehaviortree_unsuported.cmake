ly_add_target(
    NAME SSBehaviorTree.Stub ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        ssbehaviortree_stub_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            AZ::AzCore
)
add_library(Gem::SSBehaviorTree ALIAS SSBehaviorTree.Stub) 

if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME SSBehaviorTree.Editor.Stub GEM_MODULE

        NAMESPACE Gem
        FILES_CMAKE
            ssbehaviortree_stub_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
        COMPILE_DEFINITIONS
            PRIVATE
                SSBEHAVIORTREE_EDITOR
        BUILD_DEPENDENCIES
            PRIVATE
                AZ::AzCore
    )
    add_library(Gem::SSBehaviorTree.Editor ALIAS SSBehaviorTree.Editor.Stub) 
endif()

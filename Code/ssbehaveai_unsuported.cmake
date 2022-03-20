ly_add_target(
    NAME Behave.Stub ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE SparkyStudios::AI
    FILES_CMAKE
        ssbehaveai_stub_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            AZ::AzCore
)

add_library(SparkyStudios::AI::Behave ALIAS Behave.Stub) 

if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME Behave.Editor.Stub GEM_MODULE

        NAMESPACE SparkyStudios::AI
        FILES_CMAKE
            ssbehaveai_stub_files.cmake
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

    add_library(SparkyStudios::AI::Behave.Editor ALIAS Behave.Editor.Stub) 
endif()

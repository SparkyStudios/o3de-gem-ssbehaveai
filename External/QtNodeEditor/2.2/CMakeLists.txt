cmake_minimum_required(VERSION 3.8)

project(NodeEditor CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

get_directory_property(_has_parent PARENT_DIRECTORY)
if(_has_parent)
  set(is_root_project OFF)
else()
  set(is_root_project ON)
endif()

set(NE_DEVELOPER_DEFAULTS "${is_root_project}" CACHE BOOL "Turns on default settings for development of NodeEditor")

option(BUILD_SHARED_LIBS "Build as shared library" OFF)
option(BUILD_DEBUG_POSTFIX_D "Append _d suffix to debug libraries" ON)

if(MSVC)
    set( RUNTIME_OUTPUT_DIRECTORY "win" )
elseif(APPLE)
    set( RUNTIME_OUTPUT_DIRECTORY "osx" )
else()
    set( RUNTIME_OUTPUT_DIRECTORY "linux" )
endif()

if(NE_DEVELOPER_DEFAULTS)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

if(BUILD_DEBUG_POSTFIX_D)
  set(CMAKE_DEBUG_POSTFIX "_d")
endif()

# add_subdirectory(external)

# Find the QtWidgets library
find_package(Qt5 5.3 COMPONENTS
             Core
             Widgets
             Gui
             OpenGL)

qt5_add_resources(RESOURCES ./resources/resources.qrc)

# Unfortunately, as we have a split include/src, AUTOMOC doesn't work.
# We'll have to manually specify some files
set(CMAKE_AUTOMOC ON)

set(CPP_SOURCE_FILES
  src/Connection.cpp
  src/ConnectionBlurEffect.cpp
  src/ConnectionGeometry.cpp
  src/ConnectionGraphicsObject.cpp
  src/ConnectionPainter.cpp
  src/ConnectionState.cpp
  src/ConnectionStyle.cpp
  src/DataModelRegistry.cpp
  src/FlowScene.cpp
  src/FlowView.cpp
  src/FlowViewStyle.cpp
  src/Node.cpp
  src/NodeConnectionInteraction.cpp
  src/NodeDataModel.cpp
  src/NodeGeometry.cpp
  src/NodeGraphicsObject.cpp
  src/NodePainter.cpp
  src/NodeState.cpp
  src/NodeStyle.cpp
  src/Properties.cpp
  src/StyleCollection.cpp
)

# If we want to give the option to build a static library,
# set BUILD_SHARED_LIBS option to OFF
add_library(nodes
  ${CPP_SOURCE_FILES}
  ${RESOURCES}
)
add_library(NodeEditor::nodes ALIAS nodes)

target_include_directories(nodes
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/nodes/internal>
)

target_link_libraries(nodes
  PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::OpenGL
)

target_compile_definitions(nodes
  PUBLIC
    ${Qt5Widgets_DEFINITIONS}
    NODE_EDITOR_SHARED
  PRIVATE
    NODE_EDITOR_EXPORTS
    #NODE_DEBUG_DRAWING
    QT_NO_KEYWORDS
)

target_compile_options(nodes
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4127 /EHsc>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

if(NOT "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  # Clang-Cl on MSVC identifies as "Clang" but behaves more like MSVC:
  target_compile_options(nodes
    PRIVATE
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  )
endif()

target_compile_features(nodes PUBLIC cxx_std_14)

set_target_properties(nodes
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

######
# Moc
##

file(GLOB_RECURSE HEADERS_TO_MOC include/nodes/internal/*.hpp)

qt5_wrap_cpp(nodes_moc
    ${HEADERS_TO_MOC}
  TARGET nodes
  OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
)

target_sources(nodes PRIVATE ${nodes_moc})

###############
# Installation
##

include(GNUInstallDirs)

set(CMAKE_INSTALL_LIBDIR   "lib/${RUNTIME_OUTPUT_DIRECTORY}")
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/NodeEditor)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS nodes
  EXPORT NodeEditorTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT NodeEditorTargets
  FILE NodeEditorTargets.cmake
  NAMESPACE NodeEditor::
  DESTINATION ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/NodeEditorConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/NodeEditorConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/NodeEditorConfig.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

cmake_minimum_required(VERSION 3.26)
project(sktop_lite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(lib)

find_package(PkgConfig REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGLWidgets
        REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(YAML REQUIRED yaml-cpp)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${YAML_INCLUDE_DIRS})
include_directories(lib)
include_directories(lib/live/components)
include_directories(lib/local/components)

include_directories(thirdparty/lib)
link_directories(thirdparty/lib)

add_executable(sktop_lite
        main.cpp
        MainPanel.cpp
        MainPanel.h
        MainPanel.ui
)

add_dependencies(sktop_lite
        live
        review)

target_link_libraries(sktop_lite
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(sktop_lite
        ${GST_LIBRARIES}
        ${YAML_LIBRARIES}
        live
        review)
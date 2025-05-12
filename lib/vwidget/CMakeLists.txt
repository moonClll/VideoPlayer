cmake_minimum_required(VERSION 3.26)
project(vwidget)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GST_PBU REQUIRED gstreamer-pbutils-1.0)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${GST_VIDEO_INCLUDE_DIRS})
include_directories(${GST_PBU_INCLUDE_DIRS})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)

add_library(vwidget
        VWidget.h
        VWidget.cpp
        VWidget.ui
)

qt6_add_resources(vwidget "image"
        PREFIX "/"
        FILES Mask.png
)

target_link_libraries(vwidget
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Concurrent
        ${GST_LIBRARIES}
        ${GST_VIDEO_LIBRARIES}
        ${GST_PBU_LIBRARIES}
)
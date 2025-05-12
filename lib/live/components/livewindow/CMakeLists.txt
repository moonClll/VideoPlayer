cmake_minimum_required(VERSION 3.26)
project(livewindow)

add_subdirectory(lib)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${GST_VIDEO_INCLUDE_DIRS})
include_directories(${GST_PBU_INCLUDE_DIRS})
include_directories(../../../)
include_directories(lib)

add_library(livewindow
        LiveWindow.h
        LiveWindow.cpp
        LiveWindow.ui
)

add_dependencies(livewindow vwidget liveplayer)

target_link_libraries(livewindow
        PUBLIC
        ${GST_LIBRARIES}
        ${GST_VIDEO_LIBRARIES}
        ${GST_PBU_LIBRARIES}
        vwidget
        liveplayer
)
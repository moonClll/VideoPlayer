cmake_minimum_required(VERSION 3.26)
project(live)

add_subdirectory(components)

include_directories(components)
include_directories(${GST_INCLUDE_DIRS})
include_directories(${GST_VIDEO_INCLUDE_DIRS})
include_directories(${GST_PBU_INCLUDE_DIRS})
include_directories(../)

add_library(live
        LivePanel.h
        LivePanel.cpp
        LivePanel.ui
)

add_dependencies(live vmdiarea livewindow)

target_link_libraries(live
        livewindow
        vmdiarea
)
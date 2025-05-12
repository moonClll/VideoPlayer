cmake_minimum_required(VERSION 3.26)
project(review)

add_subdirectory(lib)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${GST_VIDEO_INCLUDE_DIRS})
include_directories(${GST_PBU_INCLUDE_DIRS})
include_directories(../)

add_library(review
        ReviewPanel.h
        ReviewPanel.cpp
        ReviewPanel.ui
)

add_dependencies(review vwidget reviewplayer)

target_link_libraries(review
        ${GST_LIBRARIES}
        ${GST_VIDEO_LIBRARIES}
        ${GST_PBU_LIBRARIES}
        vwidget
        reviewplayer
)
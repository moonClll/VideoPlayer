cmake_minimum_required(VERSION 3.26)
project(liveplayer)

unset(CMAKE_AUTOMOC)
unset(CMAKE_AUTORCC)
unset(CMAKE_AUTOUIC)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GST_PBU REQUIRED gstreamer-pbutils-1.0)

include_directories(${GST_INCLUDE_DIRS})
include_directories(${GST_VIDEO_INCLUDE_DIRS})
include_directories(${GST_PBU_INCLUDE_DIRS})

add_library(liveplayer
        LivePlayer.h
        LivePlayer.cpp
)

target_link_libraries(liveplayer
        ${GST_LIBRARIES}
        ${GST_VIDEO_LIBRARIES}
        ${GST_PBU_LIBRARIES}
)
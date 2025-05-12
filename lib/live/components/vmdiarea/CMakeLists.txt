cmake_minimum_required(VERSION 3.26)
project(vmdiarea)

include_directories(${PROJECT_SOURCE_DIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_library(vmdiarea
        VMdiArea.h
        VMdiArea.cpp
        VMdiArea.ui
)

target_link_libraries(vmdiarea
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
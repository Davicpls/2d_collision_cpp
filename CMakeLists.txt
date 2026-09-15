cmake_minimum_required(VERSION 3.21)

project(2DCollision
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(2DCollision

    src/qt/MainWindow.cpp
    src/qt/MainWindow.hpp
    src/qt/MainWidget.hpp
    src/qt/MainWidget.cpp
    src/simulation/Simulation.hpp
    src/simulation/Simulation.cpp
    src/main.cpp
)

target_include_directories(2DCollision PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(2DCollision PRIVATE
    Qt6::Widgets
)

if(WIN32)
    add_custom_command(TARGET 2DCollision POST_BUILD
        COMMAND Qt6::windeployqt
            "$<TARGET_FILE:2DCollision>"
    )
endif()
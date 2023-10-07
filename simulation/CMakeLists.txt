cmake_minimum_required(VERSION 3.14)
project(bldc LANGUAGES CXX)
find_package(atta 0.4.0 REQUIRED EXACT)

# Project script
set(PROJECT_SOURCES
    src/projectScript.cpp
    src/motor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../controller/controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../controller/trapezoidalController.cpp
)
atta_add_target(project_script ${PROJECT_SOURCES})
target_include_directories(project_script PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../controller)

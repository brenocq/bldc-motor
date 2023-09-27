cmake_minimum_required(VERSION 3.14)
project(bldc LANGUAGES CXX)
find_package(atta 0.4.0 REQUIRED EXACT)

# Project script
atta_add_target(project_script "src/projectScript.cpp")

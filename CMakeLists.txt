cmake_minimum_required(VERSION 3.10)

project(az-json
    DESCRIPTION "Another JSON5 & JSON manipulator library"
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

enable_testing()

include_directories(headers)

add_subdirectory(headers)
add_subdirectory(sources)
add_subdirectory(testing)
add_subdirectory(utility)

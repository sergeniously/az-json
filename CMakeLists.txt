cmake_minimum_required(VERSION 3.10)

project(az-json
    DESCRIPTION "JSON5 & JSON manipulating library"
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

enable_testing()

include_directories(headers)

add_subdirectory(headers)
add_subdirectory(sources)
add_subdirectory(tests)

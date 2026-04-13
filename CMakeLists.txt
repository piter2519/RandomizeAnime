cmake_minimum_required(VERSION 3.10)
project(RandomizeAnime VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

find_package(CURL REQUIRED)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(dep/nlohmann_json)

add_subdirectory(src)
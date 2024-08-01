cmake_minimum_required(VERSION 3.10)

IF( WIN32 )
    SET ( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO )
ENDIF( WIN32 )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(RandomizeAnime VERSION 0.4.0)

#build type
set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(curl_DIR ${PROJECT_SOURCE_DIR}/dep/curl/)

#dołączanie biblioteki curl
set(CURL_INCLUDE_DIR "${curl_DIR}/include")
set(CURL_LIB_DIR "${curl_DIR}/lib")
#dołączanie nagłówków curl
include_directories(${CURL_INCLUDE_DIR})

#dodanie biblioteki do projektu
add_library(curl STATIC IMPORTED)
set_target_properties(curl PROPERTIES IMPORTED_LOCATION ${CURL_LIB_DIR}/libcurl.dll.a)

# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")

# If you only include this third party in PRIVATE source files, you do not
# need to install it when your main project gets installed.
# set(JSON_Install OFF CACHE INTERNAL "")

# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.
add_subdirectory(dep/nlohmann_json)
add_library(json STATIC IMPORTED)

set(CMAKE_LIBRARY_PATH ${CURL_LIB_DIR} ${CMAKE_LIBRARY_PATH})



add_subdirectory(src)
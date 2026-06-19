cmake_minimum_required (VERSION 3.10)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

set(CMAKE_CXX_STANDARD 23)

project(dl-mod)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${MY_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory("deps/minhook")
add_subdirectory("deps/discord-presence")

target_link_libraries(${PROJECT_NAME} PRIVATE minhook discord-rpc)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".asi"
)
cmake_minimum_required(VERSION 3.27)
project(clay_examples_introducing_clay_video_demo C)
set(CMAKE_C_STANDARD 99)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.5"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(clay_examples_introducing_clay_video_demo main.c)

target_compile_options(clay_examples_introducing_clay_video_demo PUBLIC)
target_include_directories(clay_examples_introducing_clay_video_demo PUBLIC .)

target_link_libraries(clay_examples_introducing_clay_video_demo PUBLIC raylib)

if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "/D CLAY_DEBUG")
else()
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

add_custom_command(
        TARGET clay_examples_introducing_clay_video_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)

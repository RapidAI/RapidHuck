cmake_minimum_required(VERSION 3.16)
project(RapidHuck VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Prefer a locally installed raylib (>= 5.0); otherwise fetch and build it.
find_package(raylib 5.0 QUIET)

if(NOT raylib_FOUND)
  message(STATUS "raylib not found locally - fetching from source")
  include(FetchContent)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(raylib)
endif()

add_executable(RapidHuck
  src/main.cpp
  src/game.cpp
)
target_include_directories(RapidHuck PRIVATE src)

if(TARGET raylib)
  target_link_libraries(RapidHuck PRIVATE raylib)
endif()

if(MSVC)
  target_compile_options(RapidHuck PRIVATE /W4 /permissive-)
else()
  target_compile_options(RapidHuck PRIVATE -Wall -Wextra)
endif()

install(TARGETS RapidHuck RUNTIME DESTINATION bin)

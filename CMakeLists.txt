cmake_minimum_required(VERSION 3.1...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ProjectMinecraft VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(cmake/StandardProjectSettings.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which
  # is a good idea if any #define's change
  # 
  # 
  target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
endif()

# Set up some extra Conan dependencies based on our needs
# before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")


if(ENABLE_TESTING)
  enable_testing()
  message(
    "Building Tests. Be sure to check out test/constexpr_tests for constexpr testing"
  )
  add_subdirectory(test)
endif()

if(ENABLE_FUZZING)
  message(
    "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html"
  )
  add_subdirectory(fuzz_test)
endif()


add_executable(Minecraft Minecraft/main.cpp)
set_target_properties(Minecraft PROPERTIES CXX_EXTENSIONS OFF)

add_subdirectory(args)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)
add_subdirectory(lodepng)
add_subdirectory(spdlog)
add_subdirectory(crossguid)
add_subdirectory(pcg-cpp)
add_subdirectory(gzstream)
add_subdirectory(concurrentqueue)
add_subdirectory(dynamic_bitset)
add_subdirectory(Minecraft/net/minecraft/nbt)
add_subdirectory(Minecraft/net/minecraft/util)
add_subdirectory(Minecraft/net/minecraft/block)
add_subdirectory(Minecraft/net/minecraft/world)
add_subdirectory(Minecraft/net/minecraft/village)
add_subdirectory(Minecraft/net/minecraft/tileentity)
add_subdirectory(Minecraft/net/minecraft/stats)
add_subdirectory(Minecraft/net/minecraft/scoreboard)
add_subdirectory(Minecraft/net/minecraft/profiler)
add_subdirectory(Minecraft/net/minecraft/potion)
add_subdirectory(Minecraft/net/minecraft/pathfinding)
add_subdirectory(Minecraft/net/minecraft/item)
add_subdirectory(Minecraft/net/minecraft/inventory)
add_subdirectory(Minecraft/net/minecraft/init)
add_subdirectory(Minecraft/net/minecraft/entity)

target_link_libraries(Minecraft PRIVATE project_options project_warnings args nlohmann_json::nlohmann_json util block world)

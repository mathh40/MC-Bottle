cmake_minimum_required(VERSION 3.1...3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ProjectMinecraft VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(Flags)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)				
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(WARNING "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE PACKAGE_GIT_VERSION
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

add_executable(Minecraft Minecraft/main.cpp)
target_compile_features(Minecraft PRIVATE cxx_std_20)
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
#add_subdirectory(Minecraft/net/minecraft/client)

FLAGS(Minecraft)

target_link_libraries(Minecraft PRIVATE args nlohmann_json::nlohmann_json util block world)

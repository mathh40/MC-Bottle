cmake_minimum_required(VERSION 3.16)


# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# Add project_options v0.20.0
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.25.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'")
	
# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

# uncomment to enable vcpkg:
# # Setup vcpkg - should be called before defining project()
#run_vcpkg()

project(
	ProjectMinecraft
	VERSION 0.0.1
	DESCRIPTION ""
	HOMEPAGE_URL "%%myurl%%"
	LANGUAGES CXX C)
	

# This variable is set by project() in CMake 3.21+
string(
  COMPARE EQUAL
          "${CMAKE_SOURCE_DIR}"
          "${PROJECT_SOURCE_DIR}"
          PROJECT_IS_TOP_LEVEL)
if(PROJECT_IS_TOP_LEVEL)
  # Consider the CTest module, which creates targets and options!
  # Only needed if you want to enable submissions to a CDash server.
  include(CTest)
endif()

set(GIT_SHA
    "Unknown"
    CACHE STRING "SHA this build was generated from")
string(
  SUBSTRING "${GIT_SHA}"
            0
            8
            GIT_SHORT_SHA)
			
get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(BUILDING_MULTI_CONFIG)
  if(NOT CMAKE_BUILD_TYPE)
    # Make sure that all supported configuration types have their
    # associated conan packages available. You can reduce this
    # list to only the configuration types you use, but only if one
    # is not forced-set on the command line for VS
    message(TRACE "Setting up multi-config build types")
    set(CMAKE_CONFIGURATION_TYPES
        Debug
        Release
        RelWithDebInfo
        MinSizeRel
        CACHE STRING "Enabled build types" FORCE)
  else()
    message(TRACE "User chose a specific build type, so we are using that")
    set(CMAKE_CONFIGURATION_TYPES
        ${CMAKE_BUILD_TYPE}
        CACHE STRING "Enabled build types" FORCE)
  endif()
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# defaulted_project_options sets recommended defaults and provides user and developer
# modes and full GUI support for choosing options at configure time

# for more flexibility, look into project_options() macro

# Any default can be overridden
# set(<feature_name>_DEFAULT <value>) - set default for both user and developer modes
# set(<feature_name>_DEVELOPER_DEFAULT <value>) - set default for developer mode
# set(<feature_name>_USER_DEFAULT <value>) - set default for user mode

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment the options to enable them:
dynamic_project_options(
  # Note: PCH is disabled by default in developer mode because these headers become
  # globally included and they can mask other errors
  PCH_HEADERS
  <vector>
  <string>
  <unordered_map>
  <utility>
  <memory># This is a list of headers to pre-compile, here are some common ones
  ENABLE_COVERAGE
  #ENABLE_CONAN
  # CONAN_OPTIONS    # Extra options to pass to conan
  # MSVC_WARNINGS    # Override the defaults for the MSVC warnings
  # CLANG_WARNINGS   # Override the defaults for the CLANG warnings
  # GCC_WARNINGS     # Override the defaults for the GCC warnings
  CPPCHECK_OPTIONS
  --enable=style,performance,warning,portability
  --inline-suppr
  # We cannot act on a bug/missing feature of cppcheck
  --suppress=cppcheckError
  --suppress=internalAstError
  # if a file does not have an internalAstError, we get an unmatchedSuppression error
  --suppress=unmatchedSuppression
  --suppress=passedByValue
  --suppress=syntaxError
  --inconclusive
)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
	if(NOT TARGET fmt)
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule init
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
	endif()
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --remote
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
# TODO: The INTERFACE library NAMESPACE ALIAS are missing! CK
add_library(ProjectMinecraft::project_options INTERFACE IMPORTED)
add_library(ProjectMinecraft::project_warnings INTERFACE IMPORTED)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

add_subdirectory(args)
add_subdirectory(nlohmann_json)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(fmt)
add_subdirectory(spdlog)
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
add_subdirectory(crossguid)
add_subdirectory(compile-time-regular-expressions)
add_subdirectory(concurrentqueue)
add_subdirectory(dynamic_bitset)
add_subdirectory(authlib)
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
add_subdirectory(Minecraft/net/minecraft/enchantment)
add_subdirectory(Minecraft/net/minecraft/dispenser)
add_subdirectory(Minecraft/net/minecraft/creativetab)
add_subdirectory(Minecraft/net/minecraft/crash)
add_subdirectory(Minecraft/net/minecraft/command)
add_subdirectory(Minecraft/net/minecraft/advancements)

#add_library(MinecraftCore STATIC)
#target_link_libraries(MinecraftCore PRIVATE project_options project_warnings)

add_executable(MinecraftServer Minecraft/main.cpp)
target_link_libraries(MinecraftServer PRIVATE project_options project_warnings MinecraftCore)
target_include_directories(MinecraftServer PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# Adding the tests:
option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})
if(ENABLE_TESTING)
  enable_testing()
  message(AUTHOR_WARNING "Building Tests. Be sure to check out test/constexpr_tests.cpp for constexpr testing")
  add_subdirectory(test)
endif()

option(ENABLE_FUZZING "Enable the fuzz tests" OFF)
if(ENABLE_FUZZING)
  message(AUTHOR_WARNING "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
  add_subdirectory(fuzz_test)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT MinecraftServer)

if(CMAKE_SKIP_INSTALL_RULES)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

# Add other targets that you want installed here, be default we just package the one executable
# we know we want to ship
package_project(TARGETS MinecraftServer project_options project_warnings
  # FIXME: this does not work! CK
  # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
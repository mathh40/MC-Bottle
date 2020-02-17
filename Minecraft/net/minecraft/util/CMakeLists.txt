file(GLOB_RECURSE source_list "*.cpp" "*.h" )

include(Flags)

add_library(util STATIC ${source_list})
target_compile_features(util PRIVATE cxx_std_20)
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
FLAGS(util)

target_link_libraries(util PRIVATE lodepng spdlog pcg-cpp nbt nlohmann_json::nlohmann_json)
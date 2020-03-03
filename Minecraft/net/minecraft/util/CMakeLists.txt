file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(util STATIC ${source_list})
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(util PRIVATE project_options project_warnings lodepng spdlog pcg-cpp nbt nlohmann_json::nlohmann_json)
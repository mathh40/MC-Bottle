file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(world STATIC ${source_list})
target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(world PRIVATE project_options project_warnings nbt util block concurrentqueue nlohmann_json::nlohmann_json spdlog)
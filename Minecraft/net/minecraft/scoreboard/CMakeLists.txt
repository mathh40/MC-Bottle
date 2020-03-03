file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(scoreboard STATIC ${source_list})
target_include_directories(scoreboard PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scoreboard PRIVATE project_options project_warnings nbt util spdlog)
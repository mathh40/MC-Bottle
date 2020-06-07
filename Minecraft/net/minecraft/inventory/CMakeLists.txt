file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(inventory STATIC ${source_list})
target_include_directories(inventory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inventory PRIVATE project_options project_warnings nbt util spdlog::spdlog crossguid)
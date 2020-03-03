file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(tileentity STATIC ${source_list})
target_include_directories(tileentity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tileentity PRIVATE project_options project_warnings nbt util block spdlog)
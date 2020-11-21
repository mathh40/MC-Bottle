file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(dispenser STATIC ${source_list})
target_include_directories(dispenser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dispenser PRIVATE project_options project_warnings nbt util spdlog::spdlog crossguid)
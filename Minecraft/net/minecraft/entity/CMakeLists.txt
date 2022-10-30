file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(entity STATIC ${source_list})
target_include_directories(entity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(entity PRIVATE project_options project_warnings nbt util spdlog::spdlog)
install(TARGETS entity
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

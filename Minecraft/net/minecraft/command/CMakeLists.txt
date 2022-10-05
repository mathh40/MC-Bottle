file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(command STATIC ${source_list})
target_include_directories(command PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(command PRIVATE project_options project_warnings)
install(TARGETS command
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

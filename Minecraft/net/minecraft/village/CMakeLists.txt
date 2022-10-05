file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(village STATIC ${source_list})
target_include_directories(village PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(village PRIVATE project_options project_warnings)
install(TARGETS village
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

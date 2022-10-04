file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(stats STATIC ${source_list})
target_include_directories(stats PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(stats PRIVATE project_options project_warnings)
install(TARGETS stats
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
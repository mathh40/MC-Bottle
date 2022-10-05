file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(crash STATIC ${source_list})
target_include_directories(crash PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(crash PRIVATE project_options project_warnings)
install(TARGETS crash
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

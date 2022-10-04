file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(status STATIC ${source_list})
target_include_directories(status PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(status PRIVATE project_options project_warnings)
install(TARGETS status
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
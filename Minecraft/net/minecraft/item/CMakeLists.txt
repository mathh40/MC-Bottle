file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(item STATIC ${source_list})
target_include_directories(item PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(item PRIVATE project_options project_warnings)
install(TARGETS item
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

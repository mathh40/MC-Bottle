file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(block STATIC ${source_list})
target_include_directories(block PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(block PRIVATE project_options project_warnings)
install(TARGETS block
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(init STATIC ${source_list})
target_include_directories(init PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(init PRIVATE project_options project_warnings)
install(TARGETS init
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(potion STATIC ${source_list})
target_include_directories(potion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(potion PRIVATE project_options project_warnings)
install(TARGETS potion
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

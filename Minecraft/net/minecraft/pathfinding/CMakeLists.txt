file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(pathfinding STATIC ${source_list})
target_include_directories(pathfinding PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pathfinding PRIVATE project_options project_warnings)
install(TARGETS pathfinding
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

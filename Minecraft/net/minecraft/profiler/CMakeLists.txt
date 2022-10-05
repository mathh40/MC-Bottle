file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(profiler STATIC ${source_list})
target_include_directories(profiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(profiler PRIVATE project_options project_warnings)
install(TARGETS profiler
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

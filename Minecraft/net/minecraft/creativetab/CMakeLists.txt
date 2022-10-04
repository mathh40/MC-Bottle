file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(creativetab STATIC ${source_list})
target_include_directories(creativetab PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(creativetab PRIVATE project_options project_warnings)
install(TARGETS creativetab
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
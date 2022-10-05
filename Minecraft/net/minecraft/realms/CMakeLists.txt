file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(realms STATIC ${source_list})
target_include_directories(realms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(realms PRIVATE project_options project_warnings)
install(TARGETS realms
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(nbt STATIC ${source_list})
target_include_directories(nbt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(nbt PRIVATE project_options project_warnings)

install(TARGETS nbt
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

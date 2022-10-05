file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(advancements STATIC ${source_list})
target_include_directories(advancements PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(advancements PRIVATE project_options project_warnings nbt util)
install(TARGETS advancements
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

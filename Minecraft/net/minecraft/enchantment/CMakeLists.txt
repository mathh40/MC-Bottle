file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(enchantment STATIC ${source_list})
target_include_directories(enchantment PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(enchantment PRIVATE project_options project_warnings)
install(TARGETS enchantment
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

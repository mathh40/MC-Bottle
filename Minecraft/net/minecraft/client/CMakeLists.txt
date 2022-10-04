file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(MinecraftClient SHARED ${source_list})
target_include_directories(MinecraftClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MinecraftClient PRIVATE project_options project_warnings)
install(TARGETS MinecraftClient
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
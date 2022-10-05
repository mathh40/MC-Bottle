file(GLOB_RECURSE source_list "*.cpp" "*.h" )
list(FILTER source_list EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/imgaeloader/lodepng/examples/.*" )
list(FILTER source_list EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/crypto/.*" )

add_subdirectory(crypto/cryptopp-cmake)
add_library(util STATIC ${source_list})
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(util PRIVATE project_options project_warnings)
install(TARGETS util
        EXPORT mincraftTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

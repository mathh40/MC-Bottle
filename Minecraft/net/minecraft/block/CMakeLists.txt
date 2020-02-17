file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(block STATIC ${source_list})
target_compile_features(block PRIVATE cxx_std_20)
target_include_directories(block PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(block PRIVATE nbt)
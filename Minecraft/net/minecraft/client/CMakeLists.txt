file(GLOB_RECURSE source_list "*.cpp" "*.h" )

add_library(MinecraftClient SHARED ${source_list})
target_compile_features(MinecraftClient PRIVATE cxx_std_20)
target_include_directories(MinecraftClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MinecraftClient PRIVATE util spdlog)
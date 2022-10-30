file(GLOB_RECURSE source_list "*.cpp" "*.h" )
add_library(authlib STATIC ${source_list})
target_include_directories(authlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(authlib PRIVATE project_options project_warnings nlohmann_json::nlohmann_json spdlog::spdlog fmt::fmt util)

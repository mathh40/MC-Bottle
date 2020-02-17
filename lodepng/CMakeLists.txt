add_library(lodepng STATIC lodepng.h lodepng.cpp)
target_compile_features(lodepng PRIVATE cxx_std_20)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

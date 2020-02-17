add_library(pcg-cpp INTERFACE)
target_compile_features(pcg-cpp INTERFACE cxx_std_17)
target_sources(pcg-cpp INTERFACE pcg_random.hpp pcg_uint128.hpp pcg_extras.hpp)

target_include_directories(pcg-cpp BEFORE
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
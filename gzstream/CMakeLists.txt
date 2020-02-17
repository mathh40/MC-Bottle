add_library(gzstream INTERFACE)
target_compile_features(gzstream INTERFACE cxx_std_17)
target_sources(gzstream INTERFACE gzstream.h)

target_include_directories(gzstream BEFORE
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
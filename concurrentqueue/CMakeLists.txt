add_library(concurrentqueue INTERFACE)
target_compile_features(concurrentqueue INTERFACE cxx_std_17)
target_sources(concurrentqueue INTERFACE concurrentqueue.h blockingconcurrentqueue.h)

target_include_directories(concurrentqueue BEFORE
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

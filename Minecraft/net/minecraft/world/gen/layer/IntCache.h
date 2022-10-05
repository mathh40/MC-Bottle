#pragma once
#include <array>
#include <cstdint>
#include <mutex>
#include <vector>

class IntCache
{
  public:
    static std::vector<int32_t> getIntCache(int32_t size);
    static void resetIntCache();

  private:
    static int32_t intCacheSize;
    static std::vector<std::array<int32_t, 256>> freeSmallArrays;
    static std::vector<std::array<int32_t, 256>> inUseSmallArrays;
    static std::vector<std::vector<int32_t>> freeLargeArrays;
    static std::vector<std::vector<int32_t>> inUseLargeArrays;

    mutable std::mutex mux;
};

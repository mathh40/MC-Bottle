#pragma once
#include <memory>

class NBTSizeTracker
{
  public:
    static const std::unique_ptr<NBTSizeTracker> Infinite;

    NBTSizeTracker(uint64_t max);

    void read(uint64_t bits);

  private:
    uint64_t max   = 0;
    uint64_t readi = 0;
};

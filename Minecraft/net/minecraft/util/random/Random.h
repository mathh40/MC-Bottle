#pragma once
#include "../ByteBuffer.h"

class Random {
  public:
    Random() = default;
    ~Random() = default;
    virtual void setSeed(uint64_t seed, uint64_t state) = 0;
    virtual void nextBytes(ByteBuffer& bytes) = 0;
    virtual uint32_t nextInt() = 0;
    virtual uint32_t nextInt(uint32_t bound) = 0;
    virtual uint64_t nextLong() = 0;
    virtual bool nextBoolean() = 0;
    virtual float nextFloat() = 0;
    virtual double nextDouble() = 0;
    virtual double nextGaussian() = 0;
};

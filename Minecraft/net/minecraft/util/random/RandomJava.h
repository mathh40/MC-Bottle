#pragma once
#include "Random.h"

class RandomJava : public Random {
  public:
    RandomJava();
    RandomJava(uint64_t seed = 0);

    void setSeed(uint64_t seed, uint64_t state) override;
    void nextBytes(ByteBuffer &bytes) override;
    uint32_t nextInt() override;
    uint32_t nextInt(uint32_t bound) override;
    uint64_t nextLong() override;
    bool nextBoolean() override;
    float nextFloat() override;
    double nextDouble() override;
    double nextGaussian() override;

  protected:
    uint32_t next(uint32_t bits);
  private:
    static uint64_t seedUniquifier();
    static uint64_t initialScramble(uint64_t seed);

    uint64_t seed = 0;
    static uint64_t seedUniquifierVal;
    bool haveNextGaussian = false;
    double nextNextGaussian = 0;
};

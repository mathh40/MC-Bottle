#include "RandomJava.h"
#include "../Util.h"
#include "../Math/MathHelper.h"

constexpr uint64_t multiplier = 0x5DEECE66DL;
constexpr uint64_t addend = 0xBL;
constexpr uint64_t mask = (1L << 48) - 1;
constexpr double DOUBLE_UNIT = 0x1.0p-53;
constexpr float FLOAT_UNIT = 0x1.0p-24f;

RandomJava::RandomJava() : RandomJava(seedUniquifier() ^ Util::currentTimeMillis()) {}

RandomJava::RandomJava(uint64_t seed) : Random() {
    seed = initialScramble(seed); 
}

uint32_t RandomJava::next(uint32_t bits) {
    uint32_t oldseed = 0, nextseed = 0;
    for (;;) {
        oldseed = seed;
        nextseed = (oldseed * multiplier + addend) & mask;
        if (oldseed == seed) {
            seed = nextseed;
            break;
        }
    }
    return (uint32_t)(nextseed >>> (48 - bits));
}

uint64_t RandomJava::seedUniquifier() { 
	for (;;) {
        uint64_t current = seedUniquifierVal;
        uint64_t next = current * 1181783497276652981L;
        if (seedUniquifierVal == current) {
            seedUniquifierVal = next;
            return next;
        }           
    }
}

uint64_t RandomJava::initialScramble(uint64_t seed) 
{ 
    return (seed ^ multiplier) & mask; 
}

void RandomJava::setSeed(uint64_t seed) {
    seed = initialScramble(seed);
    haveNextGaussian = false;
}

void RandomJava::nextBytes(ByteBuffer &bytes) {
    for (size_t i = 0, len = bytes.size(); i < len;)
        for (uint32_t rnd = nextInt(), n = MathHelper::min(len - i, sizeof(uint32_t) / sizeof(std::byte); n-- > 0; rnd >>= sizeof(std::byte)))
            bytes[i++] = (std::byte)rnd;
}

uint32_t RandomJava::nextInt() { 
    return next(32); }

uint32_t RandomJava::nextInt(uint32_t bound) {
    auto r = next(31);
    auto m = bound - 1;
    if ((bound & m) == 0) // i.e., bound is a power of 2
        r = (uint32_t)((bound * (uint64_t)r) >> 31);
    else { // reject over-represented candidates
        for (uint32_t u = r; u - (r = u % bound) + m < 0; u = next(31));
    }
    return r;
}

uint64_t RandomJava::nextLong() { 
    return ((uint64_t)(next(32)) << 32) + next(32); 
}

bool RandomJava::nextBoolean() { 
    return next(1) != 0; }

float RandomJava::nextFloat() {
    return next(sizeof(float) * 8) * FLOAT_UNIT; 
}

double RandomJava::nextDouble() { 
    return (((uint64_t)(next(sizeof(double) * 8 - 27)) << 27) + next(27)) * DOUBLE_UNIT; 
}

double RandomJava::nextGaussian() { 
    if (haveNextGaussian) {
        haveNextGaussian = false;
        return nextNextGaussian;
    } else {
        double v1, v2, s;
        do {
            v1 = 2 * nextDouble() - 1; // between -1 and 1
            v2 = 2 * nextDouble() - 1; // between -1 and 1
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);
        double multiplier = MathHelper::sqrt(-2 * MathHelper::log(s) / s);
        nextNextGaussian = v2 * multiplier;
        haveNextGaussian = true;
        return v1 * multiplier;
    }
}



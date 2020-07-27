#pragma once
#include "../../../../../crossguid/include/crossguid/guid.hpp"
#include <cstdint>
#include <cstring>
#include <vector>

#include "pcg_random.hpp"

class Vec3i;

namespace MathHelper
{
	constexpr  float SQRT_2 = 1.414213f;
	constexpr int32_t MULTIPLY_DE_BRUIJN_BIT_POSITION[] = { 0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9 };

	constexpr double longBitsToDouble(int64_t bits)
	{
		double result = 0.0;
		memcpy(&result, &bits, sizeof(bits));
		return result;
	}

	constexpr int64_t doubleToRawLongBits(double bits)
	{
		int64_t result = 0;
		memcpy(&result, &bits, sizeof(bits));
		return result;
	}

	float sin(float value);
	float cos(float value);

	template<class T>
	T sqrt(T value)
	{
		return sqrt(value);
	}

	int32_t floor(float value);
	int32_t fastFloor(double value);
	int32_t floor(double value);
	int64_t lfloor(double value);
	int32_t absFloor(double value);

	template<class T>
	T abs(T value)
	{
		return value >= static_cast<T>(0) ? value : -value;
	};

	template<class T>
	T round(T value)
	{
		return floor(value + static_cast<T>(0.5));
	};
	
	int32_t ceil(float value);
	int32_t ceil(double value);

	template<class T>
	constexpr T clamp(T num, T min, T max)
	{
		if (num < min) {
			return min;
		}
		else {
			return num > max ? max : num;
		}
	}

	int32_t random();

	double clampedLerp(double lowerBnd, double upperBnd, double slide);
	double absMax(double p_76132_0_, double p_76132_2_);
	int32_t intFloorDiv(int32_t p_76137_0_, int32_t p_76137_1_);

	template<typename T>
	T getInt(pcg32& random, T minimum, T maximum)
	{
		static std::uniform_int_distribution<T> uniform_dist(0, (maximum - minimum + 1));
		return minimum >= maximum ? minimum : uniform_dist(random) + minimum;
	}

	int64_t nextLong(pcg32& random, int64_t minimum = 0, int64_t maximum = std::numeric_limits<int64_t>::max());
	float nextFloat(pcg32 & random, float minimum = 0.0f, float maximum = 1.0f);
	double nextDouble(pcg32 & random, double minimum = 0.0, double maximum = 1.0);
    float nextFloat(pcg32& random, float minimum, float maximum);
	double nextDouble(pcg32& random, double minimum, double maximum);
	template<typename T>
    T nextGaussian(pcg32& random)
	{
	    static std::normal_distribution<T> distribution(0.0,1.0);
		return distribution(random);
	}


	double average(const std::vector<long> &values);
	bool epsilonEquals(float p_180185_0_, float p_180185_1_);
	int64_t normalizeAngle(int64_t p_180184_0_, int64_t p_180184_1_);
	float positiveModulo(float numerator, float denominator);
	double positiveModulo(double numerator, double denominator);
	float wrapDegrees(float value);
	double wrapDegrees(double value);
	int32_t wrapDegrees(int32_t angle);

	int32_t getInt(std::string value, int32_t defaultValue);
	int32_t getInt(std::string value, int32_t defaultValue, int32_t max);
	double getDouble(std::string value, double defaultValue);
	double getDouble(std::string value, double defaultValue, double max);

	constexpr int64_t smallestEncompassingPowerOfTwo(int64_t value)
	{
		int64_t i = value - 1;
		i |= i >> 1;
		i |= i >> 2;
		i |= i >> 4;
		i |= i >> 8;
		i |= i >> 16;
		return i + 1;
	}

	constexpr bool isPowerOfTwo(int64_t value)
	{
		return value != 0 && (value & value - 1) == 0;
	}

	constexpr int64_t log2DeBruijn(int64_t value)
	{
		value = isPowerOfTwo(value) ? value : smallestEncompassingPowerOfTwo(value);
		return MULTIPLY_DE_BRUIJN_BIT_POSITION[(int)((long)value * 125613361L >> 27) & 31];
	}

	constexpr int64_t log2(int64_t value)
	{
		return log2DeBruijn(value) - (isPowerOfTwo(value) ? 0 : 1);
	}
	
	int32_t roundUp(int32_t number, int32_t interval);
	int32_t rgb(float rIn, float gIn, float bIn);
	int32_t rgb(int32_t rIn, int32_t gIn, int32_t bIn);
	int32_t multiplyColor(int32_t p_180188_0_, int32_t p_180188_1_);

	double frac(double number);
	int64_t getPositionRandom(Vec3i pos);
	int64_t getCoordinateRandom(int32_t x, int32_t y, int32_t z);

	xg::Guid getRandomUUID(pcg32 &random);
	double pct(double p_181160_0_, double p_181160_2_, double p_181160_4_);

	double atan2(double p_181159_0_, double p_181159_2_);
	double fastInvSqrt(double p_181161_0_);

	int32_t hsvToRGB(float hue, float saturation, float value);

	template<class T>
	constexpr T max(T a, T b)
	{
		return (a < b) ? b : a;
	}

	template<class T>
	constexpr T min(T a, T b)
	{
		return (a > b) ? b : a;
	}

	template<class T>
	T pow(T x, T x1)
	{
		return std::pow(x, x1);
	}
	
	template<class T>
	T fmod(T x, T x1)
	{
		return std::fmod(x, x1);
	}
}

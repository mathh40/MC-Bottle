#include "MathHelper.h"
#include <array>
#include <random>
#include "Vec3i.h"

namespace MathHelper
{
	constexpr std::array<float, 65536> generatesin()
	{
		std::array<float, 65536> SIN_TABLE{};
		int j;
		for (j = 0; j < 65536; ++j) {
			SIN_TABLE[j] = (float)sin((double)j * 3.141592653589793 * 2.0 / 65536.0);
		}
		return SIN_TABLE;
	}

	constexpr std::array<double, 257> generateasin()
	{
		std::array<double, 257> ASINE_TAB{};
		int j;
		for (j = 0; j < 257; ++j) {
			double d0 = (double)j / 256.0;
			double d1 = asin(d0);
			ASINE_TAB[j] = d1;
		}
		return ASINE_TAB;
	}

	constexpr std::array<double, 257> generatecos()
	{
		std::array<double, 257> COS_TAB{};
		int j;
		for (j = 0; j < 257; ++j) {
			double d0 = (double)j / 256.0;
			double d1 = asin(d0);
			COS_TAB[j] = cos(d1);
		}
		return COS_TAB;
	}


	std::array<float, 65536> SIN_TABLE = generatesin();
	
	double FRAC_BIAS = longBitsToDouble(4805340802404319232L);
	std::array<double, 257> ASINE_TAB = generateasin();
	std::array<double, 257> COS_TAB = generatecos();

	float sin(float value)
	{
		return SIN_TABLE[(int32_t)(value * 10430.378F) & 0xffff];
	}

	float cos(float value)
	{
		return SIN_TABLE[(int32_t)(value * 10430.378F + 16384.0F) & 0xffff];
	}

	int32_t floor(float value)
	{
		int32_t i = (int32_t)value;
		return value < (int32_t)i ? i - 1 : i;
	}

	int32_t fastFloor(double value)
	{
		return (int32_t)(value + 1024.0) - 1024;
	}

	int32_t floor(double value)
	{
		int32_t i = (int32_t)value;
		return value < (double)i ? i - 1 : i;
	}

	int64_t lfloor(double value)
	{
		int64_t i = (int64_t)value;
		return value < (double)i ? i - 1L : i;
	}

	int32_t absFloor(double value)
	{
		return (int32_t)(value >= 0.0 ? value : -value + 1.0);
	}

	int32_t ceil(float value)
	{
		int32_t i = (int32_t)value;
		return value > (float)i ? i + 1 : i;
	}

	int32_t ceil(double value)
	{
		int32_t i = (int32_t)value;
		return value > (double)i ? i + 1 : i;
	}

    int32_t random()
    {
		static pcg32 rand;
		return rand();
    }

	double clampedLerp(double lowerBnd, double upperBnd, double slide)
	{
		if (slide < 0.0) {
			return lowerBnd;
		}
		else {
			return slide > 1.0 ? upperBnd : lowerBnd + (upperBnd - lowerBnd) * slide;
		}
	}

	double absMax(double p_76132_0_, double p_76132_2_)
	{
		if (p_76132_0_ < 0.0) {
			p_76132_0_ = -p_76132_0_;
		}

		if (p_76132_2_ < 0.0) {
			p_76132_2_ = -p_76132_2_;
		}

		return p_76132_0_ > p_76132_2_ ? p_76132_0_ : p_76132_2_;
	}

	int32_t intFloorDiv(int32_t p_76137_0_, int32_t p_76137_1_)
	{
		return p_76137_0_ < 0 ? -((-p_76137_0_ - 1) / p_76137_1_) - 1 : p_76137_0_ / p_76137_1_;
	}

	int64_t nextLong(pcg32& random, int64_t minimum, int64_t maximum)
	{

	}

	float nextFloat(pcg32& random, float minimum, float maximum)
	{
		static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
		return minimum >= maximum ? minimum : uniform_dist(random) * (maximum - minimum) + minimum;
	}

	double nextDouble(pcg32& random, double minimum, double maximum)
	{
		static std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
		return minimum >= maximum ? minimum : uniform_dist(random) * (maximum - minimum) + minimum;
	}

    double average(const std::vector<long>& values)
	{
		long i = 0L;

		for (auto j : values)
		{
			i += j;
		}

		return (double)i / (double)values.size();
	}

	bool epsilonEquals(float p_180185_0_, float p_180185_1_)
	{
		return abs(p_180185_1_ - p_180185_0_) < 1.0E-5f;
	}

	int64_t normalizeAngle(int64_t p_180184_0_, int64_t p_180184_1_)
	{
		return (p_180184_0_ % p_180184_1_ + p_180184_1_) % p_180184_1_;
	}

	float positiveModulo(float numerator, float denominator)
	{
		return fmod(fmod(numerator , denominator) + denominator, denominator);
	}

	double positiveModulo(double numerator, double denominator)
	{
		return fmod(fmod(numerator, denominator) + denominator, denominator);
	}

	float wrapDegrees(float value)
	{
		value = fmod(value, 360.0F);
		if (value >= 180.0F) {
			value -= 360.0F;
		}

		if (value < -180.0F) {
			value += 360.0F;
		}

		return value;
	}

	double wrapDegrees(double value)
	{
		value = fmod(value, 360.0);
		if (value >= 180.0F) {
			value -= 360.0F;
		}

		if (value < -180.0F) {
			value += 360.0F;
		}

		return value;
	}

	int32_t wrapDegrees(int32_t angle)
	{
		angle %= 360;
		if (angle >= 180) {
			angle -= 360;
		}

		if (angle < -180) {
			angle += 360;
		}

		return angle;
	}

	int32_t getInt(std::string value, int32_t defaultValue)
	{
		int32_t num = 0;
		try
		{
			num = std::stoi(value);
		}
		catch (std::invalid_argument e)
		{
			num = defaultValue;
		}

		return num;
		
	}

	int32_t getInt(std::string value, int32_t defaultValue, int32_t max)
	{
		return std::max(max, getInt(value, defaultValue));
	}

	double getDouble(std::string value, double defaultValue)
	{
		double num = 0.0;
		try
		{
			num = std::stod(value);
		}
		catch (std::invalid_argument e)
		{
			num = defaultValue;
		}

		return num;

	}

	double getDouble(std::string value, double defaultValue, double max)
	{
		return std::max(max, getDouble(value, defaultValue));
	}

	int32_t roundUp(int32_t number, int32_t interval)
	{
		if (interval == 0) {
			return 0;
		}
		else if (number == 0) {
			return interval;
		}
		else {
			if (number < 0) {
				interval *= -1;
			}

			int32_t i = number % interval;
			return i == 0 ? number : number + interval - i;
		}
	}

	int32_t rgb(float rIn, float gIn, float bIn)
	{
		return rgb(floor(rIn * 255.0F), floor(gIn * 255.0F), floor(bIn * 255.0F));
	}

	int32_t rgb(int32_t rIn, int32_t gIn, int32_t bIn)
	{
		int32_t lvt_3_1_ = (rIn << 8) + gIn;
		lvt_3_1_ = (lvt_3_1_ << 8) + bIn;
		return lvt_3_1_;
	}

	int32_t multiplyColor(int32_t p_180188_0_, int32_t p_180188_1_)
	{
		int32_t i = (p_180188_0_ & 16711680) >> 16;
		int32_t j = (p_180188_1_ & 16711680) >> 16;
		int32_t k = (p_180188_0_ & 0xff00) >> 8;
		int32_t l = (p_180188_1_ & 0xff00) >> 8;
		int32_t i1 = (p_180188_0_ & 255) >> 0;
		int32_t j1 = (p_180188_1_ & 255) >> 0;
		int32_t k1 = (int32_t)((float)i * (float)j / 255.0F);
		int32_t l1 = (int32_t)((float)k * (float)l / 255.0F);
		int32_t i2 = (int32_t)((float)i1 * (float)j1 / 255.0F);
		return (p_180188_0_ & -16777216) | k1 << 16 | l1 << 8 | i2;
	}

	double frac(double number)
	{
		return number - floor(number);
	}

	int64_t getCoordinateRandom(int32_t x, int32_t y, int32_t z)
	{
		auto i = (int64_t)(x * 3129871) ^ (int64_t)z * 116129781L ^ (int64_t)y;
		i = i * i * 42317861L + i * 11L;
		return i;
	}

	xg::Guid getRandomUUID(pcg32& random)
	{
		std::uniform_int_distribution<int64_t> uniform_dist(0, std::numeric_limits<int64_t>::max());
		long i = (uniform_dist(random) & -61441L) | 16384L;
		long j = (uniform_dist(random) & 4611686018427387903L) | std::numeric_limits<int64_t>::min();
		std::array<uint8_t, 16> bytes{};
		memcpy(&bytes[0], &i, sizeof(i));
		memcpy(&bytes[4], &j, sizeof(j));
		return xg::Guid(bytes);
	}

	double pct(double p_181160_0_, double p_181160_2_, double p_181160_4_)
	{
		return (p_181160_0_ - p_181160_2_) / (p_181160_4_ - p_181160_2_);
	}

	double atan2(double p_181159_0_, double p_181159_2_)
	{
		double d0 = p_181159_2_ * p_181159_2_ + p_181159_0_ * p_181159_0_;
		if (std::isnan(d0)) {
			return NAN;
		}
		else {
			bool flag = p_181159_0_ < 0.0;
			if (flag) {
				p_181159_0_ = -p_181159_0_;
			}

			bool flag1 = p_181159_2_ < 0.0;
			if (flag1) {
				p_181159_2_ = -p_181159_2_;
			}

			bool flag2 = p_181159_0_ > p_181159_2_;
			double d9;
			if (flag2) {
				d9 = p_181159_2_;
				p_181159_2_ = p_181159_0_;
				p_181159_0_ = d9;
			}

			d9 = fastInvSqrt(d0);
			p_181159_2_ *= d9;
			p_181159_0_ *= d9;
			double d2 = FRAC_BIAS + p_181159_0_;
			int i = (int)doubleToRawLongBits(d2);
			double d3 = ASINE_TAB[i];
			double d4 = COS_TAB[i];
			double d5 = d2 - FRAC_BIAS;
			double d6 = p_181159_0_ * d4 - p_181159_2_ * d5;
			double d7 = (6.0 + d6 * d6) * d6 * 0.16666666666666666;
			double d8 = d3 + d7;
			if (flag2) {
				d8 = 1.5707963267948966 - d8;
			}

			if (flag1) {
				d8 = 3.141592653589793 - d8;
			}

			if (flag) {
				d8 = -d8;
			}

			return d8;
		}
	}

	double fastInvSqrt(double p_181161_0_)
	{
		double d0 = 0.5 * p_181161_0_;
		long i = doubleToRawLongBits(p_181161_0_);
		i = 6910469410427058090L - (i >> 1);
		p_181161_0_ = longBitsToDouble(i);
		p_181161_0_ *= 1.5 - d0 * p_181161_0_ * p_181161_0_;
		return p_181161_0_;
	}

	int32_t hsvToRGB(float hue, float saturation, float value)
	{
		int32_t i = (int32_t)(hue * 6.0F) % 6;
		float f = hue * 6.0F - (float)i;
		float f1 = value * (1.0F - saturation);
		float f2 = value * (1.0F - f * saturation);
		float f3 = value * (1.0F - (1.0F - f) * saturation);
		float f4;
		float f5;
		float f6;
		switch (i) {
		case 0:
			f4 = value;
			f5 = f3;
			f6 = f1;
			break;
		case 1:
			f4 = f2;
			f5 = value;
			f6 = f1;
			break;
		case 2:
			f4 = f1;
			f5 = value;
			f6 = f3;
			break;
		case 3:
			f4 = f1;
			f5 = f2;
			f6 = value;
			break;
		case 4:
			f4 = f3;
			f5 = f1;
			f6 = value;
			break;
		case 5:
			f4 = value;
			f5 = f1;
			f6 = f2;
			break;
		default:
			throw std::runtime_error("Something went wrong when converting from HSV to RGB. Input was " + std::to_string(hue) + ", " + std::to_string(saturation) + ", " + std::to_string(value));
		}

		int j = clamp((int)(f4 * 255.0F), 0, 255);
		int k = clamp((int)(f5 * 255.0F), 0, 255);
		int l = clamp((int)(f6 * 255.0F), 0, 255);
		return j << 16 | k << 8 | l;
	}

	int64_t getPositionRandom(Vec3i pos)
	{
		return getCoordinateRandom(pos.getx(), pos.gety(), pos.getz());
	}
}

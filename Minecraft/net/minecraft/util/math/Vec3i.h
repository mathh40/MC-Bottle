#pragma once
#include <string>

class Vec3i
{
public:
	static Vec3i NULL_VECTOR;

	Vec3i(int32_t x, int32_t y, int32_t z);
	Vec3i(double x, double y, double z);
	Vec3i(const Vec3i& lhs) = default;
	
	Vec3i crossProduct(Vec3i vec) const;
	int32_t getx()const;
	int32_t gety()const;
	int32_t getz()const;


	double getDistance(int xIn, int yIn, int zIn) const;
	double distanceSq(double toX, double toY, double toZ) const;
	double distanceSqToCenter(double xIn, double yIn, double zIn) const;
	double distanceSq(const Vec3i& to) const;

protected:
	int32_t x;
	int32_t y;
	int32_t z;
};

bool operator==(const Vec3i& a, const Vec3i& b);
std::string to_string(const Vec3i& a);

namespace std {
	template <> struct hash<Vec3i>
	{
		size_t operator()(const Vec3i & vec) const noexcept
		{
			return (vec.gety() + vec.getz() * 31) * 31 + vec.getx();
		}
	};
}
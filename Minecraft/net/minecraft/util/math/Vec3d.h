#pragma once
#include <optional>
#include <string>

class Vec2f;
class Vec3i;

class Vec3d
{
public:
	static Vec3d ZERO;
	Vec3d();
	Vec3d(double xIn, double yIn, double zIn);
	explicit Vec3d(const Vec3i &vector);
	Vec3d subtractReverse(Vec3d vec) const;
	Vec3d normalize();
	double dotProduct(Vec3d vec) const;
	Vec3d crossProduct(Vec3d vec) const;
	Vec3d subtract(Vec3d vec) const;
	Vec3d subtract(double x, double y, double z) const;
	Vec3d add(Vec3d vec) const;
	Vec3d add(double x, double y, double z) const;
	double distanceTo(Vec3d vec)const;
	double squareDistanceTo(Vec3d vec)const;
	double squareDistanceTo(double xIn, double yIn, double zIn)const;
	Vec3d scale(double factor);
	double length()const;
	double lengthSquared()const;
	std::optional<Vec3d> getIntermediateWithXValue(Vec3d vec, double x) const;
	std::optional<Vec3d> getIntermediateWithYValue(Vec3d vec, double y) const;
	std::optional<Vec3d> getIntermediateWithZValue(Vec3d vec, double z) const;

	Vec3d rotatePitch(float pitch);
	Vec3d rotateYaw(float yaw);
	

	double getx()const;
	double gety()const;
	double getz()const;
private:
	double x;
	double y;
	double z;
};

bool operator==(const Vec3d& a, const Vec3d& b);
std::string to_string(const Vec3d& a);
Vec3d Vec3dfromPitchYaw(Vec2f PitchYaw);
Vec3d Vec3dfromPitchYaw(float Pitch, float Yaw);

namespace std {
	template <> struct hash<Vec3d>
	{
		size_t operator()(const Vec3d & vec) const noexcept
		{
			long j = static_cast<long>(vec.getx());
			int i = (int)(j ^ j >> 32);
			j = static_cast<long>(vec.gety());
			i = 31 * i + (int)(j ^ j >> 32);
			j = static_cast<long>(vec.getz());
			i = 31 * i + (int)(j ^ j >> 32);
			return i;
		}
	};
}
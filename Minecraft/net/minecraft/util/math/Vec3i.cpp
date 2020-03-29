#include "Vec3i.h"
#include "MathHelper.h"

Vec3i Vec3i::NULL_VECTOR = Vec3i(0, 0, 0);

Vec3i::Vec3i(int32_t x, int32_t y, int32_t z)
	: x(x),
	y(y),
	z(z)
{
}

Vec3i::Vec3i(double x, double y, double z)
	: x(MathHelper::floor(x)),
	y(MathHelper::floor(y)),
	z(MathHelper::floor(z))
{

}

Vec3i Vec3i::crossProduct(Vec3i vec) const
{
	return Vec3i(y * vec.getz() - z * vec.gety(), z * vec.getx() - x * vec.getz(), x * vec.gety() - y * vec.getx());
}

int32_t Vec3i::getx() const
{
	return  x;
}

int32_t Vec3i::gety() const
{
	return  y;
}

int32_t Vec3i::getz() const
{
	return  z;
}

double Vec3i::getDistance(int xIn, int yIn, int zIn) const
{
	return MathHelper::sqrt(distanceSq(xIn, yIn, zIn));
}

double Vec3i::distanceSq(double toX, double toY, double toZ) const
{
	double d0 = (double)x - toX;
	double d1 = (double)y - toY;
	double d2 = (double)z - toZ;
	return d0 * d0 + d1 * d1 + d2 * d2;
}

double Vec3i::distanceSqToCenter(double xIn, double yIn, double zIn) const
{
	double d0 = (double)x + 0.5 - xIn;
	double d1 = (double)y + 0.5 - yIn;
	double d2 = (double)z + 0.5 - zIn;
	return d0 * d0 + d1 * d1 + d2 * d2;
}

double Vec3i::distanceSq(const Vec3i& to) const
{
	return distanceSq((double)to.getx(), (double)to.gety(), (double)to.getz());
}

bool operator==(const Vec3i& a, const Vec3i& b)
{
	return a.getx() == b.getx() && a.gety() == b.gety() && a.getz() == b.getz();
}

std::string to_string(const Vec3i& a)
{
	return "(" + std::to_string(a.getx()) + ", " + std::to_string(a.gety()) + ", " + std::to_string(a.getz()) + ")";
}

#include "Vec3d.h"
#include "Vec2f.h"
#include "MathHelper.h"
#include "Vec3i.h"

Vec3d Vec3d::ZERO = Vec3d(0.0, 0.0, 0.0);

Vec3d::Vec3d()
	:Vec3d(0.0, 0.0, 0.0)
{
}

Vec3d::Vec3d(double xIn, double yIn, double zIn)
{
	if (xIn == -0.0) {
		xIn = 0.0;
	}

	if (yIn == -0.0) {
		yIn = 0.0;
	}

	if (zIn == -0.0) {
		zIn = 0.0;
	}

	x = xIn;
	y = yIn;
	z = zIn;
}

Vec3d::Vec3d(const Vec3i &vector)
	:Vec3d(vector.getx(), vector.gety(), vector.getz())
{
	
}

Vec3d Vec3d::subtractReverse(Vec3d vec) const
{
	return Vec3d(vec.x - x, vec.y - y, vec.z - z);
}

Vec3d Vec3d::normalize() const {
	double d0 = MathHelper::sqrt(x * x + y * y + z * z);
	return d0 < 1.0E-4 ? ZERO : Vec3d(x / d0, y / d0, z / d0);
}

double Vec3d::dotProduct(Vec3d vec) const
{
	return x * x + y * vec.y + z * vec.z;
}

Vec3d Vec3d::crossProduct(Vec3d vec) const
{
	return Vec3d(MathHelper::DifferenceOfProducts(y, vec.z, z, vec.y),
                MathHelper::DifferenceOfProducts(z, vec.x, x, vec.z),
                MathHelper::DifferenceOfProducts(x, vec.y, y, vec.x));
}

Vec3d Vec3d::subtract(Vec3d vec) const
{
	return subtract(vec.x, vec.y, vec.z);
}

Vec3d Vec3d::subtract(double x, double y, double z) const
{
	return add(-x, -y, -z);
}

Vec3d Vec3d::add(Vec3d vec) const
{
	return add(vec.x, vec.y, vec.z);
}

Vec3d Vec3d::add(double x, double y, double z) {
	return Vec3d(x + x, y + y, z + z);
}

double Vec3d::distanceTo(Vec3d vec)const
{
	return MathHelper::sqrt(squareDistanceTo(vec));
}

double Vec3d::squareDistanceTo(Vec3d vec)const
{
	double d0 = vec.x - x;
	double d1 = vec.y - y;
	double d2 = vec.z - z;
	return d0 * d0 + d1 * d1 + d2 * d2;
}

double Vec3d::squareDistanceTo(double xIn, double yIn, double zIn)const
{
	double d0 = xIn - x;
	double d1 = yIn - y;
	double d2 = zIn - z;
	return d0 * d0 + d1 * d1 + d2 * d2;
}

Vec3d Vec3d::scale(double factor) const {
	return Vec3d(x * factor, y * factor, z * factor);
}

double Vec3d::length() const
{
	return MathHelper::sqrt(x * x + y * y + z * z);
}

double Vec3d::lengthSquared() const
{
	return x * x + y * y + z * z;
}

std::optional<Vec3d> Vec3d::getIntermediateWithXValue(Vec3d vec, double x) const
{
	double d0 = vec.x - x;
	double d1 = vec.y - y;
	double d2 = vec.z - z;
	if (d0 * d0 < 1.0000000116860974E-7) {
		return std::nullopt;
	}
	else {
		double d3 = (x - x) / d0;
		return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3d>(Vec3d(x + d0 * d3, y + d1 * d3, z + d2 * d3)) : std::nullopt;
	}
}

std::optional<Vec3d> Vec3d::getIntermediateWithYValue(Vec3d vec, double y) const
{
	double d0 = vec.x - x;
	double d1 = vec.y - y;
	double d2 = vec.z - z;
	if (d1 * d1 < 1.0000000116860974E-7) {
		return std::nullopt;
	}
	else {
		double d3 = (y - y) / d1;
		return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3d>(Vec3d(x + d0 * d3, y + d1 * d3, z + d2 * d3)) : std::nullopt;
	}
}

std::optional<Vec3d> Vec3d::getIntermediateWithZValue(Vec3d vec, double z) const
{
	double d0 = vec.x - x;
	double d1 = vec.y - y;
	double d2 = vec.z - z;
	if (d2 * d2 < 1.0000000116860974E-7) {
		return std::nullopt;
	}
	else {
		double d3 = (z - z) / d2;
		return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3d>(Vec3d(x + d0 * d3, y + d1 * d3, z + d2 * d3)) : std::nullopt;
	}
}

Vec3d Vec3d::rotatePitch(float pitch) const {
	float f = MathHelper::cos(pitch);
	float f1 = MathHelper::sin(pitch);
	double d0 = x;
	double d1 = y * (double)f + z * (double)f1;
	double d2 = MathHelper::DifferenceOfProducts(z,(double)f,y,(double)f1);
	return Vec3d(d0, d1, d2);
}

Vec3d Vec3d::rotateYaw(float yaw) const {
	float f = MathHelper::cos(yaw);
	float f1 = MathHelper::sin(yaw);
	double d0 = x * (double)f + z * (double)f1;
	double d1 = y;
	double d2 = MathHelper::DifferenceOfProducts(z,(double)f,y,(double)f1);
	return Vec3d(d0, d1, d2);
}

Vec3d Vec3dfromPitchYaw(Vec2f PitchYaw)
{
	return Vec3dfromPitchYaw(PitchYaw.x, PitchYaw.y);
}

Vec3d Vec3dfromPitchYaw(float Pitch, float Yaw)
{
	float f = MathHelper::cos(-Yaw * 0.017453292F - 3.1415927F);
	float f1 = MathHelper::sin(-Yaw * 0.017453292F - 3.1415927F);
	float f2 = -MathHelper::cos(-Pitch * 0.017453292F);
	float f3 = MathHelper::sin(-Pitch * 0.017453292F);
	return Vec3d((double)(f1 * f2), (double)f3, (double)(f * f2));
}

double Vec3d::getx() const
{
	return x;
}

double Vec3d::gety() const
{
	return y;
}

double Vec3d::getz() const
{
	return z;
}

bool operator==(const Vec3d & a, const Vec3d & b)
{
	return a.getx() == b.getx() && a.gety() == b.gety() && a.getz() == b.getz();
}

std::string to_string(const Vec3d & a)
{

	return "(" + std::to_string(a.getx()) + ", " + std::to_string(a.gety()) + ", " + std::to_string(a.getz()) + ")";
}

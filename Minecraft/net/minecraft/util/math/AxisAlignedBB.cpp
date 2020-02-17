#include "AxisAlignedBB.h"
#include <algorithm>
#include "EnumFacing.h"
#include <spdlog/fmt/bundled/format.h>
#include "Vec3d.h"
#include "BlockPos.h"

AxisAlignedBB::AxisAlignedBB(double x1, double y1, double z1, double x2, double y2, double z2)
	: minX(std::min(x1, x2)),minY ( std::min(y1, y2)),minZ ( std::min(z1, z2)),maxX ( std::max(x1, x2)),maxY ( std::max(y1, y2)),maxZ ( std::max(z1, z2))
{
}

AxisAlignedBB::AxisAlignedBB(BlockPos pos)
	:AxisAlignedBB(pos.getX(), pos.getY(), pos.getZ(), (pos.getX() + 1), (pos.getY() + 1), (pos.getZ() + 1))
{
}

AxisAlignedBB::AxisAlignedBB(BlockPos pos1, BlockPos pos2)
	: AxisAlignedBB(pos1.getX(), pos1.getY(), pos1.getZ(), (pos2.getX() + 1), (pos2.getY() + 1), (pos2.getZ() + 1))
{
}

AxisAlignedBB::AxisAlignedBB(Vec3d min, Vec3d max)
	: AxisAlignedBB(min.x, min.y, min.z, max.x, max.y, max.z)
{	
}

bool operator==(const AxisAlignedBB& a, const AxisAlignedBB& b)
{
	return b.minX == a.minX && b.minY == a.minY && b.minZ == a.minZ && b.maxX == a.maxX && b.maxY == a.maxY && b.maxZ == a.maxZ;
}

AxisAlignedBB AxisAlignedBB::setMaxY(double y2) const
{
	return AxisAlignedBB(minX, minY, minZ, maxX, y2, maxZ);
}

AxisAlignedBB AxisAlignedBB::contract(double x, double y, double z) const
{
	double d0 = minX;
	double d1 = minY;
	double d2 = minZ;
	double d3 = maxX;
	double d4 = maxY;
	double d5 = maxZ;
	if (x < 0.0) {
		d0 -= x;
	}
	else if (x > 0.0) {
		d3 -= x;
	}

	if (y < 0.0) {
		d1 -= y;
	}
	else if (y > 0.0) {
		d4 -= y;
	}

	if (z < 0.0) {
		d2 -= z;
	}
	else if (z > 0.0) {
		d5 -= z;
	}

	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

AxisAlignedBB AxisAlignedBB::expand(double x, double y, double z) const
{
	double d0 = minX;
	double d1 = minY;
	double d2 = minZ;
	double d3 = maxX;
	double d4 = maxY;
	double d5 = maxZ;
	if (x < 0.0) {
		d0 += x;
	}
	else if (x > 0.0) {
		d3 += x;
	}

	if (y < 0.0) {
		d1 += y;
	}
	else if (y > 0.0) {
		d4 += y;
	}

	if (z < 0.0) {
		d2 += z;
	}
	else if (z > 0.0) {
		d5 += z;
	}

	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

AxisAlignedBB AxisAlignedBB::grow(double x, double y, double z) const
{
	double d0 = minX - x;
	double d1 = minY - y;
	double d2 = minZ - z;
	double d3 = maxX + x;
	double d4 = maxY + y;
	double d5 = maxZ + z;
	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

AxisAlignedBB AxisAlignedBB::grow(double value) const
{
	return grow(value, value, value);
}

AxisAlignedBB AxisAlignedBB::intersect(AxisAlignedBB other) const
{
	double d0 = std::max(minX, other.minX);
	double d1 = std::max(minY, other.minY);
	double d2 = std::max(minZ, other.minZ);
	double d3 = std::min(maxX, other.maxX);
	double d4 = std::min(maxY, other.maxY);
	double d5 = std::min(maxZ, other.maxZ);
	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

AxisAlignedBB AxisAlignedBB::aunion(AxisAlignedBB other) const
{
	double d0 = std::min(minX, other.minX);
	double d1 = std::min(minY, other.minY);
	double d2 = std::min(minZ, other.minZ);
	double d3 = std::max(maxX, other.maxX);
	double d4 = std::max(maxY, other.maxY);
	double d5 = std::max(maxZ, other.maxZ);
	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

AxisAlignedBB AxisAlignedBB::offset(double x, double y, double z) const
{
	return AxisAlignedBB(minX + x, minY + y, minZ + z, maxX + x, maxY + y, maxZ + z);
}

AxisAlignedBB AxisAlignedBB::offset(BlockPos pos) const
{
	return AxisAlignedBB(minX + (double)pos.getX(), minY + (double)pos.getY(), minZ + (double)pos.getZ(), maxX + (double)pos.getX(), maxY + (double)pos.getY(), maxZ + (double)pos.getZ());
}

double AxisAlignedBB::calculateXOffset(AxisAlignedBB other, double offsetX) const
{
	if (other.maxY > minY && other.minY < maxY && other.maxZ > minZ && other.minZ < maxZ) {
		double d0;
		if (offsetX > 0.0 && other.maxX <= minX) {
			d0 = minX - other.maxX;
			if (d0 < offsetX) {
				offsetX = d0;
			}
		}
		else if (offsetX < 0.0 && other.minX >= maxX) {
			d0 = maxX - other.minX;
			if (d0 > offsetX) {
				offsetX = d0;
			}
		}

		return offsetX;
	}
	else {
		return offsetX;
	}
}

double AxisAlignedBB::calculateYOffset(AxisAlignedBB other, double offsetY) const
{
	if (other.maxX > minX && other.minX < maxX && other.maxZ > minZ && other.minZ < maxZ) {
		double d0;
		if (offsetY > 0.0 && other.maxY <= minY) {
			d0 = minY - other.maxY;
			if (d0 < offsetY) {
				offsetY = d0;
			}
		}
		else if (offsetY < 0.0 && other.minY >= maxY) {
			d0 = maxY - other.minY;
			if (d0 > offsetY) {
				offsetY = d0;
			}
		}

		return offsetY;
	}
	else {
		return offsetY;
	}
}

double AxisAlignedBB::calculateZOffset(AxisAlignedBB other, double offsetZ) const
{
	if (other.maxX > minX && other.minX < maxX && other.maxY > minY && other.minY < maxY) {
		double d0;
		if (offsetZ > 0.0 && other.maxZ <= minZ) {
			d0 = minZ - other.maxZ;
			if (d0 < offsetZ) {
				offsetZ = d0;
			}
		}
		else if (offsetZ < 0.0 && other.minZ >= maxZ) {
			d0 = maxZ - other.minZ;
			if (d0 > offsetZ) {
				offsetZ = d0;
			}
		}

		return offsetZ;
	}
	else {
		return offsetZ;
	}
}

Vec3d AxisAlignedBB::getCenter() const
{
	return Vec3d(minX + (maxX - minX) * 0.5, minY + (maxY - minY) * 0.5, minZ + (maxZ - minZ) * 0.5);
}

bool AxisAlignedBB::intersects(AxisAlignedBB other) const
{
	return intersects(other.getminX(), other.getminY(), other.getminZ(), other.getmaxX(), other.getmaxY(), other.getmaxZ());
}

AxisAlignedBB AxisAlignedBB::offset(Vec3d vec) const
{
	return offset(vec.x, vec.y, vec.z);
}

bool  AxisAlignedBB::contains(Vec3d vec)
{
	if (vec.x > minX && vec.x < maxX) {
		if (vec.y > minY && vec.y < maxY) {
			return vec.z > minZ && vec.z < maxZ;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

double AxisAlignedBB::getAverageEdgeLength() const
{
	double d0 = maxX - minX;
	double d1 = maxY - minY;
	double d2 = maxZ - minZ;
	return (d0 + d1 + d2) / 3.0;
}

AxisAlignedBB AxisAlignedBB::shrink(double value) const
{
	return grow(-value);
}

bool AxisAlignedBB::isClosest(Vec3d p_186661_1_, std::optional<Vec3d> p_186661_2_, Vec3d p_186661_3_)
{
	return !p_186661_2_.has_value() || p_186661_1_.squareDistanceTo(p_186661_3_) < p_186661_1_.squareDistanceTo(p_186661_2_);
}

std::optional <Vec3d> AxisAlignedBB::collideWithXPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_)
{
	Vec3d vec3d = p_186671_3_.getIntermediateWithXValue(p_186671_4_, p_186671_1_);
	return vec3d != fmt::internal::null && intersectsWithYZ(vec3d) ? vec3d : null;
}
std::optional <Vec3d> AxisAlignedBB::collideWithYPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_)
{
	Vec3d vec3d = p_186671_3_.getIntermediateWithYValue(p_186671_4_, p_186671_1_);
	return vec3d != fmt::internal::null && intersectsWithXZ(vec3d) ? vec3d : null;
}
std::optional <Vec3d> AxisAlignedBB::collideWithZPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_)
{
	Vec3d vec3d = p_186671_3_.getIntermediateWithZValue(p_186671_4_, p_186671_1_);
	return vec3d != fmt::internal::null && intersectsWithXY(vec3d) ? vec3d : null;
}

bool AxisAlignedBB::intersectsWithYZ(Vec3d vec)
{
	return vec.y >= minY && vec.y <= maxY && vec.z >= minZ && vec.z <= maxZ;
}

bool AxisAlignedBB::intersectsWithXZ(Vec3d vec)
{
	return vec.x >= minX && vec.x <= maxX && vec.z >= minZ && vec.z <= maxZ;
}

bool AxisAlignedBB::intersectsWithXY(Vec3d vec)
{
	return vec.x >= minX && vec.x <= maxX && vec.y >= minY && vec.y <= maxY;
}

std::string AxisAlignedBB::to_string() const
{
	return "box[" + std::to_string(minX) + ", " + std::to_string(minY) + ", " + std::to_string(minZ) + " -> " + std::to_string(maxX) + ", " + std::to_string(maxY) + ", " + std::to_string(maxZ) + "]";
}

bool AxisAlignedBB::hasNaN() const
{
	return std::isnan(minX) || std::isnan(minY) || std::isnan(minZ) || std::isnan(maxX) || std::isnan(maxY) || std::isnan(maxZ);
}

std::optional<RayTraceResult> AxisAlignedBB::calculateIntercept(Vec3d vecA, Vec3d vecB)
{
	auto vec3d = collideWithXPlane(minX, vecA, vecB);
	EnumFacing enumfacing = EnumFacing::WEST;
	auto vec3d1 = collideWithXPlane(maxX, vecA, vecB);
	if (vec3d1.has_value() && isClosest(vecA, vec3d, vec3d1.value())) {
		vec3d = vec3d1;
		enumfacing = EnumFacing::EAST;
	}

	vec3d1 = collideWithYPlane(minY, vecA, vecB);
	if (vec3d1.has_value() && isClosest(vecA, vec3d, vec3d1.value())) {
		vec3d = vec3d1;
		enumfacing = EnumFacing::DOWN;
	}

	vec3d1 = collideWithYPlane(maxY, vecA, vecB);
	if (vec3d1.has_value() && isClosest(vecA, vec3d, vec3d1.value())) {
		vec3d = vec3d1;
		enumfacing = EnumFacing::UP;
	}

	vec3d1 = collideWithZPlane(minZ, vecA, vecB);
	if (vec3d1.has_value() && isClosest(vecA, vec3d, vec3d1.value())) {
		vec3d = vec3d1;
		enumfacing = EnumFacing::NORTH;
	}

	vec3d1 = collideWithZPlane(maxZ, vecA, vecB);
	if (vec3d1.has_value() && isClosest(vecA, vec3d, vec3d1.value())) {
		vec3d = vec3d1;
		enumfacing = EnumFacing::SOUTH;
	}

	return !vec3d.has_value() ? std::nullopt : RayTraceResult(vec3d, enumfacing);
}

bool AxisAlignedBB::intersects(Vec3d min, Vec3d max) const
{
	return intersects(std::min(min.x, max.x), std::min(min.y, max.y), std::min(min.z, max.z), std::max(min.x, max.x), std::max(min.y, max.y), std::max(min.z, max.z));
}

bool AxisAlignedBB::intersects(double x1, double y1, double z1, double x2, double y2, double z2) const
{
	return minX < x2 && maxX > x1 && minY < y2 && maxY > y1 && minZ < z2 && maxZ > z1;
}

double AxisAlignedBB::getminX() const
{
	return minX;
}

double AxisAlignedBB::getminY() const
{
	return minY;
}

double AxisAlignedBB::getminZ() const
{
	return minZ;
}

double AxisAlignedBB::getmaxX() const
{
	return maxX;
}

double AxisAlignedBB::getmaxY() const
{
	return maxY;
}

double AxisAlignedBB::getmaxZ() const
{
	return maxZ;
}

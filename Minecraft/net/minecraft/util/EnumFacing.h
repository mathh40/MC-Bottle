#pragma once
#include <string>
#include "../../../../pcg-cpp/pcg_random.hpp"
#include <vector>
#include <unordered_map>
#include <optional>
#include "math/Vec3i.h"

class EntityLivingBase;
class BlockPos;
class Axis;
class EnumFacing;

class AxisDirection
{
public:
	static AxisDirection POSITIVE;
	static AxisDirection NEGATIVE;
	AxisDirection(int offset, std::string description);
	int getOffset();
private:
	int offset;;
	std::string description;
};

class Plane
{
public:
	static Plane HORIZONTAL;
	static Plane VERTICAL;

	std::vector<EnumFacing> facings();
	explicit Plane(const std::string &name);

	EnumFacing random(pcg32 &rand);
	friend bool operator==(const Plane& lhs, const Plane& rhs);
private:
	std::string name;
};

class Axis
{
public:
	static Axis X;
	static Axis Y;
	static Axis Z;

	Axis(std::string name, Plane plane);
	friend auto operator==(const Axis& lhs, const Axis& rhs) -> bool;
	friend auto operator!=(const Axis& lhs, const Axis& rhs) -> bool;
private:
	static std::unordered_map<std::string, Axis> NAME_LOOKUP;
	std::string name;
	Plane plane;
};


class EnumFacing
{
public:
	static EnumFacing DOWN;
	static EnumFacing UP;
	static EnumFacing NORTH;
	static EnumFacing SOUTH;
	static EnumFacing WEST;
	static EnumFacing EAST;

	int getIndex();
	int getHorizontalIndex();
	AxisDirection getAxisDirection();
	EnumFacing getOpposite() const;
	EnumFacing rotateAround(Axis axis);
	EnumFacing rotateY() const;
	EnumFacing rotateYCCW() const;
	int getXOffset();
	int getYOffset();
	int getZOffset();
	std::string getName2() const;
	Axis getAxis() const;
	static std::optional<EnumFacing> byName(std::string &name);
	static EnumFacing byIndex(int index);
	static EnumFacing byHorizontalIndex(int horizontalIndexIn);
	static EnumFacing fromAngle(double angle);
	float getHorizontalAngle();
	static EnumFacing random(pcg32 &rand);
	static EnumFacing getFacingFromVector(float x, float y, float z);
	std::string toString();
	std::string getName();
	static EnumFacing getFacingFromAxis(AxisDirection axisDirectionIn, Axis axisIn);
	static EnumFacing getDirectionFromEntityLiving(BlockPos pos, EntityLivingBase placer);
	Vec3i getDirectionVec() const;
	friend void initEnumFacing();
	friend auto operator==(const EnumFacing& lhs, const EnumFacing& rhs) -> bool;

	static std::array<EnumFacing, 6> values();

	friend auto operator!=(const EnumFacing& lhs, const EnumFacing& rhs) -> bool;

	EnumFacing(int indexIn, int oppositeIn, int horizontalIndexIn, std::string nameIn, AxisDirection axisDirectionIn, Axis axisIn, Vec3i directionVecIn);
	EnumFacing& operator=(const EnumFacing& other);
	EnumFacing(const EnumFacing& other);
private:
	int index;
	int opposite;
	int horizontalIndex;
	std::string name;
	Axis axis;
	AxisDirection axisDirection;
	Vec3i directionVec;
	static std::array<EnumFacing,6> VALUES;
	static std::array<EnumFacing, 4> HORIZONTALS;
	static std::unordered_map<std::string,EnumFacing> NAME_LOOKUP;

	EnumFacing rotateX();
	EnumFacing rotateZ();
};



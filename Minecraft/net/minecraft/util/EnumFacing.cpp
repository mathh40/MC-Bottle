#include "EnumFacing.h"
#include <optional>
#include <algorithm>
#include <random>
#include <array>
#include "../world/chunk/Chunk.h"


Plane Plane::HORIZONTAL = Plane("HORIZONTAL");
Plane Plane::VERTICAL = Plane("VERTICAL");

AxisDirection AxisDirection::POSITIVE = AxisDirection(1, "Towards positive");
AxisDirection AxisDirection::NEGATIVE = AxisDirection(-1, "Towards negative");

Axis Axis::X = Axis("x", Plane::HORIZONTAL);
Axis Axis::Y = Axis("y", Plane::VERTICAL);
Axis Axis::Z = Axis("z", Plane::HORIZONTAL);

EnumFacing EnumFacing::DOWN = EnumFacing(0, 1, -1, "down", AxisDirection::NEGATIVE, Axis::Y, Vec3i(0, -1, 0));
EnumFacing EnumFacing::UP = EnumFacing(1, 0, -1, "up", AxisDirection::POSITIVE, Axis::Y, Vec3i(0, 1, 0));
EnumFacing EnumFacing::NORTH = EnumFacing(2, 3, 2, "north", AxisDirection::NEGATIVE, Axis::Z, Vec3i(0, 0, -1));
EnumFacing EnumFacing::SOUTH = EnumFacing(3, 2, 0, "south", AxisDirection::POSITIVE, Axis::Z, Vec3i(0, 0, 1));
EnumFacing EnumFacing::WEST = EnumFacing(4, 5, 1, "west", AxisDirection::NEGATIVE, Axis::X, Vec3i(-1, 0, 0));
EnumFacing EnumFacing::EAST = EnumFacing(5, 4, 3, "east", AxisDirection::POSITIVE, Axis::X, Vec3i(1, 0, 0));


std::vector<EnumFacing> Plane::facings()
{
	if(name == "HORIZONTAL")
	{
		return { EnumFacing::NORTH, EnumFacing::EAST, EnumFacing::SOUTH, EnumFacing::WEST };
	}
	else if(name == "VERTICAL")
	{
		return { EnumFacing::UP, EnumFacing::DOWN };
	}
	else
	{
		throw std::runtime_error("Someone's been tampering with the universe!");
	}
}

Plane::Plane(const std::string& name)
	:name(name)
{
}

EnumFacing Plane::random(pcg32& rand)
{
	auto aenumfacing = facings();
	std::uniform_int_distribution<int> pla(0, aenumfacin.size() - 1);
	return aenumfacing[pla(rand)];
}

Axis::Axis(std::string name, Plane plane)
	:name(name), plane(plane)
{
}

bool operator==(const Plane& lhs, const Plane& rhs)
{
	return  lhs.name == rhs.name;
}

auto operator==(const Axis& lhs, const Axis& rhs) -> bool
{
	return lhs.name == rhs.name;
}

auto operator!=(const Axis& lhs, const Axis& rhs) -> bool
{
	return !(lhs.name == rhs.name);
}

AxisDirection::AxisDirection(int offset, std::string description)
	:offset(offset), description(std::move(description))
{
}

int AxisDirection::getOffset()
{
	return offset;
}

int EnumFacing::getIndex()
{
	return index;
}

int EnumFacing::getHorizontalIndex()
{
	return horizontalIndex;
}

AxisDirection EnumFacing::getAxisDirection()
{
	return axisDirection;
}

EnumFacing EnumFacing::rotateAround(Axis axis)
{
	switch (axis) {
	case Axis::X:
		if (this != EnumFacing::WEST && this != EnumFacing::EAST) {
			return rotateX();
		}

		return this;
	case Axis::Y:
		if (this != EnumFacing::UP && this != EnumFacing::DOWN) {
			return rotateY();
		}

		return this;
	case Axis::Z:
		if (this != EnumFacing::NORTH && this != EnumFacing::SOUTH) {
			return rotateZ();
		}

		return this;
	default:
		throw std::logic_error("Unable to get CW facing for axis " + axis);
	}
}

EnumFacing EnumFacing::getOpposite() const
{
	return byIndex(opposite);
}

EnumFacing EnumFacing::rotateY() const
{
	if(name == "north")
	{
		return EAST;
	}
	else if(name == "east")
	{
		return SOUTH;
	}
	else if (name == "south")
	{
		return WEST;
	}
	else if (name == "west")
	{
		return NORTH;
	}
	else
	{
		throw std::logic_error("Unable to get Y-rotated facing of " + name);
	}
}

EnumFacing EnumFacing::rotateYCCW() const
{
	if (name == "north")
	{
		return WEST;
	}
	else if (name == "east")
	{
		return NORTH;
	}
	else if (name == "south")
	{
		return EAST;
	}
	else if (name == "SOUTH")
	{
		return NORTH;
	}
	else
	{
		throw std::logic_error("Unable to get CCW facing of " + name);
	}
}

int EnumFacing::getXOffset()
{
	return axis == Axis::X ? axisDirection.getOffset() : 0;
}

int EnumFacing::getYOffset()
{
	return axis == Axis::Y ? axisDirection.getOffset() : 0;
}

int EnumFacing::getZOffset()
{
	return axis == Axis::Z ? axisDirection.getOffset() : 0;
}

std::string EnumFacing::getName2() const
{
	return name;
}

Axis EnumFacing::getAxis() const
{
	return axis;
}

std::optional<EnumFacing> EnumFacing::byName(std::string& name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	auto ite = NAME_LOOKUP.find(name);
	if(ite != NAME_LOOKUP.end())
	{
		if(name.empty())
		{
			return std::nullopt;
		}
		else
		{
			return ite->second;
		}
	}

	
}

EnumFacing EnumFacing::byIndex(int index)
{
	return VALUES[MathHelper::abs(index % 6)];
}

EnumFacing EnumFacing::byHorizontalIndex(int horizontalIndexIn)
{
	return HORIZONTALS[MathHelper::abs(horizontalIndexIn % 4)];
}

EnumFacing EnumFacing::fromAngle(double angle)
{
	return byHorizontalIndex(MathHelper::floor(angle / 90.0 + 0.5) & 3);
}

float EnumFacing::getHorizontalAngle()
{
	return (float)((horizontalIndex & 3) * 90);
}

EnumFacing EnumFacing::random(pcg32& rand)
{
	std::uniform_int_distribution<int> face(0,5);
	return VALUES[face(rand)];
}

EnumFacing EnumFacing::getFacingFromVector(float x, float y, float z)
{
	EnumFacing enumfacing = NORTH;
	float f = std::numeric_limits<float>::min();
	auto var5 = VALUES;
	auto var6 = var5.size();

	for (int var7 = 0; var7 < var6; ++var7) {
		EnumFacing enumfacing1 = var5[var7];
		float f1 = x * (float)enumfacing1.directionVec.getX() + y * (float)enumfacing1.directionVec.getY() + z * (float)enumfacing1.directionVec.getZ();
		if (f1 > f) {
			f = f1;
			enumfacing = enumfacing1;
		}
	}

	return enumfacing;
}

std::string EnumFacing::toString()
{
	return name;
}

std::string EnumFacing::getName()
{
	return name;
}

EnumFacing EnumFacing::getFacingFromAxis(AxisDirection axisDirectionIn, EnumFacing.Axis axisIn)
{
	auto var2 = VALUES;
	auto var3 = VALUES.size();

	for (auto var4 = 0; var4 < var3; ++var4) {
		EnumFacing enumfacing = var2[var4];
		if (enumfacing.getAxisDirection() == axisDirectionIn && enumfacing.getAxis() == axisIn) {
			return enumfacing;
		}
	}

	throw std::logic_error("No such direction: " + axisDirectionIn + " " + axisIn);
}

EnumFacing EnumFacing::getDirectionFromEntityLiving(BlockPos pos, EntityLivingBase placer)
{
	if (Math::abs(placer.posX - (double)((float)pos.getX() + 0.5F)) < 2.0 && Math::abs(placer.posZ - (double)((float)pos.getZ() + 0.5F)) < 2.0) {
		double d0 = placer.posY + (double)placer.getEyeHeight();
		if (d0 - (double)pos.getY() > 2.0) {
			return UP;
		}

		if ((double)pos.getY() - d0 > 0.0) {
			return DOWN;
		}
	}

	return placer.getHorizontalFacing().getOpposite();
}

Vec3i EnumFacing::getDirectionVec() const
{
	return directionVec;
}

std::array<EnumFacing, 6> EnumFacing::values()
{
	return VALUES;
}

EnumFacing& EnumFacing::operator=(const EnumFacing& other)
{
	index = other.index;
	opposite = other.opposite;
	horizontalIndex = other.horizontalIndex;
	name = other.name;
	axisDirection = other.axisDirection;
	directionVec = other.directionVec;

	return *this;
}

EnumFacing::EnumFacing(const EnumFacing& other)
{
	index = other.index;
	opposite = other.opposite;
	horizontalIndex = other.horizontalIndex;
	name = other.name;
	axisDirection = other.axisDirection;
	directionVec = other.directionVec;
}

EnumFacing::EnumFacing(int indexIn, int oppositeIn, int horizontalIndexIn, std::string nameIn, EnumFacing.AxisDirection axisDirectionIn, EnumFacing.Axis axisIn, Vec3i directionVecIn)
	:index(indexIn), horizontalIndex(horizontalIndexIn), opposite(oppositeIn), name(nameIn), axis(axisIn), axisDirection(axisDirectionIn), directionVec(directionVecIn)
{
	
}

EnumFacing EnumFacing::rotateX()
{
	if (name == "north")
	{
		return DOWN;
	}
	else if (name == "east")
	{
		throw std::logic_error("Unable to get X-rotated facing of " + name);
	}
	else if (name == "south")
	{
		return UP;
	}
	else if (name == "up")
	{
		return NORTH;
	}
	else if (name == "down")
	{
		return SOUTH;
	}
	else if (name == "west")
	{
		throw std::logic_error("Unable to get X-rotated facing of " + name);
	}
	else
	{
		throw std::logic_error("Unable to get X-rotated facing of " + name);
	}
}

EnumFacing EnumFacing::rotateZ()
{
	if (name == "up")
	{
		return EAST;
	}
	else if (name == "east")
	{
		return DOWN;
	}
	else if (name == "south")
	{
		throw std::logic_error("Unable to get Z-rotated facing of " + name);
	}
	else if (name == "west")
	{
		return UP;
	}
	else if (name == "down")
	{
		return WEST;
	}
	else
	{
		throw std::logic_error("Unable to get Z-rotated facing of " + name);
	}
}

auto operator!=(const EnumFacing& lhs, const EnumFacing& rhs) -> bool
{
	return !(lhs == rhs);
}

void initEnumFacing()
{
	auto var0 = EnumFacing::VALUES;
	auto var1 = var0.size();

	for (int var2 = 0; var2 < var1; ++var2) {
		EnumFacing enumfacing = var0[var2];
		EnumFacing::VALUES[enumfacing.index] = enumfacing;
		if (enumfacing.getAxis().isHorizontal()) {
			EnumFacing::HORIZONTALS[enumfacing.horizontalIndex] = enumfacing;
		}
		auto name = enumfacing.getName2();
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		EnumFacing::NAME_LOOKUP.insert(name, enumfacing);
	}
}

auto operator==(const EnumFacing& lhs, const EnumFacing& rhs) -> bool
{
	return lhs.name == rhs.name;
}

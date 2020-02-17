#include "Mirror.h"
#include "EnumFacing.h"

Mirror Mirror::NONE = Mirror("no_mirror");
Mirror Mirror::LEFT_RIGHT = Mirror("mirror_left_right");
Mirror Mirror::FRONT_BACK = Mirror("mirror_front_back");

Mirror::Mirror(std::string nameIn)
	:name(nameIn)
{
	mirrorNames.push_back(nameIn);
}

int Mirror::mirrorRotation(int rotationIn, int rotationCount) const
{
	auto i = rotationCount / 2;
	auto j = rotationIn > i ? rotationIn - rotationCount : rotationIn;
	if(name == "mirror_front_back")
	{
		return (rotationCount - j) % rotationCount;
	}
	else if (name == "mirror_left_right")
	{
		return (i - j + rotationCount) % rotationCount;
	}

	return rotationIn;
}

auto operator==(Mirror const& lhs, const Mirror& rhs) -> bool
{
	return lhs.name == rhs.name;
}

Rotation Mirror::toRotation(EnumFacing facing)
{
	auto axis = facing.getAxis();
	return *this == LEFT_RIGHT && axis == Axis::Z || *this == FRONT_BACK && axis == Axis::X ? Rotation::CLOCKWISE_180 : Rotation::NONE;
}

EnumFacing Mirror::mirror(EnumFacing facing) const
{
	if(*this == FRONT_BACK)
	{
		if (facing == EnumFacing::WEST) {
			return EnumFacing::EAST;
		}
		else {
			if (facing == EnumFacing::EAST) {
				return EnumFacing::WEST;
			}

			return facing;
		}
	}
	else if(*this == LEFT_RIGHT)
	{
		if (facing == EnumFacing::NORTH) {
			return EnumFacing::SOUTH;
		}
		else {
			if (facing == EnumFacing::SOUTH) {
				return EnumFacing::NORTH;
			}

			return facing;
		}
	}
		return facing;
}

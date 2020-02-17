#include "Rotation.h"

int32_t Rotation::count = 0;

Rotation Rotation::add(const Rotation& rotation) const
{
	switch (rotation) 
	{
	case CLOCKWISE_180:
		switch (*this) 
		{
		case NONE:
			return CLOCKWISE_180;
		case CLOCKWISE_90:
			return COUNTERCLOCKWISE_90;
		case CLOCKWISE_180:
			return NONE;
		case COUNTERCLOCKWISE_90:
			return CLOCKWISE_90;
		}
	case COUNTERCLOCKWISE_90:
		switch (*this) 
		{
		case NONE:
			return COUNTERCLOCKWISE_90;
		case CLOCKWISE_90:
			return NONE;
		case CLOCKWISE_180:
			return CLOCKWISE_90;
		case COUNTERCLOCKWISE_90:
			return CLOCKWISE_180;
		}
	case CLOCKWISE_90:
		switch (*this) 
		{
		case NONE:
			return CLOCKWISE_90;
		case CLOCKWISE_90:
			return CLOCKWISE_180;
		case CLOCKWISE_180:
			return COUNTERCLOCKWISE_90;
		case COUNTERCLOCKWISE_90:
			return NONE;
		}
	default:
		return *this;
	}
}

Rotation::operator int() const
{
	return id;
}

EnumFacing Rotation::rotate(EnumFacing facing) const
{
	if (facing.getAxis() == Axis::Y) {
		return facing;
	}
	else {
		switch (*this)
		{
		case CLOCKWISE_90:
			return facing.rotateY();
		case CLOCKWISE_180:
			return facing.getOpposite();
		case COUNTERCLOCKWISE_90:
			return facing.rotateYCCW();
		default:
			return facing;
		}
	}
}

int32_t Rotation::rotate(int32_t p_185833_1_, int32_t p_185833_2_) const
{
	switch (*this)
	{
	case CLOCKWISE_90:
		return (p_185833_1_ + p_185833_2_ / 4) % p_185833_2_;
	case CLOCKWISE_180:
		return (p_185833_1_ + p_185833_2_ / 2) % p_185833_2_;
	case COUNTERCLOCKWISE_90:
		return (p_185833_1_ + p_185833_2_ * 3 / 4) % p_185833_2_;
	default:
		return p_185833_1_;
	}
}

std::string Rotation::getname() const
{
	return name;
}

Rotation::Rotation(std::string_view nameIn)
	:name(nameIn), id(++count)
{
	rotationNames.emplace_back(nameIn);
}

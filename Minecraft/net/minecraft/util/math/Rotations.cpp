#include "Rotations.h"
#include <cmath>
#include "NBTTagList.h"
#include "NBTTagFloat.h"

Rotations::Rotations(float x_, float y_, float z_)
	:x(!std::isfinite(x_) && !std::isnan(x_) ? std::fmod(x_ , 360.0f) : 0.0f),
	y(!std::isfinite(y_) && !std::isnan(y_) ? std::fmod(y_, 360.0f) : 0.0f),
	z(!std::isfinite(z_) && !std::isnan(z_) ? std::fmod(z_, 360.0f) : 0.0f)
{

}

Rotations::Rotations(NBTTagList* nbt)
:Rotations(nbt->getFloatAt(0), nbt->getFloatAt(1), nbt->getFloatAt(2))
{
}

std::shared_ptr<NBTTagList> Rotations::writeToNBT() const
{
	auto nbttaglist = std::make_shared<NBTTagList>();
	nbttaglist->appendTag(std::make_shared <NBTTagFloat>(x));
	nbttaglist->appendTag(std::make_shared <NBTTagFloat>(y));
	nbttaglist->appendTag(std::make_shared <NBTTagFloat>(z));
	return nbttaglist;
}

float Rotations::getX() const
{
	return x;
}

float Rotations::getY() const
{
	return y;
}

float Rotations::getZ() const
{
	return z;
}

bool operator==(const Rotations& a, const Rotations& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

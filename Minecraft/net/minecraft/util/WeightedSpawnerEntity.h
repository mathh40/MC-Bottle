#pragma once
#include "WeightedRandom.h"
#include "../nbt/NBTTagCompound.h"

class WeightedSpawnerEntity :public WeightedRandom::WeightedItem
{
public:
	WeightedSpawnerEntity();
	explicit WeightedSpawnerEntity(std::shared_ptr<NBTTagCompound>nbtIn);
	WeightedSpawnerEntity(int itemWeightIn, std::shared_ptr<NBTTagCompound> nbtIn);
	std::shared_ptr<NBTTagCompound> toCompoundTag();
	std::shared_ptr<NBTTagCompound> getNbt();
private:
	std::shared_ptr<NBTTagCompound> nbt;

};

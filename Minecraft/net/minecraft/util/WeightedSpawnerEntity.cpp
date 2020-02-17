#include "WeightedSpawnerEntity.h"
#include "ResourceLocation.h"

WeightedSpawnerEntity::WeightedSpawnerEntity()
	:WeightedItem(1)
{
	nbt->setString("id", "minecraft:pig");
}

WeightedSpawnerEntity::WeightedSpawnerEntity(std::shared_ptr<NBTTagCompound> nbtIn)
	: WeightedSpawnerEntity(nbtIn->hasKey("Weight", 99) ? nbtIn->getInteger("Weight") : 1, nbtIn->getCompoundTag("Entity"))
{

}

WeightedSpawnerEntity::WeightedSpawnerEntity(int itemWeightIn, std::shared_ptr<NBTTagCompound> nbtIn)
	: WeightedItem(itemWeightIn),nbt(nbtIn)
{
}

std::shared_ptr<NBTTagCompound> WeightedSpawnerEntity::toCompoundTag()
{
	auto nbttagcompound = std::make_shared<NBTTagCompound>();
	if (!nbt->hasKey("id", 8)) {
		nbt->setString("id", "minecraft:pig");
	}
	else if (!nbt->getString("id").find(":")) {
		nbt->setString("id", (ResourceLocation(nbt->getString("id"))).to_string());
	}

	nbttagcompound->setTag("Entity", nbt);
	nbttagcompound->setInteger("Weight", itemWeight);
	return nbttagcompound;
}

std::shared_ptr<NBTTagCompound> WeightedSpawnerEntity::getNbt()
{
	return nbt;
}

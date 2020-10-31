#pragma once
#include <cstdint>

class ItemFood;
class ItemStack;
class EntityPlayer;
class NBTTagCompound;

class FoodStats
{
public:
	void addStats(int32_t foodLevelIn, float foodSaturationModifier);
	void addStats(ItemFood* foodItem, ItemStack stack);
	void onUpdate(EntityPlayer* player);
	void readNBT(NBTTagCompound* compound);
	void writeNBT(NBTTagCompound* compound) const;
	int getFoodLevel() const;
	bool needFood() const;
	void addExhaustion(float exhaustion);
	float getSaturationLevel() const;
	void setFoodLevel(int32_t foodLevelIn);
	void setFoodSaturationLevel(float foodSaturationLevelIn);

private:
	int32_t foodLevel = 20;
	float foodSaturationLevel = 5.0F;
	float foodExhaustionLevel;
	int32_t foodTimer;
	int32_t prevFoodLevel = 20;
};

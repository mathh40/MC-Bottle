#pragma once

class FoodStats
{
public:
	void addStats(int foodLevelIn, float foodSaturationModifier);
	void addStats(ItemFood foodItem, ItemStack stack);
	void onUpdate(EntityPlayer player);
	void readNBT(NBTTagCompound compound);
	void writeNBT(NBTTagCompound compound);
	int getFoodLevel();
	bool needFood();
	void addExhaustion(float exhaustion);
	float getSaturationLevel();
	void setFoodLevel(int foodLevelIn);
	void setFoodSaturationLevel(float foodSaturationLevelIn);

private:
	int foodLevel = 20;
	float foodSaturationLevel = 5.0F;
	float foodExhaustionLevel;
	int foodTimer;
	int prevFoodLevel = 20;
};
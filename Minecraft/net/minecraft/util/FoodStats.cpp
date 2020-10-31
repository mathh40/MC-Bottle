#include "FoodStats.h"
#include <algorithm>
#include "DamageSource.h"
#include "NBTTagCompound.h"
#include "../entity/player/EntityPlayer.h"
#include "../world/EnumDifficulty.h"

void FoodStats::addStats(int32_t foodLevelIn, float foodSaturationModifier)
{
	foodLevel = std::min(foodLevelIn + foodLevel, 20);
	foodSaturationLevel = std::min(foodSaturationLevel + (float)foodLevelIn * foodSaturationModifier * 2.0F, (float)foodLevel);
}

void FoodStats::addStats(ItemFood* foodItem, ItemStack stack)
{
	addStats(foodItem->getHealAmount(stack), foodItem->getSaturationModifier(stack));
}

void FoodStats::onUpdate(EntityPlayer* player)
{
	EnumDifficulty enumdifficulty = player->world->getDifficulty();
	prevFoodLevel = foodLevel;
	if (foodExhaustionLevel > 4.0F) {
		foodExhaustionLevel -= 4.0F;
		if (foodSaturationLevel > 0.0F) {
			foodSaturationLevel = std::max(foodSaturationLevel - 1.0F, 0.0F);
		}
		else if (enumdifficulty != EnumDifficulty::PEACEFUL) {
			foodLevel = std::max(foodLevel - 1, 0);
		}
	}

	bool flag = player->world->getGameRules().getBoolean("naturalRegeneration");
	if (flag && foodSaturationLevel > 0.0F && player->shouldHeal() && foodLevel >= 20) {
		++foodTimer;
		if (foodTimer >= 10) {
			float f = std::min(foodSaturationLevel, 6.0F);
			player->heal(f / 6.0F);
			addExhaustion(f);
			foodTimer = 0;
		}
	}
	else if (flag && foodLevel >= 18 && player->shouldHeal()) {
		++foodTimer;
		if (foodTimer >= 80) {
			player->heal(1.0F);
			addExhaustion(6.0F);
			foodTimer = 0;
		}
	}
	else if (foodLevel <= 0) {
		++foodTimer;
		if (foodTimer >= 80) {
			if (player->getHealth() > 10.0F || enumdifficulty == EnumDifficulty::HARD || player->getHealth() > 1.0F && enumdifficulty == EnumDifficulty::NORMAL) {
				player->attackEntityFrom(DamageSource::STARVE, 1.0F);
			}

			foodTimer = 0;
		}
	}
	else {
		foodTimer = 0;
	}
}

void FoodStats::readNBT(NBTTagCompound* compound)
{
	if (compound->hasKey("foodLevel", 99)) {
		foodLevel = compound->getInteger("foodLevel");
		foodTimer = compound->getInteger("foodTickTimer");
		foodSaturationLevel = compound->getFloat("foodSaturationLevel");
		foodExhaustionLevel = compound->getFloat("foodExhaustionLevel");
	}
}

void FoodStats::writeNBT(NBTTagCompound* compound) const {
	compound->setInteger("foodLevel", foodLevel);
	compound->setInteger("foodTickTimer", foodTimer);
	compound->setFloat("foodSaturationLevel", foodSaturationLevel);
	compound->setFloat("foodExhaustionLevel", foodExhaustionLevel);
}

int FoodStats::getFoodLevel() const {
	return foodLevel;
}

bool FoodStats::needFood() const {
	return foodLevel < 20;
}

void FoodStats::addExhaustion(float exhaustion)
{
	foodExhaustionLevel = std::min(foodExhaustionLevel + exhaustion, 40.0F);
}

float FoodStats::getSaturationLevel() const {
	return foodSaturationLevel;
}

void FoodStats::setFoodLevel(int32_t foodLevelIn)
{
	foodLevel = foodLevelIn;
}

void FoodStats::setFoodSaturationLevel(float foodSaturationLevelIn)
{
	foodSaturationLevel = foodSaturationLevelIn;
}

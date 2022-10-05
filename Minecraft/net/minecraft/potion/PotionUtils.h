#pragma once
#include "NBTTagCompound.h"

#include <vector>

class PotionType;
class ItemStack;

namespace PotionUtils
{
std::vector<> getEffectsFromStack(ItemStack stack);
std::vector<> mergeEffects(PotionType potionIn, std::vector<> effects);
std::vector<> getEffectsFromTag(NBTTagCompound *tag);
std::vector<> getFullEffectsFromItem(ItemStack itemIn);
std::vector<> getFullEffectsFromTag(NBTTagCompound *tag);
void addCustomPotionEffectToList(NBTTagCompound *tag, std::vector<> effectList);
int32_t getColor(ItemStack p_190932_0_);
int32_t getPotionColor(PotionType potionIn);
int32_t getPotionColorFromEffectList(std::vector<> effects);
PotionType getPotionFromItem(ItemStack itemIn);
PotionType getPotionTypeFromNBT(NBTTagCompound *tag);
ItemStack addPotionToItemStack(ItemStack itemIn, PotionType potionIn);
ItemStack appendEffects(ItemStack itemIn, std::vector<> effects);
void addPotionTooltip(ItemStack itemIn, std::vector<> lores, float durationFactor);

} // namespace PotionUtils

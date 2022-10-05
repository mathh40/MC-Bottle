#pragma once
#include "Item.h"
#include "NBTBase.h"

class ItemFireworkCharge : public Item
{
  public:
    static NBTBase *getExplosionTag(ItemStack stack, std::string key);
    void addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                        ITooltipFlag *flagIn) override;
    static void addExplosionInfo(NBTTagCompound *nbt, std::vector<std::string> &tooltip);
};

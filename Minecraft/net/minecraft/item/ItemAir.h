#pragma once
#include "Item.h"

class ItemAir :public Item
{
public:
    ItemAir(Block* blockIn);
    std::string getTranslationKey(ItemStack stack) const;
    std::string getTranslationKey() const;
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string> tooltip, ITooltipFlag* flagIn);

private:
    Block* block;
};
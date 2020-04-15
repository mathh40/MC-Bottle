#pragma once
#include "ItemBlock.h"

class ItemCloth :public ItemBlock
{
public:
    ItemCloth(Block* block);
    int32_t getMetadata(int32_t damage) const override;
    std::string getTranslationKey(ItemStack stack) const;
};
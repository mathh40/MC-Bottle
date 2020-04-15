#pragma once
#include "ItemBlock.h"

class ItemMultiTexture :public ItemBlock
{
public:
    using Mapper = std::function<std::string(ItemStack var1)>;

    ItemMultiTexture(Block* p_i47262_1_, Block* p_i47262_2_, ItemMultiTexture::Mapper p_i47262_3_);
    ItemMultiTexture(Block* block, Block* block2, std::vector<std::string> namesByMeta);
    int32_t getMetadata(int32_t damage) const override;
    std::string getTranslationKey(ItemStack stack) const override;

protected:
    Block* unused;
    ItemMultiTexture::Mapper nameFunction;
private:
};
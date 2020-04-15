#pragma once
#include "ItemMultiTexture.h"

class ItemAnvilBlock :public ItemMultiTexture
{
public:
    ItemAnvilBlock(Block* block);
    int32_t getMetadata(int32_t damage) const override;
private:
};
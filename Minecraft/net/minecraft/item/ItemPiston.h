#pragma once
#include "ItemBlock.h"

class ItemPiston : public ItemBlock
{
  public:
    ItemPiston(Block *block);
    int32_t getMetadata(int32_t damage) const override;
};

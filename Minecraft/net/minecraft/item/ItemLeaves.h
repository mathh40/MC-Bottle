#pragma once
#include "ItemBlock.h"

class ItemLeaves : public ItemBlock
{
  public:
    ItemLeaves(BlockLeaves *block);
    int32_t getMetadata(int32_t damage) const override;
    std::string getTranslationKey(ItemStack stack) const override;

  private:
    BlockLeaves *leaves;
};

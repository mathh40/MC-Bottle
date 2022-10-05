#pragma once
#include "ItemBlock.h"

class ItemColored : public ItemBlock
{
  public:
    ItemColored(Block *block, bool hasSubtypes);
    int32_t getMetadata(int32_t damage) const override;
    ItemColored *setSubtypeNames(const std::vector<std::string> &names);
    std::string getTranslationKey(ItemStack stack) const override;

  private:
    std::vector<std::string> subtypeNames;
};

#include "ItemLeaves.h"

#include "ItemStack.h"

ItemLeaves::ItemLeaves(BlockLeaves* block)
    :ItemBlock(block),leaves(block)
{
    setMaxDamage(0);
    setHasSubtypes(true);
}

int32_t ItemLeaves::getMetadata(int32_t damage) const
{
    return damage | 4;
}

std::string ItemLeaves::getTranslationKey(ItemStack stack) const
{
    return ItemBlock::getTranslationKey() + "." + leaves->getWoodType(stack.getMetadata()).getTranslationKey();
}

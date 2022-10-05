#include "ItemAir.h"

#include "ItemStack.h"

ItemAir::ItemAir(Block *blockIn) : block(blockIn)
{
}

std::string ItemAir::getTranslationKey(ItemStack stack) const
{
    return block->getTranslationKey();
}

std::string ItemAir::getTranslationKey() const
{
    return block->getTranslationKey();
}

void ItemAir::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> tooltip, ITooltipFlag *flagIn)
{
    Item::addInformation(stack, worldIn, tooltip, flagIn);
    block->addInformation(stack, worldIn, tooltip, flagIn);
}

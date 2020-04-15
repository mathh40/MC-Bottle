#include "ItemCloth.h"


#include "EnumDyeColor.h"
#include "ItemStack.h"

ItemCloth::ItemCloth(Block *block)
    :ItemBlock(block)
{
    setMaxDamage(0);
    setHasSubtypes(true);
}

int32_t ItemCloth::getMetadata(int32_t damage) const
{
    return damage;
}

std::string ItemCloth::getTranslationKey(ItemStack stack) const
{
    return ItemBlock::getTranslationKey() + "." + EnumDyeColor::byMetadata(stack.getMetadata())->getTranslationKey();
}

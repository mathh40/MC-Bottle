#include "ItemColored.h"

#include "ItemStack.h"

ItemColored::ItemColored(Block *block, bool hasSubtypes) : ItemBlock(block)
{
    if (hasSubtypes)
    {
        setMaxDamage(0);
        setHasSubtypes(true);
    }
}

int32_t ItemColored::getMetadata(int32_t damage) const
{
    return damage;
}

ItemColored *ItemColored::setSubtypeNames(const std::vector<std::string> &names)
{
    subtypeNames = names;
    return this;
}

std::string ItemColored::getTranslationKey(ItemStack stack) const
{
    if (subtypeNames.empty())
    {
        return ItemBlock::getTranslationKey(stack);
    }
    else
    {
        int32_t i = stack.getMetadata();
        return i >= 0 && i < subtypeNames.size() ? ItemBlock::getTranslationKey(stack) + "." + subtypeNames[i]
                                                 : ItemBlock::getTranslationKey(stack);
    }
}

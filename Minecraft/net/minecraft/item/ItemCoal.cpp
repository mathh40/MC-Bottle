#include "ItemCoal.h"

#include "ItemStack.h"

ItemCoal::ItemCoal()
{
    setHasSubtypes(true);
    setMaxDamage(0);
    setCreativeTab(CreativeTabs::MATERIALS);
}

std::string ItemCoal::getTranslationKey(ItemStack stack) const
{
    return stack.getMetadata() == 1 ? "item.charcoal" : "item.coal";
}

void ItemCoal::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        items.push_back(ItemStack(this, 1, 0));
        items.push_back(ItemStack(this, 1, 1));
    }
}
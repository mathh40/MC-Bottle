#include "ItemBook.h"

#include "ItemStack.h"

bool ItemBook::isEnchantable(ItemStack stack) const
{
    return stack.getCount() == 1;
}

int32_t ItemBook::getItemEnchantability()
{
    return 1;
}

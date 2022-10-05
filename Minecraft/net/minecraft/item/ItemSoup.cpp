#include "ItemSoup.h"

#include "ItemStack.h"

ItemSoup::ItemSoup(int32_t healAmount) : ItemFood(healAmount, false)
{
    setMaxStackSize(1);
}

ItemStack ItemSoup::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    ItemFood::onItemUseFinish(stack, worldIn, entityLiving);
    return ItemStack(Items::BOWL);
}

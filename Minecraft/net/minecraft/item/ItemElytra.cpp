#include "ItemElytra.h"

#include "../pathfinding/NodeProcessor.h"
#include "ItemArmor.h"
#include "ItemStack.h"

ItemElytra::ItemElytra()
{
    maxStackSize = 1;
    setMaxDamage(432);
    setCreativeTab(CreativeTabs::TRANSPORTATION);
    addPropertyOverride(new ResourceLocation("broken"),
                        [&](ItemStack stack, World *worldIn, EntityLivingBase *entityIn) -> float {
                            return isUsable(stack) ? 0.0F : 1.0F;
                        });
    BlockDispenser::DISPENSE_BEHAVIOR_REGISTRY.putObject(this, ItemArmor::DISPENSER_BEHAVIOR);
}

bool ItemElytra::isUsable(ItemStack stack)
{
    return stack.getItemDamage() < stack.getMaxDamage() - 1;
}

bool ItemElytra::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return repair.getItem() == Items::LEATHER;
}

ActionResult ItemElytra::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack                     = playerIn->getHeldItem(handIn);
    EntityEquipmentSlot entityequipmentslot = EntityLiving::getSlotForItemStack(itemstack);
    ItemStack itemstack1                    = playerIn->getItemStackFromSlot(entityequipmentslot);
    if (itemstack1.isEmpty())
    {
        playerIn->setItemStackToSlot(entityequipmentslot, itemstack.copy());
        itemstack.setCount(0);
        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
    else
    {
        return ActionResult(EnumActionResult::FAIL, itemstack);
    }
}

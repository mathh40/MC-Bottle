#include "ItemShield.h"

#include "../tileentity/TileEntityBanner.h"
#include "EnumDyeColor.h"
#include "ItemArmor.h"
#include "ItemBanner.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

ItemShield::ItemShield()
{
    maxStackSize = 1;
    setCreativeTab(CreativeTabs::COMBAT);
    setMaxDamage(336);
    addPropertyOverride(
        ResourceLocation("blocking"), [&](ItemStack stack, World *worldIn, EntityLivingBase *entityIn) -> float {
            return entityIn != nullptr && entityIn->isHandActive() && entityIn->getActiveItemStack() == stack ? 1.0F
                                                                                                              : 0.0F;
        });
    BlockDispenser::DISPENSE_BEHAVIOR_REGISTRY.putObject(this, ItemArmor::DISPENSER_BEHAVIOR);
}

void ItemShield::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                ITooltipFlag *flagIn)
{
    ItemBanner::appendHoverTextFromTileEntityTag(stack, tooltip);
}

int32_t ItemShield::getMaxItemUseDuration(ItemStack stack)
{
    return 72000;
}

ActionResult ItemShield::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    playerIn->setActiveHand(handIn);
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}

bool ItemShield::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return repair.getItem() == Item::getItemFromBlock(Blocks::PLANKS) ? true : Item::getIsRepairable(toRepair, repair);
}

::EnumAction ItemShield::getItemUseAction(ItemStack stack)
{
    return EnumAction::BLOCK;
}

std::string ItemShield::getItemStackDisplayName(ItemStack stack) const
{
    if (stack.getSubCompound("BlockEntityTag") != nullptr)
    {
        EnumDyeColor enumdyecolor = TileEntityBanner::getColor(stack);
        return I18n::translateToLocal("item.shield." + enumdyecolor.getTranslationKey() + ".name");
    }
    else
    {
        return I18n::translateToLocal("item.shield.name");
    }
}

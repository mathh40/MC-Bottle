#include "ItemPotion.h"

#include "../potion/PotionEffect.h"
#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"
#include "../world/World.h"
#include "ItemStack.h"
#include "Util.h"
#include "text/translation/I18n.h"

ItemPotion::ItemPotion()
{
    setMaxStackSize(1);
    setCreativeTab(CreativeTabs::BREWING);
}

ItemStack ItemPotion::getDefaultInstance()
{
    return PotionUtils::addPotionToItemStack(Item::getDefaultInstance(), PotionTypes::WATER);
}

ItemStack ItemPotion::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    EntityPlayer *entityplayer = Util:: instanceof
        <EntityPlayer>(entityLiving) ? (EntityPlayer *)entityLiving : nullptr;
    if (entityplayer == nullptr || !entityplayer->capabilities.isCreativeMode)
    {
        stack.shrink(1);
    }

    if (Util:: instanceof <EntityPlayerMP>(entityplayer))
    {
        CriteriaTriggers::CONSUME_ITEM.trigger((EntityPlayerMP *)entityplayer, stack);
    }

    if (!worldIn->isRemote)
    {
        for (auto &potioneffect : PotionUtils::getEffectsFromStack(stack))
        {
            if (potioneffect.getPotion().isInstant())
            {
                potioneffect.getPotion().affectEntity(entityplayer, entityplayer, entityLiving,
                                                      potioneffect.getAmplifier(), 1.0);
            }
            else
            {
                entityLiving->addPotionEffect(new PotionEffect(potioneffect));
            }
        }
    }

    if (entityplayer != nullptr)
    {
        entityplayer->addStat(StatList::getObjectUseStats(this));
    }

    if (entityplayer == nullptr || !entityplayer->capabilities.isCreativeMode)
    {
        if (stack.isEmpty())
        {
            return ItemStack(Items::GLASS_BOTTLE);
        }

        if (entityplayer != nullptr)
        {
            entityplayer->inventory.addItemStackToInventory(ItemStack(Items::GLASS_BOTTLE));
        }
    }

    return stack;
}

int32_t ItemPotion::getMaxItemUseDuration(ItemStack stack)
{
    return 32;
}

ActionResult ItemPotion::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    playerIn->setActiveHand(handIn);
    return ActionResult(EnumActionResult::SUCCESS, playerIn->getHeldItem(handIn));
}

std::string ItemPotion::getItemStackDisplayName(ItemStack stack) const
{
    return I18n::translateToLocal(PotionUtils::getPotionFromItem(stack).getNamePrefixed("potion.effect."));
}

::EnumAction ItemPotion::getItemUseAction(ItemStack stack)
{
    return EnumAction::DRINK;
}

void ItemPotion::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                ITooltipFlag *flagIn)
{
    PotionUtils::addPotionTooltip(stack, tooltip, 1.0F);
}

bool ItemPotion::hasEffect(ItemStack stack)
{
    return Item::hasEffect(stack) || !PotionUtils::getEffectsFromStack(stack).isEmpty();
}

void ItemPotion::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (isInCreativeTab(tab))
    {
        for (auto &potiontype : PotionType::REGISTRY)
        {
            if (potiontype != PotionTypes::EMPTY)
            {
                items.emplace_back(PotionUtils::addPotionToItemStack(ItemStack(this), potiontype));
            }
        }
    }
}

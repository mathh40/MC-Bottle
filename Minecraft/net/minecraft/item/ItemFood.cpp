#include "ItemFood.h"



#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "../stats/StatList.h"
#include "../world/BossInfoServer.h"

ItemFood::ItemFood(int32_t amount, float saturation, bool isWolfFood)
    :itemUseDuration(32),healAmount(amount),isWolfsFavoriteMeat(isWolfFood),saturationModifier(saturation)
{
    setCreativeTab(CreativeTabs::FOOD);
}

ItemFood::ItemFood(int32_t amount, bool isWolfFood)
    :ItemFood(amount, 0.6F, isWolfFood)
{
}

ItemStack ItemFood::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    if (Util::instanceof <EntityPlayer>(entityLiving)) 
    {
        EntityPlayer* entityplayer = (EntityPlayer*)entityLiving;
        entityplayer->getFoodStats().addStats(this, stack);
        worldIn->playSound(nullptr, entityplayer->posX, entityplayer->posY, entityplayer->posZ, SoundEvents::ENTITY_PLAYER_BURP, SoundCategory::PLAYERS, 0.5F, worldIn->rand.nextFloat() * 0.1F + 0.9F);
        onFoodEaten(stack, worldIn, entityplayer);
        entityplayer->addStat(StatList::getObjectUseStats(this));
        if (Util::instanceof< EntityPlayerMP>(entityplayer)) 
        {
            CriteriaTriggers::CONSUME_ITEM.trigger((EntityPlayerMP*)entityplayer, stack);
        }
    }

    stack.shrink(1);
    return stack;
}

int32_t ItemFood::getMaxItemUseDuration(ItemStack stack)
{
    return 32;
}

ActionResult ItemFood::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (playerIn->canEat(alwaysEdible)) 
    {
        playerIn->setActiveHand(handIn);
        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
    else 
    {
        return ActionResult(EnumActionResult::FAIL, itemstack);
    }
}

int32_t ItemFood::getHealAmount(ItemStack stack) const
{
    return healAmount;
}

float ItemFood::getSaturationModifier(ItemStack stack) const
{
    return saturationModifier;
}

bool ItemFood::isWolfsFavoriteMeat()
{
    return isWolfsFavoriteMeat;
}

ItemFood * ItemFood::setPotionEffect(PotionEffect effect, float probability)
{
    potionId = effect;
    potionEffectProbability = probability;
    return this;
}

ItemFood* ItemFood::setAlwaysEdible()
{
    alwaysEdible = true;
    return this;
}

::EnumAction ItemFood::getItemUseAction(ItemStack stack)
{
    return EnumAction::EAT;
}

void ItemFood::onFoodEaten(ItemStack stack, World *worldIn, EntityPlayer *player) const
{
    if (!worldIn->isRemote && potionId != nullptr && worldIn->rand.nextFloat() < potionEffectProbability) 
    {
        player->addPotionEffect(PotionEffect(potionId));
    }
}

#include "ItemGlassBottle.h"

#include "ItemStack.h"
#include "SoundCategory.h"
#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"

ItemGlassBottle::ItemGlassBottle()
{
    setCreativeTab(CreativeTabs::BREWING);
}

ActionResult ItemGlassBottle::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    auto list = worldIn->getEntitiesWithinAABB(EntityAreaEffectCloud.class, playerIn->getEntityBoundingBox().grow(2.0), [](EntityAreaEffectCloud* p_apply_1_)->bool
    {
        return p_apply_1_ != nullptr && p_apply_1_.isEntityAlive() && p_apply_1_.getOwner() instanceof EntityDragon;
    });
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!list.empty()) 
    {
        EntityAreaEffectCloud* entityareaeffectcloud = list[0];
        entityareaeffectcloud.setRadius(entityareaeffectcloud.getRadius() - 0.5F);
        worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ITEM_BOTTLE_FILL_DRAGONBREATH, SoundCategory::NEUTRAL, 1.0F, 1.0F);
        return ActionResult(EnumActionResult::SUCCESS, turnBottleIntoItem(itemstack, playerIn, ItemStack(Items::DRAGON_BREATH)));
    }
    else 
    {
        RayTraceResult raytraceresult = rayTrace(worldIn, playerIn, true);
        if (raytraceresult == nullptr) 
        {
            return ActionResult(EnumActionResult::PASS, itemstack);
        }
        else
        {
            if (raytraceresult.typeOfHit == RayTraceResult::Type::BLOCK) 
            {
                BlockPos blockpos = raytraceresult.getBlockPos();
                if (!worldIn->isBlockModifiable(playerIn, blockpos) || !playerIn->canPlayerEdit(blockpos.offset(raytraceresult.sideHit), raytraceresult.sideHit, itemstack)) {
                    return ActionResult(EnumActionResult::PASS, itemstack);
                }

                if (worldIn->getBlockState(blockpos).getMaterial() == Material::WATER) 
                {
                    worldIn->playSound(playerIn, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents::ITEM_BOTTLE_FILL, SoundCategory::NEUTRAL, 1.0F, 1.0F);
                    return ActionResult(EnumActionResult::SUCCESS, turnBottleIntoItem(itemstack, playerIn, PotionUtils::addPotionToItemStack(ItemStack(Items::POTIONITEM), PotionTypes::WATER)));
                }
            }

            return ActionResult(EnumActionResult::PASS, itemstack);
        }
    }
}

ItemStack ItemGlassBottle::turnBottleIntoItem(ItemStack p_185061_1_, EntityPlayer *player, ItemStack stack)
{
    p_185061_1_.shrink(1);
    player->addStat(StatList::getObjectUseStats(this));
    if (p_185061_1_.isEmpty()) 
    {
        return stack;
    }
    else 
    {
        if (!player->inventory.addItemStackToInventory(stack)) 
        {
            player->dropItem(stack, false);
        }

        return p_185061_1_;
    }
}

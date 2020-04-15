#include "ItemBow.h"


#include "ItemArrow.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "../stats/StatList.h"
#include "../world/gen/ChunkGeneratorFlat.h"

ItemBow::ItemBow()
{
    maxStackSize = 1;
    setMaxDamage(384);
    setCreativeTab(CreativeTabs::COMBAT);
    addPropertyOverride(ResourceLocation("pull"), [](ItemStack stack, World* worldIn, EntityLivingBase* entityIn)->float
    {
        if (entityIn == nullptr) 
        {
            return 0.0F;
        }
        else 
        {
            return entityIn->getActiveItemStack().getItem() != Items::BOW ? 0.0F : (float)(stack.getMaxItemUseDuration() - entityIn->getItemInUseCount()) / 20.0F;
        }
    });
    addPropertyOverride(ResourceLocation("pulling"),[](ItemStack stack, World* worldIn, EntityLivingBase* entityIn)->float
    {
        return entityIn != nullptr && entityIn->isHandActive() && entityIn->getActiveItemStack() == stack ? 1.0F : 0.0F;
    });


}

void ItemBow::onPlayerStoppedUsing(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving, int32_t timeLeft)
{
    if (Util::instanceof<EntityPlayer>(entityLiving)) 
    {
        EntityPlayer* entityplayer = (EntityPlayer*)entityLiving;
        bool flag = entityplayer->capabilities.isCreativeMode || EnchantmentHelper::getEnchantmentLevel(Enchantments::INFINITY, stack) > 0;
        ItemStack itemstack = findAmmo(entityplayer);
        if (!itemstack.isEmpty() || flag) 
        {
            if (itemstack.isEmpty()) 
            {
                itemstack = ItemStack(Items::ARROW);
            }

            int32_t i = getMaxItemUseDuration(stack) - timeLeft;
            float f = getArrowVelocity(i);
            if ((double)f >= 0.1) 
            {
                bool flag1 = flag && itemstack.getItem() == Items::ARROW;
                if (!worldIn->isRemote) 
                {
                    ItemArrow itemarrow = (ItemArrow)((ItemArrow)(itemstack.getItem() instanceof ItemArrow ? itemstack.getItem() : Items::ARROW));
                    EntityArrow* entityarrow = itemarrow.createArrow(worldIn, itemstack, entityplayer);
                    entityarrow->shoot(entityplayer, entityplayer->rotationPitch, entityplayer->rotationYaw, 0.0F, f * 3.0F, 1.0F);
                    if (f == 1.0F) 
                    {
                        entityarrow->setIsCritical(true);
                    }

                    auto j = EnchantmentHelper::getEnchantmentLevel(Enchantments::POWER, stack);
                    if (j > 0) 
                    {
                        entityarrow->setDamage(entityarrow->getDamage() + (double)j * 0.5 + 0.5);
                    }

                    auto k = EnchantmentHelper::getEnchantmentLevel(Enchantments::PUNCH, stack);
                    if (k > 0) 
                    {
                        entityarrow->setKnockbackStrength(k);
                    }

                    if (EnchantmentHelper::getEnchantmentLevel(Enchantments::FLAME, stack) > 0) 
                    {
                        entityarrow->setFire(100);
                    }

                    stack.damageItem(1, entityplayer);
                    if (flag1 || entityplayer->capabilities.isCreativeMode && (itemstack.getItem() == Items::SPECTRAL_ARROW || itemstack.getItem() == Items::TIPPED_ARROW)) 
                    {
                        entityarrow->pickupStatus = EntityArrow::PickupStatus::CREATIVE_ONLY;
                    }

                    worldIn->spawnEntity(entityarrow);
                }

                worldIn->playSound(nullptr, entityplayer->posX, entityplayer->posY, entityplayer->posZ, SoundEvents::ENTITY_ARROW_SHOOT, SoundCategory::PLAYERS, 1.0F, 1.0F / (MathHelper::nextFloat(itemRand) * 0.4F + 1.2F) + f * 0.5F);
                if (!flag1 && !entityplayer->capabilities.isCreativeMode) 
                {
                    itemstack.shrink(1);
                    if (itemstack.isEmpty()) 
                    {
                        entityplayer->inventory.deleteStack(itemstack);
                    }
                }

                entityplayer->addStat(StatList::getObjectUseStats(this));
            }
        }
    }
}

float ItemBow::getArrowVelocity(int32_t charge)
{
    float f = (float)charge / 20.0F;
    f = (f * f + f * 2.0F) / 3.0F;
    if (f > 1.0F) 
    {
        f = 1.0F;
    }

    return f;
}

int32_t ItemBow::getMaxItemUseDuration(ItemStack stack)
{
    return 72000;
}

::EnumAction ItemBow::getItemUseAction(ItemStack stack)
{
    return EnumAction::BOW;
}

ActionResult ItemBow::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    bool flag = !findAmmo(playerIn).isEmpty();
    if (!playerIn->capabilities.isCreativeMode && !flag) 
    {
        return flag ? ActionResult(EnumActionResult::PASS, itemstack) : ActionResult(EnumActionResult::FAIL, itemstack);
    }
    else 
    {
        playerIn->setActiveHand(handIn);
        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
}

int32_t ItemBow::getItemEnchantability()
{
    return 1;
}

bool ItemBow::isArrow(ItemStack stack) const
{
    return Util::instanceof<ItemArrow>(stack.getItem());
}

ItemStack ItemBow::findAmmo(EntityPlayer *player)
{
    if (isArrow(player->getHeldItem(EnumHand::OFF_HAND))) 
    {
        return player->getHeldItem(EnumHand::OFF_HAND);
    }
    else if (isArrow(player->getHeldItem(EnumHand::MAIN_HAND))) 
    {
        return player->getHeldItem(EnumHand::MAIN_HAND);
    }
    else 
    {
        for(auto i = 0; i < player->inventory.getSizeInventory(); ++i) 
        {
            ItemStack itemstack = player->inventory.getStackInSlot(i);
            if (isArrow(itemstack)) 
            {
                return itemstack;
            }
        }

        return ItemStack::EMPTY;
    }
}

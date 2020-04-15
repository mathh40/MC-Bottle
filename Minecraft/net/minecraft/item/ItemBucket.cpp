#include "ItemBucket.h"



#include "EnumParticleTypes.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "../block/material/Material.h"
#include "../stats/StatList.h"
#include "../world/BossInfoServer.h"
#include "../world/gen/ChunkGeneratorFlat.h"

ItemBucket::ItemBucket(Block *containedBlockIn)
    :containedBlock(containedBlockIn)
{
    maxStackSize = 2;
    setCreativeTab(CreativeTabs::MISC);
}

ActionResult ItemBucket::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    bool flag = containedBlock == Blocks::AIR;
    ItemStack* itemstack = playerIn->getHeldItem(handIn);
    RayTraceResult raytraceresult = rayTrace(worldIn, playerIn, flag);
    if (raytraceresult == nullptr) 
    {
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
    else if (raytraceresult.typeOfHit != RayTraceResult::Type::BLOCK) 
    {
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
    else 
    {
        BlockPos blockpos = raytraceresult.getBlockPos();
        if (!worldIn->isBlockModifiable(playerIn, blockpos)) 
        {
            return ActionResult(EnumActionResult::FAIL, itemstack);
        }
        else if (flag) 
        {
            if (!playerIn->canPlayerEdit(blockpos.offset(raytraceresult.sideHit), raytraceresult.sideHit, itemstack)) 
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }
            else 
            {
                IBlockState* iblockstate = worldIn->getBlockState(blockpos);
                Material material = iblockstate->getMaterial();
                if (material == Material::WATER && iblockstate->getValue(BlockLiquid::LEVEL) == 0) 
                {
                    worldIn->setBlockState(blockpos, Blocks.AIR.getDefaultState(), 11);
                    playerIn->addStat(StatList::getObjectUseStats(this));
                    playerIn->playSound(SoundEvents.ITEM_BUCKET_FILL, 1.0F, 1.0F);
                    return ActionResult(EnumActionResult::SUCCESS, fillBucket(itemstack, playerIn, Items::WATER_BUCKET));
                }
                else if (material == Material::LAVA && (Integer)iblockstate->getValue(BlockLiquid.LEVEL) == 0) 
                {
                    playerIn->playSound(SoundEvents.ITEM_BUCKET_FILL_LAVA, 1.0F, 1.0F);
                    worldIn->setBlockState(blockpos, Blocks.AIR.getDefaultState(), 11);
                    playerIn->addStat(StatList::getObjectUseStats(this));
                    return ActionResult(EnumActionResult::SUCCESS, fillBucket(itemstack, playerIn, Items::LAVA_BUCKET));
                }
                else 
                {
                    return ActionResult(EnumActionResult::FAIL, itemstack);
                }
            }
        }
        else 
        {
            bool flag1 = worldIn->getBlockState(blockpos).getBlock().isReplaceable(worldIn, blockpos);
            BlockPos blockpos1 = flag1 && raytraceresult.sideHit == EnumFacing::UP ? blockpos : blockpos.offset(raytraceresult.sideHit);
            if (!playerIn->canPlayerEdit(blockpos1, raytraceresult.sideHit, itemstack)) 
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }
            else if (tryPlaceContainedLiquid(playerIn, worldIn, blockpos1)) 
            {
                if (Util::instanceof<EntityPlayerMP>(playerIn)) 
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP*)playerIn, blockpos1, itemstack);
                }

                playerIn->addStat(StatList::getObjectUseStats(this));
                return !playerIn->capabilities.isCreativeMode ? ActionResult(EnumActionResult::SUCCESS, ItemStack(Items::BUCKET)) : ActionResult(EnumActionResult::SUCCESS, itemstack);
            }
            else 
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }
        }
    }
}

bool ItemBucket::tryPlaceContainedLiquid(EntityPlayer *player, World *worldIn, BlockPos posIn)
{
    if (containedBlock == Blocks::AIR) 
    {
        return false;
    }
    else 
    {
        IBlockState* iblockstate = worldIn->getBlockState(posIn);
        Material material = iblockstate->getMaterial();
        bool flag = !material.isSolid();
        bool flag1 = iblockstate->getBlock().isReplaceable(worldIn, posIn);
        if (!worldIn->isAirBlock(posIn) && !flag && !flag1) 
        {
            return false;
        }
        else 
        {
            if (worldIn->provider.doesWaterVaporize() && containedBlock == Blocks::FLOWING_WATER) 
            {
                auto l = posIn.getx();
                auto i = posIn.gety();
                auto j = posIn.getz();
                worldIn->playSound(player, posIn, SoundEvents::BLOCK_FIRE_EXTINGUISH, SoundCategory::BLOCKS, 0.5F, 2.6F + (MathHelper::nextFloat(worldIn->rand) - MathHelper::nextFloat(worldIn->rand)) * 0.8F);

                for(int k = 0; k < 8; ++k) 
                {
                    worldIn->spawnParticle(EnumParticleTypes::SMOKE_LARGE, (double)l + MathHelper::random(), (double)i + MathHelper::random(), (double)j +MathHelper::random(), 0.0, 0.0, 0.0);
                }
            }
            else 
            {
                if (!worldIn->isRemote && (flag || flag1) && !material.isLiquid()) 
                {
                    worldIn->destroyBlock(posIn, true);
                }

                SoundEvent soundevent = containedBlock == Blocks::FLOWING_LAVA ? SoundEvents::ITEM_BUCKET_EMPTY_LAVA : SoundEvents::ITEM_BUCKET_EMPTY;
                worldIn->playSound(player, posIn, soundevent, SoundCategory::BLOCKS, 1.0F, 1.0F);
                worldIn->setBlockState(posIn, containedBlock->getDefaultState(), 11);
            }

            return true;
        }
    }
}

ItemStack ItemBucket::fillBucket(ItemStack emptyBuckets, EntityPlayer *player, Item *fullBucket)
{
    if (player->capabilities.isCreativeMode) 
    {
        return emptyBuckets;
    }
    else 
    {
        emptyBuckets.shrink(1);
        if (emptyBuckets.isEmpty()) 
        {
            return ItemStack(fullBucket);
        }
        else 
        {
            if (!player->inventory.addItemStackToInventory(new ItemStack(fullBucket))) 
            {
                player->dropItem(ItemStack(fullBucket), false);
            }

            return emptyBuckets;
        }
    }
}

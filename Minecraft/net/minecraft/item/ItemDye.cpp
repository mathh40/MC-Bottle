#include "ItemDye.h"

#include "../block/IGrowable.h"
#include "../world/gen/ChunkGeneratorFlat.h"
#include "EnumDyeColor.h"
#include "EnumFacing.h"
#include "EnumParticleTypes.h"
#include "ItemStack.h"
#include "Util.h"

ItemDye::ItemDye()
{
    setHasSubtypes(true);
    setMaxDamage(0);
    setCreativeTab(CreativeTabs::MATERIALS);
}

std::string ItemDye::getTranslationKey(ItemStack stack) const
{
    int32_t i = stack.getMetadata();
    return Item::getTranslationKey() + "." + EnumDyeColor::byDyeDamage(i)->getTranslationKey();
}

EnumActionResult ItemDye::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (!player->canPlayerEdit(pos.offset(facing), facing, itemstack))
    {
        return EnumActionResult::FAIL;
    }
    else
    {
        EnumDyeColor enumdyecolor = EnumDyeColor::byDyeDamage(itemstack.getMetadata());
        if (enumdyecolor == EnumDyeColor::WHITE)
        {
            if (applyBonemeal(itemstack, worldIn, pos))
            {
                if (!worldIn->isRemote)
                {
                    worldIn->playEvent(2005, pos, 0);
                }

                return EnumActionResult::SUCCESS;
            }
        }
        else if (enumdyecolor == EnumDyeColor::BROWN)
        {
            IBlockState *iblockstate = worldIn->getBlockState(pos);
            Block block              = iblockstate->getBlock();
            if (block == Blocks::LOG && iblockstate->getValue(BlockOldLog::VARIANT) == BlockPlanks::EnumType::JUNGLE)
            {
                if (facing == EnumFacing::DOWN || facing == EnumFacing::UP)
                {
                    return EnumActionResult::FAIL;
                }

                pos = pos.offset(facing);
                if (worldIn->isAirBlock(pos))
                {
                    IBlockState *iblockstate1 =
                        Blocks::COCOA::getStateForPlacement(worldIn, pos, facing, hitX, hitY, hitZ, 0, player);
                    worldIn->setBlockState(pos, iblockstate1, 10);
                    if (!player->capabilities.isCreativeMode)
                    {
                        itemstack.shrink(1);
                    }

                    return EnumActionResult::SUCCESS;
                }
            }

            return EnumActionResult::FAIL;
        }

        return EnumActionResult::PASS;
    }
}

bool ItemDye::applyBonemeal(ItemStack stack, World *worldIn, BlockPos target)
{
    IBlockState *iblockstate = worldIn->getBlockState(target);
    if (iblockstate->getBlock() instanceof IGrowable)
    {
        IGrowable *igrowable = (IGrowable *)iblockstate->getBlock();
        if (igrowable->canGrow(worldIn, target, iblockstate, worldIn->isRemote))
        {
            if (!worldIn->isRemote)
            {
                if (igrowable->canUseBonemeal(worldIn, worldIn->rand, target, iblockstate))
                {
                    igrowable->grow(worldIn, worldIn->rand, target, iblockstate);
                }

                stack.shrink(1);
            }

            return true;
        }
    }

    return false;
}

void ItemDye::spawnBonemealParticles(World *worldIn, BlockPos pos, int32_t amount)
{
    if (amount == 0)
    {
        amount = 15;
    }

    IBlockState *iblockstate = worldIn->getBlockState(pos);
    if (iblockstate->getMaterial() != Material::AIR)
    {
        for (int32_t i = 0; i < amount; ++i)
        {
            double d0 = MathHelper::nextGaussian(itemRand) * 0.02;
            double d1 = MathHelper::nextGaussian(itemRand) * 0.02;
            double d2 = MathHelper::nextGaussian(itemRand) * 0.02;
            worldIn->spawnParticle(EnumParticleTypes::VILLAGER_HAPPY,
                                   (double)((float)pos.getx() + MathHelper::nextFloat(itemRand)),
                                   (double)pos.gety() + (double)MathHelper::nextFloat(itemRand) *
                                                            iblockstate->getBoundingBox(worldIn, pos).maxY,
                                   (double)((float)pos.getz() + MathHelper::nextFloat(itemRand)), d0, d1, d2);
        }
    }
}

bool ItemDye::itemInteractionForEntity(ItemStack stack, EntityPlayer *playerIn, EntityLivingBase *target, EnumHand hand)
{
    if (Util:: instanceof <EntitySheep>(target))
    {
        EntitySheep *entitysheep  = (EntitySheep *)target;
        EnumDyeColor enumdyecolor = EnumDyeColor::byDyeDamage(stack.getMetadata());
        if (!entitysheep->getSheared() && entitysheep->getFleeceColor() != enumdyecolor)
        {
            entitysheep->setFleeceColor(enumdyecolor);
            stack.shrink(1);
        }

        return true;
    }
    else
    {
        return false;
    }
}

void ItemDye::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (isInCreativeTab(tab))
    {
        for (int32_t i = 0; i < 16; ++i)
        {
            items.push_back(ItemStack(this, 1, i));
        }
    }
}

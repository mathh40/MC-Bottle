#include "ItemBlock.h"

#include "../block/SoundType.h"
#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"

ItemBlock::ItemBlock(Block *block) : block(block)
{
}

EnumActionResult ItemBlock::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                      EnumFacing facing, float hitX, float hitY, float hitZ)
{
    auto iblockstate = worldIn->getBlockState(pos);
    auto block       = iblockstate.getBlock();
    if (!block.isReplaceable(worldIn, pos))
    {
        pos = pos.offset(facing);
    }

    ItemStack itemstack = player->getHeldItem(hand);
    if (!itemstack.isEmpty() && player->canPlayerEdit(pos, facing, itemstack) &&
        worldIn->mayPlace(block, pos, false, facing, nullptr))
    {
        auto i            = getMetadata(itemstack.getMetadata());
        auto iblockstate1 = block.getStateForPlacement(worldIn, pos, facing, hitX, hitY, hitZ, i, player);
        if (worldIn->setBlockState(pos, iblockstate1, 11))
        {
            iblockstate1 = worldIn->getBlockState(pos);
            if (iblockstate1.getBlock() == block)
            {
                setTileEntityNBT(worldIn, player, pos, itemstack);
                block.onBlockPlacedBy(worldIn, pos, iblockstate1, player, itemstack);
                if (Util:: instanceof <EntityPlayerMP>(player))
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos, itemstack);
                }
            }

            SoundType soundtype = block->getSoundType();
            worldIn->playSound(player, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS,
                               (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
            itemstack.shrink(1);
        }

        return EnumActionResult::SUCCESS;
    }
    else
    {
        return EnumActionResult::FAIL;
    }
}

bool ItemBlock::setTileEntityNBT(World *worldIn, EntityPlayer *player, BlockPos pos, ItemStack stackIn)
{
    auto minecraftserver = worldIn->getMinecraftServer();
    if (minecraftserver == nullptr)
    {
        return false;
    }
    else
    {
        auto nbttagcompound = stackIn.getSubCompound("BlockEntityTag");
        if (nbttagcompound != nullptr)
        {
            auto tileentity = worldIn->getTileEntity(pos);
            if (tileentity != nullptr)
            {
                if (!worldIn->isRemote && tileentity.onlyOpsCanSetNbt() &&
                    (player == nullptr || !player->canUseCommandBlock()))
                {
                    return false;
                }

                auto nbttagcompound1 = tileentity.writeToNBT(new NBTTagCompound());
                auto nbttagcompound2 = nbttagcompound1.copy();
                nbttagcompound1.merge(nbttagcompound);
                nbttagcompound1.setInteger("x", pos.getX());
                nbttagcompound1.setInteger("y", pos.getY());
                nbttagcompound1.setInteger("z", pos.getZ());
                if (!nbttagcompound1.equals(nbttagcompound2))
                {
                    tileentity.readFromNBT(nbttagcompound1);
                    tileentity.markDirty();
                    return true;
                }
            }
        }

        return false;
    }
}

bool ItemBlock::canPlaceBlockOnSide(World *worldIn, BlockPos pos, EnumFacing side, EntityPlayer *player,
                                    ItemStack stack)
{
    Block block = worldIn->getBlockState(pos).getBlock();
    if (block == Blocks::SNOW_LAYER)
    {
        side = EnumFacing::UP;
    }
    else if (!block.isReplaceable(worldIn, pos))
    {
        pos = pos.offset(side);
    }

    return worldIn->mayPlace(block, pos, false, side, nullptr);
}

std::string ItemBlock::getTranslationKey(ItemStack stack) const
{
    return block->getTranslationKey();
}

std::string ItemBlock::getTranslationKey() const
{
    return block->getTranslationKey();
}

CreativeTabs &ItemBlock::getCreativeTab()
{
    return block->getCreativeTab();
}

void ItemBlock::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (isInCreativeTab(tab))
    {
        block->getSubBlocks(tab, items);
    }
}

void ItemBlock::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip, ITooltipFlag *flagIn)
{
    Item::addInformation(stack, worldIn, tooltip, flagIn);
    block->addInformation(stack, worldIn, tooltip, flagIn);
}

Block *ItemBlock::getBlock() const
{
    return block;
}

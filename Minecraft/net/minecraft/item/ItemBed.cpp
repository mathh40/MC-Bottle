#include "ItemBed.h"

#include "../tileentity/TileEntity.h"
#include "../tileentity/TileEntityBed.h"
#include "EnumDyeColor.h"
#include "ItemStack.h"

ItemBed::ItemBed()
{
    setCreativeTab(CreativeTabs::DECORATIONS);
    setMaxDamage(0);
    setHasSubtypes(true);
}

EnumActionResult ItemBed::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (worldIn->isRemote)
    {
        return EnumActionResult::SUCCESS;
    }
    else if (facing != EnumFacing::UP)
    {
        return EnumActionResult::FAIL;
    }
    else
    {
        IBlockState *iblockstate = worldIn->getBlockState(pos);
        Block *block             = iblockstate->getBlock();
        bool flag                = block->isReplaceable(worldIn, pos);
        if (!flag)
        {
            pos = pos.up();
        }

        auto i                = MathHelper::floor((double)(player->rotationYaw * 4.0F / 360.0F) + 0.5) & 3;
        EnumFacing enumfacing = EnumFacing::byHorizontalIndex(i);
        BlockPos blockpos     = pos.offset(enumfacing);
        ItemStack itemstack   = player->getHeldItem(hand);
        if (player->canPlayerEdit(pos, facing, itemstack) && player->canPlayerEdit(blockpos, facing, itemstack))
        {
            IBlockState *iblockstate1 = worldIn->getBlockState(blockpos);
            bool flag1                = iblockstate1->getBlock()->isReplaceable(worldIn, blockpos);
            bool flag2                = flag || worldIn->isAirBlock(pos);
            bool flag3                = flag1 || worldIn->isAirBlock(blockpos);
            if (flag2 && flag3 && worldIn->getBlockState(pos.down()).isTopSolid() &&
                worldIn->getBlockState(blockpos.down()).isTopSolid())
            {
                IBlockState *iblockstate2 = Blocks::BED.getDefaultState()
                                                .withProperty(BlockBed::OCCUPIED, false)
                                                .withProperty(BlockBed::FACING, enumfacing)
                                                .withProperty(BlockBed::PART, BlockBed::EnumPartType::FOOT);
                worldIn->setBlockState(pos, iblockstate2, 10);
                worldIn->setBlockState(blockpos,
                                       iblockstate2.withProperty(BlockBed::PART, BlockBed::EnumPartType::HEAD), 10);
                SoundType soundtype = iblockstate2.getBlock().getSoundType();
                worldIn->playSound(nullptr, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS,
                                   (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
                TileEntity *tileentity = worldIn->getTileEntity(blockpos);
                if (Util:: instanceof <TileEntityBed>(tileentity))
                {
                    ((TileEntityBed *)tileentity)->setItemValues(itemstack);
                }

                TileEntity *tileentity1 = worldIn->getTileEntity(pos);
                if (Util:: instanceof <TileEntityBed>(tileentity1))
                {
                    ((TileEntityBed *)tileentity1)->setItemValues(itemstack);
                }

                worldIn->notifyNeighborsRespectDebug(pos, block, false);
                worldIn->notifyNeighborsRespectDebug(blockpos, iblockstate1->getBlock(), false);
                if (Util:: instanceof <EntityPlayerMP>(player))
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos, itemstack);
                }

                itemstack.shrink(1);
                return EnumActionResult::SUCCESS;
            }
            else
            {
                return EnumActionResult::FAIL;
            }
        }
        else
        {
            return EnumActionResult::FAIL;
        }
    }
}

std::string ItemBed::getTranslationKey(ItemStack stack) const
{
    return Item::getTranslationKey() + "." + EnumDyeColor::byMetadata(stack.getMetadata())->getTranslationKey();
}

void ItemBed::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (isInCreativeTab(tab))
    {
        for (auto i = 0; i < 16; ++i)
        {
            items.push_back(ItemStack(this, 1, i));
        }
    }
}

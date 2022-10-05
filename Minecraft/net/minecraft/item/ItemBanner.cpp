#include "ItemBanner.h"

#include "../tileentity/TileEntity.h"
#include "../tileentity/TileEntityBanner.h"
#include "EnumDyeColor.h"
#include "EnumFacing.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

ItemBanner::ItemBanner() : ItemBlock(Blocks::STANDING_BANNER)
{
    maxStackSize = 16;
    setCreativeTab(CreativeTabs::DECORATIONS);
    setHasSubtypes(true);
    setMaxDamage(0);
}

EnumActionResult ItemBanner::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                       EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState *iblockstate = worldIn->getBlockState(pos);
    bool flag                = iblockstate->getBlock()->isReplaceable(worldIn, pos);
    if (facing != EnumFacing::DOWN && (iblockstate->getMaterial().isSolid() || flag) &&
        (!flag || facing == EnumFacing::UP))
    {
        pos                 = pos.offset(facing);
        ItemStack itemstack = player->getHeldItem(hand);
        if (player->canPlayerEdit(pos, facing, itemstack) && Blocks::STANDING_BANNER.canPlaceBlockAt(worldIn, pos))
        {
            if (worldIn->isRemote)
            {
                return EnumActionResult::SUCCESS;
            }
            else
            {
                pos = flag ? pos.down() : pos;
                if (facing == EnumFacing::UP)
                {
                    int i = MathHelper::floor((double)((player->rotationYaw + 180.0F) * 16.0F / 360.0F) + 0.5) & 15;
                    worldIn->setBlockState(
                        pos, Blocks::STANDING_BANNER.getDefaultState().withProperty(BlockStandingSign::ROTATION, i), 3);
                }
                else
                {
                    worldIn->setBlockState(
                        pos, Blocks::WALL_BANNER.getDefaultState().withProperty(BlockWallSign::FACING, facing), 3);
                }

                TileEntity *tileentity = worldIn->getTileEntity(pos);
                if (Util:: instanceof <TileEntityBanner>(tileentity))
                {
                    ((TileEntityBanner *)tileentity).setItemValues(itemstack, false);
                }

                if (Util:: instanceof <EntityPlayerMP>(player))
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos, itemstack);
                }

                itemstack.shrink(1);
                return EnumActionResult::SUCCESS;
            }
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

std::string ItemBanner::getItemStackDisplayName(ItemStack stack) const
{
    std::string s             = "item.banner.";
    EnumDyeColor enumdyecolor = getBaseColor(stack);
    s                         = s + enumdyecolor.getTranslationKey() + ".name";
    return I18n::translateToLocal(s);
}

void ItemBanner::appendHoverTextFromTileEntityTag(ItemStack stack, std::vector<std::string> &p_185054_1_)
{
    NBTTagCompound *nbttagcompound = stack.getSubCompound("BlockEntityTag");
    if (nbttagcompound != nullptr && nbttagcompound->hasKey("Patterns"))
    {
        NBTTagList *nbttaglist = nbttagcompound->getTagList("Patterns", 10);

        for (int i = 0; i < nbttaglist->tagCount() && i < 6; ++i)
        {
            NBTTagCompound *nbttagcompound1 = nbttaglist->getCompoundTagAt(i);
            EnumDyeColor enumdyecolor       = EnumDyeColor::byDyeDamage(nbttagcompound1->getInteger("Color"));
            auto bannerpattern              = BannerPattern::byHash(nbttagcompound1->getString("Pattern"));
            if (bannerpattern.has_value())
            {
                p_185054_1_.push_back(I18n::translateToLocal("item.banner." + bannerpattern->getFileName() + "." +
                                                             enumdyecolor.getTranslationKey()));
            }
        }
    }
}

void ItemBanner::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                ITooltipFlag *flagIn)
{
    appendHoverTextFromTileEntityTag(stack, tooltip);
}

void ItemBanner::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (isInCreativeTab(tab))
    {
        auto var3 = EnumDyeColor::values();
        int var4  = var3.size();

        for (int var5 = 0; var5 < var4; ++var5)
        {
            EnumDyeColor enumdyecolor = var3[var5];
            items.push_back(makeBanner(enumdyecolor, nullptr));
        }
    }
}

ItemStack ItemBanner::makeBanner(EnumDyeColor color, NBTTagList *patterns)
{
    ItemStack itemstack = ItemStack(Items::BANNER, 1, color.getDyeDamage());
    if (patterns != nullptr && !patterns->isEmpty())
    {
        itemstack.getOrCreateSubCompound("BlockEntityTag").setTag("Patterns", patterns->copy());
    }

    return itemstack;
}

CreativeTabs &ItemBanner::getCreativeTab()
{
    return CreativeTabs::DECORATIONS;
}

EnumDyeColor ItemBanner::getBaseColor(ItemStack stack)
{
    return EnumDyeColor::byDyeDamage(stack.getMetadata() & 15);
}

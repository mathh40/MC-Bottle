#include "TileEntityBanner.h"

#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

TileEntityBanner::TileEntityBanner() : baseColor(EnumDyeColor::BLACK)
{
}

void TileEntityBanner::setItemValues(ItemStack stack, bool p_175112_2_)
{
    patterns                       = nullptr;
    NBTTagCompound *nbttagcompound = stack.getSubCompound("BlockEntityTag");
    if (nbttagcompound != nullptr && nbttagcompound->hasKey("Patterns", 9))
    {
        patterns = nbttagcompound->getTagList("Patterns", 10).copy();
    }

    baseColor = p_175112_2_ ? getColor(stack) : ItemBanner.getBaseColor(stack);
    patternList.clear();
    colorList.clear();
    patternResourceLocation = "";
    patternDataSet          = true;
    name                    = stack.hasDisplayName() ? stack.getDisplayName() : nullptr;
}

std::string TileEntityBanner::getName() const
{
    return hasCustomName() ? name : "banner";
}

bool TileEntityBanner::hasCustomName() const
{
    return !name.empty();
}

ITextComponent *TileEntityBanner::getDisplayName()
{
    return (ITextComponent)(hasCustomName() ? TextComponentString(getName())
                                            : TextComponentTranslation(getName(), new Object[0]));
}

NBTTagCompound *TileEntityBanner::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    compound->setInteger("Base", baseColor.getDyeDamage());
    if (patterns != nullptr)
    {
        compound->setTag("Patterns", patterns);
    }

    if (hasCustomName())
    {
        compound->setString("CustomName", name);
    }

    return compound;
}

void TileEntityBanner::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    if (compound->hasKey("CustomName", 8))
    {
        name = compound->getString("CustomName");
    }

    baseColor = EnumDyeColor.byDyeDamage(compound->getInteger("Base"));
    patterns  = compound->getTagList("Patterns", 10);
    patternList.clear();
    colorList.clear();
    patternResourceLocation.clear();
    patternDataSet = true;
}

SPacketUpdateTileEntity TileEntityBanner::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 6, getUpdateTag());
}

NBTTagCompound *TileEntityBanner::getUpdateTag()
{
    return writeToNBT(new NBTTagCompound());
}

int32_t TileEntityBanner::getPatterns(ItemStack stack)
{
    auto nbttagcompound = stack.getSubCompound("BlockEntityTag");
    return nbttagcompound != nullptr && nbttagcompound.hasKey("Patterns")
               ? nbttagcompound.getTagList("Patterns", 10).tagCount()
               : 0;
}

void TileEntityBanner::removeBannerData(ItemStack stack)
{
    auto nbttagcompound = stack.getSubCompound("BlockEntityTag");
    if (nbttagcompound != nullptr && nbttagcompound.hasKey("Patterns", 9))
    {
        auto nbttaglist = nbttagcompound.getTagList("Patterns", 10);
        if (!nbttaglist.isEmpty())
        {
            nbttaglist.removeTag(nbttaglist.tagCount() - 1);
            if (nbttaglist.isEmpty())
            {
                stack.getTagCompound().removeTag("BlockEntityTag");
                if (stack.getTagCompound().isEmpty())
                {
                    stack.setTagCompound(nullptr);
                }
            }
        }
    }
}

std::vector<BannerPattern> TileEntityBanner::getPatternList()
{
    initializeBannerData();
    return patternList;
}

std::vector<EnumDyeColor> TileEntityBanner::getColorList()
{
    initializeBannerData();
    return colorList;
}

std::string TileEntityBanner::getPatternResourceLocation()
{
    initializeBannerData();
    return patternResourceLocation;
}

ItemStack TileEntityBanner::getItem()
{
    ItemStack itemstack = ItemBanner::makeBanner(baseColor, patterns);
    if (hasCustomName())
    {
        itemstack.setStackDisplayName(getName());
    }

    return itemstack;
}

EnumDyeColor TileEntityBanner::getColor(ItemStack p_190616_0_)
{
    auto nbttagcompound = p_190616_0_.getSubCompound("BlockEntityTag");
    return nbttagcompound != nullptr && nbttagcompound.hasKey("Base")
               ? EnumDyeColor::byDyeDamage(nbttagcompound.getInteger("Base"))
               : EnumDyeColor::BLACK;
}

void TileEntityBanner::initializeBannerData()
{
    if (patternList.empty() || colorList.empty() || patternResourceLocation.empty())
    {
        if (!patternDataSet)
        {
            patternResourceLocation = "";
        }
        else
        {
            patternList.clear();
            colorList.clear();
            patternList.emplace_back(BannerPattern::BASE);
            colorList.emplace_back(baseColor);
            patternResourceLocation = "b" + baseColor.getDyeDamage();
            if (patterns != nullptr)
            {
                for (auto i = 0; i < patterns->tagCount(); ++i)
                {
                    auto nbttagcompound = patterns->getCompoundTagAt(i);
                    auto bannerpattern  = BannerPattern::byHash(nbttagcompound->getString("Pattern"));
                    if (bannerpattern)
                    {
                        patternList.emplace_back(bannerpattern);
                        auto j = nbttagcompound->getInteger("Color");
                        colorList.emplace_back(EnumDyeColor::byDyeDamage(j));
                        patternResourceLocation = patternResourceLocation + bannerpattern.value().getHashname() + j;
                    }
                }
            }
        }
    }
}

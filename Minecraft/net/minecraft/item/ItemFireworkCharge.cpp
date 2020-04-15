#include "ItemFireworkCharge.h"



#include "EnumDyeColor.h"
#include "ItemDye.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

NBTBase * ItemFireworkCharge::getExplosionTag(ItemStack stack, std::string key)
{
    if (stack.hasTagCompound()) 
    {
        NBTTagCompound* nbttagcompound = stack.getTagCompound()->getCompoundTag("Explosion");
        if (nbttagcompound != nullptr) 
        {
            return nbttagcompound->getTag(key);
        }
    }

    return nullptr;
}

void ItemFireworkCharge::addExplosionInfo(NBTTagCompound *nbt, std::vector<std::string> &tooltip)
{
    auto b0 = nbt->getByte("Type");
    if (b0 >= 0 && b0 <= 4) 
    {
        tooltip.push_back(I18n::translateToLocal("item.fireworksCharge.type." + b0));
    }
    else 
    {
        tooltip.push_back(I18n::translateToLocal("item.fireworksCharge.type"));
    }

    auto aint = nbt->getIntArray("Colors");
    if (aint.size() > 0) 
    {
        bool flag = true;
        std::string s;

        for(auto var8 = 0; var8 < aint.size(); ++var8) 
        {
            auto i = aint[var8];
            if (!flag) 
            {
                s = s + ", ";
            }

            flag = false;
            bool flag1 = false;

            for(auto j = 0; j < ItemDye::DYE_COLORS.size(); ++j) 
            {
                if (i == ItemDye::DYE_COLORS[j]) 
                {
                    flag1 = true;
                    s = s + I18n::translateToLocal("item.fireworksCharge." + EnumDyeColor::byDyeDamage(j)->getTranslationKey());
                    break;
                }
            }

            if (!flag1) 
            {
                s = s + I18n::translateToLocal("item.fireworksCharge.customColor");
            }
        }

        tooltip.push_back(s);
    }

    auto aint1 = nbt->getIntArray("FadeColors");
    bool flag2;
    if (!aint1.empty()) 
    {
        flag2 = true;
        auto s1 = I18n::translateToLocal("item.fireworksCharge.fadeTo") + " ";

        for(auto i = 0; i < aint1.size(); ++i) 
        {
            int l = aint1[i];
            if (!flag2) 
            {
                s1 = s1 + ", ";
            }

            flag2 = false;
            bool flag5 = false;

            for(int k = 0; k < 16; ++k) 
            {
                if (l == ItemDye::DYE_COLORS[k]) 
                {
                    flag5 = true;
                    s1 = s1 + I18n::translateToLocal("item.fireworksCharge." + EnumDyeColor::byDyeDamage(k)->getTranslationKey());
                    break;
                }
            }

            if (!flag5) 
            {
                s1 = s1 + I18n::translateToLocal("item.fireworksCharge.customColor");
            }
        }

        tooltip.push_back(s1);
    }

    flag2 = nbt->getBoolean("Trail");
    if (flag2) 
    {
        tooltip.push_back(I18n::translateToLocal("item.fireworksCharge.trail"));
    }

    const bool flag4 = nbt->getBoolean("Flicker");
    if (flag4) 
    {
        tooltip.push_back(I18n::translateToLocal("item.fireworksCharge.flicker"));
    }
}

void ItemFireworkCharge::addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn)
{
    if (stack.hasTagCompound()) 
    {
        NBTTagCompound* nbttagcompound = stack.getTagCompound()->getCompoundTag("Explosion");
        if (nbttagcompound != nullptr) 
        {
            addExplosionInfo(nbttagcompound, tooltip);
        }
    }
}

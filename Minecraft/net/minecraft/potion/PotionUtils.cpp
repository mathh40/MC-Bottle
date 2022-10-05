#include "PotionUtils.h"

#include "PotionEffect.h"
#include "text/TextFormatting.h"
#include "text/translation/I18n.h"

std::vector<> PotionUtils::getEffectsFromStack(ItemStack stack)
{
    return getEffectsFromTag(stack.getTagCompound());
}

std::vector<> PotionUtils::mergeEffects(PotionType potionIn, std::vector<> effects)
{
    std::vector<> list;
    list.addAll(potionIn.getEffects());
    list.addAll(effects);
    return list;
}

std::vector<> PotionUtils::getEffectsFromTag(NBTTagCompound *tag)
{
    std::vector<> list;
    list.addAll(getPotionTypeFromNBT(tag).getEffects());
    addCustomPotionEffectToList(tag, list);
    return list;
}

std::vector<> PotionUtils::getFullEffectsFromItem(ItemStack itemIn)
{
    return getFullEffectsFromTag(itemIn.getTagCompound());
}

std::vector<> PotionUtils::getFullEffectsFromTag(NBTTagCompound *tag)
{
    std::vector<> list;
    addCustomPotionEffectToList(tag, list);
    return list;
}

void PotionUtils::addCustomPotionEffectToList(NBTTagCompound *tag, std::vector<> effectList)
{
    if (tag != nullptr && tag->hasKey("CustomPotionEffects", 9))
    {
        NBTTagList *nbttaglist = tag->getTagList("CustomPotionEffects", 10);

        for (int i = 0; i < nbttaglist->tagCount(); ++i)
        {
            NBTTagCompound *nbttagcompound = nbttaglist->getCompoundTagAt(i);
            PotionEffect potioneffect      = PotionEffect::readCustomPotionEffectFromNBT(nbttagcompound);
            if (potioneffect != nullptr)
            {
                effectList.add(potioneffect);
            }
        }
    }
}

int32_t PotionUtils::getColor(ItemStack p_190932_0_)
{
    NBTTagCompound *nbttagcompound = p_190932_0_.getTagCompound();
    if (nbttagcompound != nullptr && nbttagcompound->hasKey("CustomPotionColor", 99))
    {
        return nbttagcompound->getInteger("CustomPotionColor");
    }
    else
    {
        return getPotionFromItem(p_190932_0_) == PotionTypes::EMPTY
                   ? 16253176
                   : getPotionColorFromEffectList(getEffectsFromStack(p_190932_0_));
    }
}

int32_t PotionUtils::getPotionColor(PotionType potionIn)
{
    return potionIn == PotionTypes::EMPTY ? 16253176 : getPotionColorFromEffectList(potionIn.getEffects());
}

int32_t PotionUtils::getPotionColorFromEffectList(std::vector<> effects)
{
    int32_t i = 3694022;
    if (effects.isEmpty())
    {
        return 3694022;
    }
    else
    {
        float f   = 0.0F;
        float f1  = 0.0F;
        float f2  = 0.0F;
        int32_t j = 0;

        for (auto potioneffect : effects)
        {
            if (potioneffect.doesShowParticles())
            {
                int k = potioneffect.getPotion().getLiquidColor();
                int l = potioneffect.getAmplifier() + 1;
                f += (float)(l * (k >> 16 & 255)) / 255.0F;
                f1 += (float)(l * (k >> 8 & 255)) / 255.0F;
                f2 += (float)(l * (k >> 0 & 255)) / 255.0F;
                j += l;
            }
        }

        if (j == 0)
        {
            return 0;
        }
        else
        {
            f  = f / (float)j * 255.0F;
            f1 = f1 / (float)j * 255.0F;
            f2 = f2 / (float)j * 255.0F;
            return (int32_t)f << 16 | (int32_t)f1 << 8 | (int32_t)f2;
        }
    }
}

PotionType PotionUtils::getPotionFromItem(ItemStack itemIn)
{
    return getPotionTypeFromNBT(itemIn.getTagCompound());
}

PotionType PotionUtils::getPotionTypeFromNBT(NBTTagCompound *tag)
{
    return tag == nullptr ? PotionTypes::EMPTY : PotionType::getPotionTypeForName(tag->getString("Potion"));
}

ItemStack PotionUtils::addPotionToItemStack(ItemStack itemIn, PotionType potionIn)
{
    ResourceLocation resourcelocation = (ResourceLocation)PotionType::REGISTRY.getNameForObject(potionIn);
    NBTTagCompound *nbttagcompound;
    if (potionIn == PotionTypes::EMPTY)
    {
        if (itemIn->hasTagCompound())
        {
            nbttagcompound = itemIn->getTagCompound();
            nbttagcompound->removeTag("Potion");
            if (nbttagcompound->isEmpty())
            {
                itemIn->setTagCompound((NBTTagCompound) nullptr);
            }
        }
    }
    else
    {
        nbttagcompound = itemIn->hasTagCompound() ? itemIn->getTagCompound() : NBTTagCompound();
        nbttagcompound->setString("Potion", resourcelocation.toString());
        itemIn->setTagCompound(nbttagcompound);
    }

    return itemIn;
}

ItemStack PotionUtils::appendEffects(ItemStack itemIn, std::vector<> effects)
{
    if (effects.isEmpty())
    {
        return itemIn;
    }
    else
    {
        NBTTagCompound *nbttagcompound = MoreObjects::firstNonNull(itemIn->getTagCompound(), new NBTTagCompound());
        NBTTagList *nbttaglist         = nbttagcompound->getTagList("CustomPotionEffects", 9);
        Iterator var4                  = effects.iterator();

        while (var4.hasNext())
        {
            PotionEffect potioneffect = (PotionEffect)var4.next();
            nbttaglist->appendTag(potioneffect->writeCustomPotionEffectToNBT(new NBTTagCompound()));
        }

        nbttagcompound->setTag("CustomPotionEffects", nbttaglist);
        itemIn->setTagCompound(nbttagcompound);
        return itemIn;
    }
}

void PotionUtils::addPotionTooltip(ItemStack itemIn, std::vector<> lores, float durationFactor)
{
    std::vector<> list  = getEffectsFromStack(itemIn);
    std::vector<> list1 = Lists.newArrayList();
    Iterator var14;
    if (list.isEmpty())
    {
        String s = I18n.translateToLocal("effect.none").trim();
        lores.add(TextFormatting.GRAY + s);
    }
    else
    {
        var14 = list.iterator();

        while (var14.hasNext())
        {
            PotionEffect potioneffect = (PotionEffect)var14.next();
            String s1                 = I18n.translateToLocal(potioneffect.getEffectName()).trim();
            Potion potion             = potioneffect.getPotion();
            Map map                   = potion.getAttributeModifierMap();
            if (!map.isEmpty())
            {
                Iterator var10 = map.entrySet().iterator();

                while (var10.hasNext())
                {
                    Entry entry                          = (Entry)var10.next();
                    AttributeModifier attributemodifier  = (AttributeModifier)entry.getValue();
                    AttributeModifier attributemodifier1 = new AttributeModifier(
                        attributemodifier.getName(),
                        potion.getAttributeModifierAmount(potioneffect.getAmplifier(), attributemodifier),
                        attributemodifier.getOperation());
                    list1.add(new Tuple(((IAttribute)entry.getKey()).getName(), attributemodifier1));
                }
            }

            if (potioneffect.getAmplifier() > 0)
            {
                s1 = s1 + " " + I18n.translateToLocal("potion.potency." + potioneffect.getAmplifier()).trim();
            }

            if (potioneffect.getDuration() > 20)
            {
                s1 = s1 + " (" + Potion.getPotionDurationString(potioneffect, durationFactor) + ")";
            }

            if (potion.isBadEffect())
            {
                lores.add(TextFormatting.RED + s1);
            }
            else
            {
                lores.add(TextFormatting.BLUE + s1);
            }
        }
    }

    if (!list1.isEmpty())
    {
        lores.add("");
        lores.add(TextFormatting.DARK_PURPLE + I18n.translateToLocal("potion.whenDrank"));
        var14 = list1.iterator();

        while (var14.hasNext())
        {
            Tuple tuple                          = (Tuple)var14.next();
            AttributeModifier attributemodifier2 = (AttributeModifier)tuple.getSecond();
            double d0                            = attributemodifier2.getAmount();
            double d1;
            if (attributemodifier2.getOperation() != 1 && attributemodifier2.getOperation() != 2)
            {
                d1 = attributemodifier2.getAmount();
            }
            else
            {
                d1 = attributemodifier2.getAmount() * 100.0D;
            }

            if (d0 > 0.0D)
            {
                lores.add(TextFormatting.BLUE +
                          I18n.translateToLocalFormatted(
                              "attribute.modifier.plus." + attributemodifier2.getOperation(),
                              ItemStack.DECIMALFORMAT.format(d1),
                              I18n.translateToLocal("attribute.name." + (String)tuple.getFirst())));
            }
            else if (d0 < 0.0D)
            {
                d1 *= -1.0D;
                lores.add(TextFormatting.RED +
                          I18n.translateToLocalFormatted(
                              "attribute.modifier.take." + attributemodifier2.getOperation(),
                              ItemStack.DECIMALFORMAT.format(d1),
                              I18n.translateToLocal("attribute.name." + (String)tuple.getFirst())));
            }
        }
    }
}

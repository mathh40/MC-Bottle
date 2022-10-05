#include "EnumFishType.h"

#include "ItemStack.h"
#include "Util.h"

FishType FishType::COD(0, "cod", 2, 0.1F, 5, 0.6F);
FishType FishType::SALMON(1, "salmon", 2, 0.1F, 6, 0.8F);
FishType FishType::CLOWNFISH(2, "clownfish", 1, 0.1F);
FishType FishType::PUFFERFISH(3, "pufferfish", 1, 0.1F);

FishType::FishType(int32_t meta, std::string_view unlocalizedName, int32_t uncookedHeal, float uncookedSaturation,
                   int32_t cookedHeal, float cookedSaturation, bool bcookable)
    : meta(meta), translationKey(unlocalizedName), uncookedHealAmount(uncookedHeal),
      uncookedSaturationModifier(uncookedSaturation), cookedHealAmount(cookedHeal),
      cookedSaturationModifier(cookedSaturation), cookable(bcookable)
{
    META_LOOKUP.emplace(meta, *this);
}

FishType::FishType(int32_t meta, std::string_view unlocalizedName, int32_t uncookedHeal, float uncookedSaturation)
    : FishType(meta, unlocalizedName, uncookedHeal, uncookedSaturation, 0, 0.0f, false)
{
}

int32_t FishType::getMetadata() const
{
    return meta;
}

std::string FishType::getTranslationKey() const
{
    return translationKey;
}

int32_t FishType::getUncookedHealAmount() const
{
    return uncookedHealAmount;
}

float FishType::getUncookedSaturationModifier() const
{
    return uncookedSaturationModifier;
}

int32_t FishType::getCookedHealAmount() const
{
    return cookedHealAmount;
}

float FishType::getCookedSaturationModifier() const
{
    return cookedSaturationModifier;
}

bool FishType::canCook() const
{
    return cookable;
}

FishType FishType::byMetadata(int32_t meta)
{
    FishType itemfishfood$fishtype = META_LOOKUP[meta];
    return itemfishfood$fishtype == nullptr ? COD : itemfishfood$fishtype;
}

FishType FishType::byItemStack(ItemStack stack)
{
    return Util:: instanceof <ItemFishFood>(stack.getItem()) ? byMetadata(stack.getMetadata()) : COD;
}

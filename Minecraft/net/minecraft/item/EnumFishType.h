#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class ItemStack;

class FishType
{
public:
    static FishType COD;
    static FishType SALMON;
    static FishType CLOWNFISH;
    static FishType PUFFERFISH;

    FishType(int32_t meta, std::string_view unlocalizedName, int32_t uncookedHeal, float uncookedSaturation, int32_t cookedHeal, float cookedSaturation,bool bcookable = true);
    FishType(int32_t meta, std::string_view unlocalizedName, int32_t uncookedHeal, float uncookedSaturation);
    int32_t getMetadata() const;
    std::string getTranslationKey() const;
    int32_t getUncookedHealAmount() const;
    float getUncookedSaturationModifier() const;
    int32_t getCookedHealAmount() const;
    float getCookedSaturationModifier() const;
    bool canCook() const;
    static FishType byMetadata(int32_t meta);
    static FishType byItemStack(ItemStack stack);

private:
    static std::unordered_map<int32_t,FishType> META_LOOKUP;
    int32_t meta;
    std::string translationKey;
    int32_t uncookedHealAmount;
    float uncookedSaturationModifier;
    int32_t cookedHealAmount;
    float cookedSaturationModifier;
    bool cookable;
};

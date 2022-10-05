#pragma once
#include "PotionEffect.h"
#include "ResourceLocation.h"
#include "registry/RegistryNamespacedDefaultedByKey.h"

class PotionType
{
  public:
    static inline ResourceLocation EMPTY = ResourceLocation("empty");
    static inline RegistryNamespacedDefaultedByKey<ResourceLocation, PotionType> REGISTRY =
        RegistryNamespacedDefaultedByKey<ResourceLocation, PotionType>(EMPTY);
    static PotionType getPotionTypeForName(std::string_view p_185168_0_);
    PotionType(std::initializer_list<PotionEffect> p_i46739_1_);
    PotionType(std::string_view p_i46740_1_, std::initializer_list<PotionEffect> p_i46740_2_);
    std::string getNamePrefixed(std::string_view p_185174_1_);
    std::vector<PotionEffect> getEffects();
    bool hasInstantEffect();

    static void registerPotionTypes();

  protected:
    static void registerPotionType(std::string_view p_185173_0_, PotionType p_185173_1_);

  private:
    static int32_t nextPotionTypeId;
    std::string baseName;
    std::vector<PotionEffect> effects;
};

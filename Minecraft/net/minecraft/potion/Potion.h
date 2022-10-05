#pragma once
#include "ResourceLocation.h"
#include "registry/RegistryNamespaced.h"

class AbstractAttributeMap;
class AttributeModifier;
class IAttribute;
class PotionEffect;
class Entity;
class EntityLivingBase;

class Potion
{
  public:
    static RegistryNamespaced<ResourceLocation, Potion *> REGISTRY;

    static Potion *getPotionById(int32_t potionID);
    static int32_t getIdFromPotion(Potion *potionIn);
    static Potion *getPotionFromResourceLocation(std::string_view location);
    static std::string getPotionDurationString(const PotionEffect &effect, float durationFactor);

    void performEffect(EntityLivingBase *entityLivingBaseIn, int32_t amplifier);
    void affectEntity(Entity *source, Entity *indirectSource, EntityLivingBase *entityLivingBaseIn, int32_t amplifier,
                      double health);
    bool isReady(int32_t duration, int32_t amplifier);
    bool isInstant();
    Potion &setPotionName(std::string_view nameIn);
    std::string getName() const;
    bool hasStatusIcon() const;
    int32_t getStatusIconIndex() const;
    bool isBadEffect() const;
    int32_t getLiquidColor() const;
    Potion &registerPotionAttributeModifier(IAttribute *attribute, std::string_view uniqueId, double ammount,
                                            int32_t operation);
    std::unordered_map<IAttribute *, AttributeModifier> &getAttributeModifierMap();
    void removeAttributesModifiersFromEntity(EntityLivingBase *entityLivingBaseIn, AbstractAttributeMap attributeMapIn,
                                             int32_t amplifier);
    void applyAttributesModifiersToEntity(EntityLivingBase *entityLivingBaseIn, AbstractAttributeMap attributeMapIn,
                                          int32_t amplifier);
    double getAttributeModifierAmount(int32_t amplifier, AttributeModifier modifier);
    bool isBeneficial() const;
    Potion &setBeneficial();
    static void registerPotions();

  protected:
    Potion(bool isBadEffectIn, int32_t liquidColorIn);
    Potion &setIconIndex(int32_t p_76399_1_, int32_t p_76399_2_);
    Potion &setEffectiveness(double effectivenessIn);

  private:
    std::unordered_map<IAttribute *, AttributeModifier> attributeModifierMap;
    bool bisBadEffect;
    int32_t liquidColor;
    std::string name        = "";
    int32_t statusIconIndex = -1;
    double effectiveness;
    bool beneficial;
};

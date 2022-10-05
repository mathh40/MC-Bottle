#pragma once
#include "Potion.h"

#include <cstdint>

class NBTTagCompound;

class PotionEffect
{
  public:
    PotionEffect(Potion *potionIn);
    PotionEffect(Potion *potionIn, int32_t durationIn);
    PotionEffect(Potion *potionIn, int32_t durationIn, int32_t amplifierIn);
    PotionEffect(Potion *potionIn, int32_t durationIn, int32_t amplifierIn, bool ambientIn, bool showParticlesIn);

    void combine(const PotionEffect &other);
    Potion *getPotion() const;
    int32_t getDuration() const;
    int32_t getAmplifier() const;
    bool getSplashPotion() const;
    bool getAmbient() const;
    bool doesShowParticles() const;
    bool onUpdate(EntityLivingBase *entityIn);
    void performEffect(EntityLivingBase *entityIn) const;
    std::string getEffectName() const;
    std::string toString();
    friend bool operator==(const PotionEffect &a, const PotionEffect &b);
    friend bool operator>(const PotionEffect &a, const PotionEffect &b);
    NBTTagCompound *writeCustomPotionEffectToNBT(NBTTagCompound *nbt) const;
    PotionEffect readCustomPotionEffectFromNBT(NBTTagCompound *nbt);
    void setPotionDurationMax(bool maxDuration);
    bool getIsPotionDurationMax() const;

  private:
    int32_t deincrementDuration();

    static std::shared_ptr<spdlog::logger> LOGGER;
    Potion *potion;
    int32_t duration;
    int32_t amplifier;
    bool isSplashPotion;
    bool isAmbient;
    bool isPotionDurationMax;
    bool showParticles;
};

namespace std
{
template <> class hash<PotionEffect>
{
  public:
    size_t operator()(const PotionEffect &s) const
    {
        std::hash<Potion *> hash_fn;
        size_t i = hash_fn(s.getPotion());
        i        = 31 * i + s.getDuration();
        i        = 31 * i + s.getAmplifier();
        i        = 31 * i + (s.getSplashPotion() ? 1 : 0);
        i        = 31 * i + (s.getAmbient() ? 1 : 0);
        return i;
    }
};
} // namespace std

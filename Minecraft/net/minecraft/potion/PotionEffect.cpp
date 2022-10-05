#include "PotionEffect.h"

#include "spdlog/spdlog-inl.h"

std::shared_ptr<spdlog::logger> PotionEffect::LOGGER = spdlog::get("Minecraft")->clone("PotionEffect");

PotionEffect::PotionEffect(Potion *potionIn) : PotionEffect(potionIn, 0, 0)
{
}

PotionEffect::PotionEffect(Potion *potionIn, int32_t durationIn) : PotionEffect(potionIn, durationIn, 0)
{
}

PotionEffect::PotionEffect(Potion *potionIn, int32_t durationIn, int32_t amplifierIn)
    : PotionEffect(potionIn, durationIn, amplifierIn, false, true)
{
}

PotionEffect::PotionEffect(Potion *potionIn, int32_t durationIn, int32_t amplifierIn, bool ambientIn,
                           bool showParticlesIn)
    : potion(potionIn), duration(durationIn), amplifier(amplifierIn), isAmbient(ambientIn),
      showParticles(showParticlesIn)
{
}

void PotionEffect::combine(const PotionEffect &other)
{
    if (potion != other.potion)
    {
        LOGGER->warn("This method should only be called for matching effects!");
    }

    if (other.amplifier > amplifier)
    {
        amplifier = other.amplifier;
        duration  = other.duration;
    }
    else if (other.amplifier == amplifier && duration < other.duration)
    {
        duration = other.duration;
    }
    else if (!other.isAmbient && isAmbient)
    {
        isAmbient = other.isAmbient;
    }

    showParticles = other.showParticles;
}

Potion *PotionEffect::getPotion() const
{
    return potion;
}

int32_t PotionEffect::getDuration() const
{
    return duration;
}

int32_t PotionEffect::getAmplifier() const
{
    return amplifier;
}

bool PotionEffect::doesShowParticles() const
{
    return showParticles;
}

bool PotionEffect::onUpdate(EntityLivingBase *entityIn)
{
    if (duration > 0)
    {
        if (potion->isReady(duration, amplifier))
        {
            performEffect(entityIn);
        }

        deincrementDuration();
    }

    return duration > 0;
}

void PotionEffect::performEffect(EntityLivingBase *entityIn) const
{
    if (duration > 0)
    {
        potion->performEffect(entityIn, amplifier);
    }
}

std::string PotionEffect::getEffectName() const
{
    return potion->getName();
}

std::string PotionEffect::toString()
{
    std::string s;
    if (amplifier > 0)
    {
        s = getEffectName() + fmt::format(" x {}, Duration: {}", (amplifier + 1), duration);
    }
    else
    {
        s = getEffectName() + fmt::format(", Duration: {}", duration);
    }

    if (isSplashPotion)
    {
        s = s + ", Splash: true";
    }

    if (!showParticles)
    {
        s = s + ", Particles: false";
    }

    return s;
}

NBTTagCompound *PotionEffect::writeCustomPotionEffectToNBT(NBTTagCompound *nbt) const
{
    nbt->setByte("Id", (std::byte)Potion->getIdFromPotion(getPotion()));
    nbt->setByte("Amplifier", (std::byte)getAmplifier());
    nbt->setInteger("Duration", getDuration());
    nbt->setBoolean("Ambient", getAmbient());
    nbt->setBoolean("ShowParticles", doesShowParticles());
    return nbt;
}

PotionEffect PotionEffect::readCustomPotionEffectFromNBT(NBTTagCompound *nbt)
{
    int32_t i   = nbt->getByte("Id");
    auto potion = Potion::getPotionById(i);
    if (potion == nullptr)
    {
        return nullptr;
    }
    else
    {
        int32_t j  = nbt->getByte("Amplifier");
        int32_t k  = nbt->getInteger("Duration");
        bool flag  = nbt->getBoolean("Ambient");
        bool flag1 = true;
        if (nbt->hasKey("ShowParticles", 1))
        {
            flag1 = nbt->getBoolean("ShowParticles");
        }

        return PotionEffect(potion, k, j < 0 ? 0 : j, flag, flag1);
    }
}

void PotionEffect::setPotionDurationMax(bool maxDuration)
{
    isPotionDurationMax = maxDuration;
}

bool PotionEffect::getIsPotionDurationMax() const
{
    return isPotionDurationMax;
}

int32_t PotionEffect::deincrementDuration()
{
    return --duration;
}

bool operator==(const PotionEffect &a, const PotionEffect &b)
{
    return a.duration == b.duration && a.amplifier == b.amplifier && a.isSplashPotion == b.isSplashPotion &&
           a.isAmbient == b.isAmbient && (a.potion) == (b.potion);
}

bool operator>(const PotionEffect &a, const PotionEffect &b)
{
    int i = true;
    return a.getDuration() > 32147 && b.getDuration() > 32147 || a.getAmbient() && b.getAmbient()
               ? ComparisonChain.start()
                     .compare(this.getIsAmbient(), p_compareTo_1_.getIsAmbient())
                     .compare(a.getPotion().getLiquidColor(), b.getPotion().getLiquidColor())
                     .result()
               : ComparisonChain.start()
                     .compare(a.getAmbient(), b.getAmbient())
                     .compare(a.getDuration(), b.getDuration())
                     .compare(a.getPotion().getLiquidColor(), b.getPotion().getLiquidColor())
                     .result();
}

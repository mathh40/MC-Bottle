#include "EnumParticleTypes.h"

#include <algorithm>

EnumParticleTypes EnumParticleTypes::EXPLOSION_NORMAL  = EnumParticleTypes("explode", 0, true);
EnumParticleTypes EnumParticleTypes::EXPLOSION_LARGE   = EnumParticleTypes("largeexplode", 1, true);
EnumParticleTypes EnumParticleTypes::EXPLOSION_HUGE    = EnumParticleTypes("hugeexplosion", 2, true);
EnumParticleTypes EnumParticleTypes::FIREWORKS_SPARK   = EnumParticleTypes("fireworksSpark", 3, false);
EnumParticleTypes EnumParticleTypes::WATER_BUBBLE      = EnumParticleTypes("bubble", 4, false);
EnumParticleTypes EnumParticleTypes::WATER_SPLASH      = EnumParticleTypes("splash", 5, false);
EnumParticleTypes EnumParticleTypes::WATER_WAKE        = EnumParticleTypes("wake", 6, false);
EnumParticleTypes EnumParticleTypes::SUSPENDED         = EnumParticleTypes("suspended", 7, false);
EnumParticleTypes EnumParticleTypes::SUSPENDED_DEPTH   = EnumParticleTypes("depthsuspend", 8, false);
EnumParticleTypes EnumParticleTypes::CRIT              = EnumParticleTypes("crit", 9, false);
EnumParticleTypes EnumParticleTypes::CRIT_MAGIC        = EnumParticleTypes("magicCrit", 10, false);
EnumParticleTypes EnumParticleTypes::SMOKE_NORMAL      = EnumParticleTypes("smoke", 11, false);
EnumParticleTypes EnumParticleTypes::SMOKE_LARGE       = EnumParticleTypes("largesmoke", 12, false);
EnumParticleTypes EnumParticleTypes::SPELL             = EnumParticleTypes("spell", 13, false);
EnumParticleTypes EnumParticleTypes::SPELL_INSTANT     = EnumParticleTypes("instantSpell", 14, false);
EnumParticleTypes EnumParticleTypes::SPELL_MOB         = EnumParticleTypes("mobSpell", 15, false);
EnumParticleTypes EnumParticleTypes::SPELL_MOB_AMBIENT = EnumParticleTypes("mobSpellAmbient", 16, false);
EnumParticleTypes EnumParticleTypes::SPELL_WITCH       = EnumParticleTypes("witchMagic", 17, false);
EnumParticleTypes EnumParticleTypes::DRIP_WATER        = EnumParticleTypes("dripWater", 18, false);
EnumParticleTypes EnumParticleTypes::DRIP_LAVA         = EnumParticleTypes("dripLava", 19, false);
EnumParticleTypes EnumParticleTypes::VILLAGER_ANGRY    = EnumParticleTypes("angryVillager", 20, false);
EnumParticleTypes EnumParticleTypes::VILLAGER_HAPPY    = EnumParticleTypes("happyVillager", 21, false);
EnumParticleTypes EnumParticleTypes::TOWN_AURA         = EnumParticleTypes("townaura", 22, false);
EnumParticleTypes EnumParticleTypes::NOTE              = EnumParticleTypes("note", 23, false);
EnumParticleTypes EnumParticleTypes::PORTAL            = EnumParticleTypes("portal", 24, false);
EnumParticleTypes EnumParticleTypes::ENCHANTMENT_TABLE = EnumParticleTypes("enchantmenttable", 25, false);
EnumParticleTypes EnumParticleTypes::FLAME             = EnumParticleTypes("flame", 26, false);
EnumParticleTypes EnumParticleTypes::LAVA              = EnumParticleTypes("lava", 27, false);
EnumParticleTypes EnumParticleTypes::FOOTSTEP          = EnumParticleTypes("footstep", 28, false);
EnumParticleTypes EnumParticleTypes::CLOUD             = EnumParticleTypes("cloud", 29, false);
EnumParticleTypes EnumParticleTypes::REDSTONE          = EnumParticleTypes("reddust", 30, false);
EnumParticleTypes EnumParticleTypes::SNOWBALL          = EnumParticleTypes("snowballpoof", 31, false);
EnumParticleTypes EnumParticleTypes::SNOW_SHOVEL       = EnumParticleTypes("snowshovel", 32, false);
EnumParticleTypes EnumParticleTypes::SLIME             = EnumParticleTypes("slime", 33, false);
EnumParticleTypes EnumParticleTypes::HEART             = EnumParticleTypes("heart", 34, false);
EnumParticleTypes EnumParticleTypes::BARRIER           = EnumParticleTypes("barrier", 35, false);
EnumParticleTypes EnumParticleTypes::ITEM_CRACK        = EnumParticleTypes("iconcrack", 36, false, 2);
EnumParticleTypes EnumParticleTypes::BLOCK_CRACK       = EnumParticleTypes("blockcrack", 37, false, 1);
EnumParticleTypes EnumParticleTypes::BLOCK_DUST        = EnumParticleTypes("blockdust", 38, false, 1);
EnumParticleTypes EnumParticleTypes::WATER_DROP        = EnumParticleTypes("droplet", 39, false);
EnumParticleTypes EnumParticleTypes::ITEM_TAKE         = EnumParticleTypes("take", 40, false);
EnumParticleTypes EnumParticleTypes::MOB_APPEARANCE    = EnumParticleTypes("mobappearance", 41, true);
EnumParticleTypes EnumParticleTypes::DRAGON_BREATH     = EnumParticleTypes("dragonbreath", 42, false);
EnumParticleTypes EnumParticleTypes::END_ROD           = EnumParticleTypes("endRod", 43, false);
EnumParticleTypes EnumParticleTypes::DAMAGE_INDICATOR  = EnumParticleTypes("damageIndicator", 44, true);
EnumParticleTypes EnumParticleTypes::SWEEP_ATTACK      = EnumParticleTypes("sweepAttack", 45, true);
EnumParticleTypes EnumParticleTypes::FALLING_DUST      = EnumParticleTypes("fallingdust", 46, false, 1);
EnumParticleTypes EnumParticleTypes::TOTEM             = EnumParticleTypes("totem", 47, false);
EnumParticleTypes EnumParticleTypes::SPIT              = EnumParticleTypes("spit", 48, true);

EnumParticleTypes::EnumParticleTypes(std::string particleNameIn, int particleIDIn, bool shouldIgnoreRangeIn,
                                     int argumentCountIn)
    : particleName(particleNameIn), particleID(particleIDIn), shouldIgnoreRange(shouldIgnoreRangeIn),
      argumentCount(argumentCountIn)
{
}

EnumParticleTypes::EnumParticleTypes(std::string particleNameIn, int particleIDIn, bool shouldIgnoreRangeIn)
    : EnumParticleTypes(particleNameIn, particleIDIn, shouldIgnoreRangeIn, 0)
{
}

std::vector<int> EnumParticleTypes::getParticleNames()
{
    std::vector<int> keys;

    std::transform(PARTICLES.begin(), PARTICLES.end(), std::back_inserter(keys),
                   [](const std::unordered_map<int, EnumParticleTypes>::value_type &pair) { return pair.first; });

    return keys;
}

std::string EnumParticleTypes::getParticleName() const
{
    return particleName;
}

int EnumParticleTypes::getParticleID() const
{
    return particleID;
}

int EnumParticleTypes::getArgumentCount() const
{
    return argumentCount;
}

bool EnumParticleTypes::getShouldIgnoreRange() const
{
    return shouldIgnoreRange;
}

std::optional<EnumParticleTypes> EnumParticleTypes::getParticleFromId(int particleId)
{
    auto ite = PARTICLES.find(particleId);
    if (ite != PARTICLES.end())
    {
        return ite->second;
    }

    return {};
}

std::optional<EnumParticleTypes> EnumParticleTypes::getByName(std::string nameIn)
{
    auto ite = BY_NAME.find(nameIn);
    if (ite != BY_NAME.end())
    {
        return ite->second;
    }

    return {};
}

void initParticle()
{
    std::vector<EnumParticleTypes> particles = {EnumParticleTypes::EXPLOSION_NORMAL,
                                                EnumParticleTypes::EXPLOSION_LARGE,
                                                EnumParticleTypes::EXPLOSION_HUGE,
                                                EnumParticleTypes::FIREWORKS_SPARK,
                                                EnumParticleTypes::WATER_BUBBLE,
                                                EnumParticleTypes::WATER_SPLASH,
                                                EnumParticleTypes::WATER_WAKE,
                                                EnumParticleTypes::SUSPENDED,
                                                EnumParticleTypes::SUSPENDED_DEPTH,
                                                EnumParticleTypes::CRIT,
                                                EnumParticleTypes::CRIT_MAGIC,
                                                EnumParticleTypes::SMOKE_NORMAL,
                                                EnumParticleTypes::SMOKE_LARGE,
                                                EnumParticleTypes::SPELL,
                                                EnumParticleTypes::SPELL_INSTANT,
                                                EnumParticleTypes::SPELL_MOB,
                                                EnumParticleTypes::SPELL_MOB_AMBIENT,
                                                EnumParticleTypes::SPELL_WITCH,
                                                EnumParticleTypes::DRIP_WATER,
                                                EnumParticleTypes::DRIP_LAVA,
                                                EnumParticleTypes::VILLAGER_ANGRY,
                                                EnumParticleTypes::VILLAGER_HAPPY,
                                                EnumParticleTypes::TOWN_AURA,
                                                EnumParticleTypes::NOTE,
                                                EnumParticleTypes::PORTAL,
                                                EnumParticleTypes::ENCHANTMENT_TABLE,
                                                EnumParticleTypes::FLAME,
                                                EnumParticleTypes::LAVA,
                                                EnumParticleTypes::FOOTSTEP,
                                                EnumParticleTypes::CLOUD,
                                                EnumParticleTypes::REDSTONE,
                                                EnumParticleTypes::SNOWBALL,
                                                EnumParticleTypes::SNOW_SHOVEL,
                                                EnumParticleTypes::SLIME,
                                                EnumParticleTypes::HEART,
                                                EnumParticleTypes::BARRIER,
                                                EnumParticleTypes::ITEM_CRACK,
                                                EnumParticleTypes::BLOCK_CRACK,
                                                EnumParticleTypes::BLOCK_DUST,
                                                EnumParticleTypes::WATER_DROP,
                                                EnumParticleTypes::ITEM_TAKE,
                                                EnumParticleTypes::MOB_APPEARANCE,
                                                EnumParticleTypes::DRAGON_BREATH,
                                                EnumParticleTypes::END_ROD,
                                                EnumParticleTypes::DAMAGE_INDICATOR,
                                                EnumParticleTypes::SWEEP_ATTACK,
                                                EnumParticleTypes::FALLING_DUST,
                                                EnumParticleTypes::TOTEM,
                                                EnumParticleTypes::SPIT};

    for (int var2 = 0; var2 < particles.size(); ++var2)
    {
        EnumParticleTypes enumparticletypes = particles[var2];
        EnumParticleTypes::PARTICLES.insert(enumparticletypes.getParticleID(), enumparticletypes);
        EnumParticleTypes::BY_NAME.insert(enumparticletypes.getParticleName(), enumparticletypes);
    }
}

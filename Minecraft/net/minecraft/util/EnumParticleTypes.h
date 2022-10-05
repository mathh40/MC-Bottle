#pragma once
#include <optional>
#include <string>
#include <unordered_map>

class EnumParticleTypes
{
  public:
    static EnumParticleTypes EXPLOSION_NORMAL;
    static EnumParticleTypes EXPLOSION_LARGE;
    static EnumParticleTypes EXPLOSION_HUGE;
    static EnumParticleTypes FIREWORKS_SPARK;
    static EnumParticleTypes WATER_BUBBLE;
    static EnumParticleTypes WATER_SPLASH;
    static EnumParticleTypes WATER_WAKE;
    static EnumParticleTypes SUSPENDED;
    static EnumParticleTypes SUSPENDED_DEPTH;
    static EnumParticleTypes CRIT;
    static EnumParticleTypes CRIT_MAGIC;
    static EnumParticleTypes SMOKE_NORMAL;
    static EnumParticleTypes SMOKE_LARGE;
    static EnumParticleTypes SPELL;
    static EnumParticleTypes SPELL_INSTANT;
    static EnumParticleTypes SPELL_MOB;
    static EnumParticleTypes SPELL_MOB_AMBIENT;
    static EnumParticleTypes SPELL_WITCH;
    static EnumParticleTypes DRIP_WATER;
    static EnumParticleTypes DRIP_LAVA;
    static EnumParticleTypes VILLAGER_ANGRY;
    static EnumParticleTypes VILLAGER_HAPPY;
    static EnumParticleTypes TOWN_AURA;
    static EnumParticleTypes NOTE;
    static EnumParticleTypes PORTAL;
    static EnumParticleTypes ENCHANTMENT_TABLE;
    static EnumParticleTypes FLAME;
    static EnumParticleTypes LAVA;
    static EnumParticleTypes FOOTSTEP;
    static EnumParticleTypes CLOUD;
    static EnumParticleTypes REDSTONE;
    static EnumParticleTypes SNOWBALL;
    static EnumParticleTypes SNOW_SHOVEL;
    static EnumParticleTypes SLIME;
    static EnumParticleTypes HEART;
    static EnumParticleTypes BARRIER;
    static EnumParticleTypes ITEM_CRACK;
    static EnumParticleTypes BLOCK_CRACK;
    static EnumParticleTypes BLOCK_DUST;
    static EnumParticleTypes WATER_DROP;
    static EnumParticleTypes ITEM_TAKE;
    static EnumParticleTypes MOB_APPEARANCE;
    static EnumParticleTypes DRAGON_BREATH;
    static EnumParticleTypes END_ROD;
    static EnumParticleTypes DAMAGE_INDICATOR;
    static EnumParticleTypes SWEEP_ATTACK;
    static EnumParticleTypes FALLING_DUST;
    static EnumParticleTypes TOTEM;
    static EnumParticleTypes SPIT;

    EnumParticleTypes(std::string particleNameIn, int particleIDIn, bool shouldIgnoreRangeIn, int argumentCountIn);
    EnumParticleTypes(std::string particleNameIn, int particleIDIn, bool shouldIgnoreRangeIn);

    static std::vector<int> getParticleNames();

    std::string getParticleName() const;
    int getParticleID() const;
    int getArgumentCount() const;
    bool getShouldIgnoreRange() const;
    static std::optional<EnumParticleTypes> getParticleFromId(int particleId);
    static std::optional<EnumParticleTypes> getByName(std::string nameIn);
    friend void initParticle();

  private:
    std::string particleName;
    int particleID;
    bool shouldIgnoreRange;
    int argumentCount;
    static std::unordered_map<int, EnumParticleTypes> PARTICLES;
    static std::unordered_map<std::string, EnumParticleTypes> BY_NAME;
};

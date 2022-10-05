#pragma once
#include "EnumPushReaction.h"
#include "MapColor.h"

class Material
{
  public:
    virtual ~Material() = default;
    static Material AIR;
    static Material GRASS;
    static Material GROUND;
    static Material WOOD;
    static Material ROCK;
    static Material IRON;
    static Material ANVIL;
    static Material WATER;
    static Material LAVA;
    static Material LEAVES;
    static Material PLANTS;
    static Material VINE;
    static Material SPONGE;
    static Material CLOTH;
    static Material FIRE;
    static Material SAND;
    static Material CIRCUITS;
    static Material CARPET;
    static Material GLASS;
    static Material REDSTONE_LIGHT;
    static Material TNT;
    static Material CORAL;
    static Material ICE;
    static Material PACKED_ICE;
    static Material SNOW;
    static Material CRAFTED_SNOW;
    static Material CACTUS;
    static Material CLAY;
    static Material GOURD;
    static Material DRAGON_EGG;
    static Material PORTAL;
    static Material CAKE;
    static Material WEB;
    static Material PISTON;
    static Material BARRIER;
    static Material STRUCTURE_VOID;

    Material(MapColor color);
    virtual bool isLiquid();
    virtual bool isSolid();
    virtual bool blocksLight();
    virtual bool blocksMovement();
    virtual bool getCanBurn();
    virtual Material &setReplaceable();
    virtual bool isReplaceable() const;
    virtual bool isOpaque();
    virtual bool isToolNotRequired() const;
    virtual EnumPushReaction getPushReaction() const;
    virtual MapColor &getMaterialMapColor();

    friend bool operator==(const Material &lhs, const Material &rhs);
    friend bool operator!=(const Material &lhs, const Material &rhs);

  protected:
    Material &setRequiresTool();
    Material &setBurning();
    Material &setNoPushMobility();
    Material &setImmovableMobility();
    Material &setAdventureModeExempt();

  private:
    bool canBurn;
    bool replaceable;
    bool isTranslucent;
    MapColor materialMapColor;
    bool requiresNoTool = true;
    EnumPushReaction pushReaction;
    bool isAdventureModeExempt;
    static uint32_t count;
    uint32_t id;

    Material &setTranslucent();
};

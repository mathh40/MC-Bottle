#pragma once
#include "MapColor.h"
#include "EnumPushReaction.h"

class Material {
public:
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
    bool isLiquid();
    bool isSolid();
    bool blocksLight();
    bool blocksMovement();
    bool getCanBurn();
    Material &setReplaceable();
    bool isReplaceable() const;
    bool isOpaque();
    bool isToolNotRequired() const;
    EnumPushReaction getPushReaction() const;
    MapColor &getMaterialMapColor();

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

    Material &setTranslucent();
};

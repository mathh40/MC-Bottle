#include "Material.h"
#include "MaterialLiquid.h"
#include "MaterialLogic.h"
#include "MaterialPortal.h"
#include "MaterialTransparent.h"

uint32_t Material::count = 0;

Material Material::AIR = MaterialTransparent(MapColor::AIR);
Material Material::GRASS = Material(MapColor::GRASS);
Material Material::GROUND = Material(MapColor::DIRT);
Material Material::WOOD = Material(MapColor::WOOD).setBurning();
Material Material::ROCK = Material(MapColor::STONE).setRequiresTool();
Material Material::IRON = Material(MapColor::IRON).setRequiresTool();
Material Material::ANVIL = Material(MapColor::IRON).setRequiresTool().setImmovableMobility();
Material Material::WATER = MaterialLiquid(MapColor::WATER).setNoPushMobility();
Material Material::LAVA = MaterialLiquid(MapColor::TNT).setNoPushMobility();
Material Material::LEAVES = Material(MapColor::FOLIAGE).setBurning().setTranslucent().setNoPushMobility();
Material Material::PLANTS = MaterialLogic(MapColor::FOLIAGE).setNoPushMobility();
Material Material::VINE = MaterialLogic(MapColor::FOLIAGE).setBurning().setNoPushMobility().setReplaceable();
Material Material::SPONGE = Material(MapColor::YELLOW);
Material Material::CLOTH = Material(MapColor::CLOTH).setBurning();
Material Material::FIRE = MaterialTransparent(MapColor::AIR).setNoPushMobility();
Material Material::SAND = Material(MapColor::SAND);
Material Material::CIRCUITS = MaterialLogic(MapColor::AIR).setNoPushMobility();
Material Material::CARPET = MaterialLogic(MapColor::CLOTH).setBurning();
Material Material::GLASS = Material(MapColor::AIR).setTranslucent().setAdventureModeExempt();
Material Material::REDSTONE_LIGHT = Material(MapColor::AIR).setAdventureModeExempt();
Material Material::TNT = Material(MapColor::TNT).setBurning().setTranslucent();
Material Material::CORAL = Material(MapColor::FOLIAGE).setNoPushMobility();
Material Material::ICE = Material(MapColor::ICE).setTranslucent().setAdventureModeExempt();
Material Material::PACKED_ICE = Material(MapColor::ICE).setAdventureModeExempt();
Material Material::SNOW = MaterialLogic(MapColor::SNOW).setReplaceable().setTranslucent().setRequiresTool().setNoPushMobility();
Material Material::CRAFTED_SNOW = Material(MapColor::SNOW).setRequiresTool();
Material Material::CACTUS = Material(MapColor::FOLIAGE).setTranslucent().setNoPushMobility();
Material Material::CLAY = Material(MapColor::CLAY);
Material Material::GOURD = Material(MapColor::FOLIAGE).setNoPushMobility();
Material Material::DRAGON_EGG = Material(MapColor::FOLIAGE).setNoPushMobility();
Material Material::PORTAL = MaterialPortal(MapColor::AIR).setImmovableMobility();
Material Material::CAKE = Material(MapColor::AIR).setNoPushMobility();
Material Material::WEB = Material(MapColor::CLOTH).setRequiresTool().setNoPushMobility();
Material Material::PISTON = Material(MapColor::STONE).setImmovableMobility();
Material Material::BARRIER = Material(MapColor::AIR).setRequiresTool().setImmovableMobility();
Material Material::STRUCTURE_VOID = MaterialTransparent(MapColor::AIR);

Material::Material(MapColor color) : pushReaction(EnumPushReaction::NORMAL), materialMapColor(color), id(count++) {

}

bool Material::isLiquid() {
    return false;
}

bool Material::isSolid() {
    return true;
}

bool Material::blocksLight() {
    return true;
}

bool Material::blocksMovement() {
    return true;
}

bool Material::getCanBurn() {
    return canBurn;
}

Material &Material::setReplaceable() {
    replaceable = true;
    return *this;
}

bool Material::isReplaceable() const {
    return replaceable;
}

bool Material::isOpaque() {
    return isTranslucent ? false : blocksMovement();
}

bool Material::isToolNotRequired() const {
    return requiresNoTool;
}

EnumPushReaction Material::getPushReaction() const {
    return pushReaction;
}

MapColor &Material::getMaterialMapColor() {
    return materialMapColor;
}

Material &Material::setRequiresTool() {
    requiresNoTool = false;
    return *this;
}

Material &Material::setBurning() {
    canBurn = true;
    return *this;
}

Material &Material::setNoPushMobility() {
    pushReaction = EnumPushReaction::DESTROY;
    return *this;
}

Material &Material::setImmovableMobility() {
    pushReaction = EnumPushReaction::BLOCK;
    return *this;
}

Material &Material::setAdventureModeExempt() {
    isAdventureModeExempt = true;
    return *this;
}

Material &Material::setTranslucent() {
    isTranslucent = true;
    return *this;
}

bool operator==(const Material &lhs, const Material &rhs) { return lhs.id == rhs.id; }

bool operator!=(const Material &lhs, const Material &rhs) { return !(lhs == rhs); }

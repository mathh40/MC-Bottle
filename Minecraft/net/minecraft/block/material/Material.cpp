#include "Material.h"

Material Material::AIR = MaterialTransparent(MapColor::AIR);
Material Material::GRASS(MapColor::GRASS);
Material Material::GROUND(MapColor::DIRT);
Material Material::WOOD(MapColor::WOOD)
.
setBurning();
Material Material::ROCK(MapColor::STONE)
.
setRequiresTool();
Material Material::IRON(MapColor::IRON)
.
setRequiresTool();
Material Material::ANVIL(MapColor::IRON)
.
setRequiresTool()
.
setImmovableMobility();
Material Material::WATER = MaterialLiquid(MapColor::WATER).setNoPushMobility();
Material Material::LAVA = MaterialLiquid(MapColor::TNT).setNoPushMobility();
Material Material::LEAVES(MapColor::FOLIAGE)
.
setBurning()
.
setTranslucent()
.
setNoPushMobility();
Material Material::PLANTS = MaterialLogic(MapColor::FOLIAGE).setNoPushMobility();
Material Material::VINE = MaterialLogic(MapColor::FOLIAGE).setBurning().setNoPushMobility().setReplaceable();
Material Material::SPONGE(MapColor::YELLOW);
Material Material::CLOTH(MapColor::CLOTH)
.
setBurning();
Material Material::FIRE = MaterialTransparent(MapColor::AIR).setNoPushMobility();
Material Material::SAND(MapColor::SAND);
Material Material::CIRCUITS = MaterialLogic(MapColor::AIR).setNoPushMobility();
Material Material::CARPET = MaterialLogic(MapColor::CLOTH).setBurning();
Material Material::GLASS(MapColor::AIR)
.
setTranslucent()
.
setAdventureModeExempt();
Material Material::REDSTONE_LIGHT(MapColor::AIR)
.
setAdventureModeExempt();
Material Material::TNT(MapColor::TNT)
.
setBurning()
.
setTranslucent();
Material Material::CORAL(MapColor::FOLIAGE)
.
setNoPushMobility();
Material Material::ICE(MapColor::ICE)
.
setTranslucent()
.
setAdventureModeExempt();
Material Material::PACKED_ICE(MapColor::ICE)
.
setAdventureModeExempt();
Material Material::SNOW = MaterialLogic(MapColor::SNOW).setReplaceable().setTranslucent().setRequiresTool().
                                                        setNoPushMobility();
Material Material::CRAFTED_SNOW(MapColor::SNOW)
.
setRequiresTool();
Material Material::CACTUS(MapColor::FOLIAGE)
.
setTranslucent()
.
setNoPushMobility();
Material Material::CLAY(MapColor::CLAY);
Material Material::GOURD(MapColor::FOLIAGE)
.
setNoPushMobility();
Material Material::DRAGON_EGG(MapColor::FOLIAGE)
.
setNoPushMobility();
Material Material::PORTAL = MaterialPortal(MapColor::AIR).setImmovableMobility();
Material Material::CAKE(MapColor::AIR)
.
setNoPushMobility();

Material Material::WEB(MapColor::CLOTH) {
    public
    boolean blocksMovement()
    {
        return false;
    }
}

)
.
setRequiresTool()
.
setNoPushMobility();
Material Material::PISTON(MapColor::STONE)
.
setImmovableMobility();
Material Material::BARRIER(MapColor::AIR)
.
setRequiresTool()
.
setImmovableMobility();
Material Material::STRUCTURE_VOIDTransparent(MapColor::AIR);

Material::Material(MapColor color) :
    pushReaction(EnumPushReaction::NORMAL), materialMapColor(color) {

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

bool operator==(const Material &lhs, const Material &rhs) {

}

bool operator!=(const Material &lhs, const Material &rhs) {
}

#include "MaterialLiquid.h"

MaterialLiquid::MaterialLiquid(MapColor color) : Material(color) {
    Material::setReplaceable();
    setNoPushMobility();
}

bool MaterialLiquid::isLiquid() {
    return true;
}

bool MaterialLiquid::blocksMovement() {
    return false;
}

bool MaterialLiquid::isSolid() {
    return false;
}

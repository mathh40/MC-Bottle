#include"MaterialTransparent.h"

MaterialTransparent::MaterialTransparent(MapColor color) : Material(color) {
  Material::setReplaceable();
}

bool MaterialTransparent::isSolid() {
    return false;
}

bool MaterialTransparent::blocksLight() {
    return false;
}

bool MaterialTransparent::blocksMovement() {
    return false;
}

#include "MaterialPortal.h"

MaterialPortal::MaterialPortal(MapColor color) : Material(color)
{
}

bool MaterialPortal::isSolid()
{
    return false;
}

bool MaterialPortal::blocksLight()
{
    return false;
}

bool MaterialPortal::blocksMovement()
{
    return false;
}

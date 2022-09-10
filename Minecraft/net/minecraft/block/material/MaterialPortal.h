#pragma once
#include "Material.h"

class MaterialPortal :public Material
{
public:
    MaterialPortal(MapColor color);

    bool isSolid() override;
    bool blocksLight() override;
    bool blocksMovement() override;
};

#pragma once
#include "Material.h"

class MaterialLiquid : public Material
{
  public:
    MaterialLiquid(MapColor color);
    bool isLiquid() override;
    bool blocksMovement() override;
    bool isSolid() override;
};

#pragma once
#include "Material.h"

class MaterialTransparent : public Material
{
  public:
    MaterialTransparent(MapColor color);
    bool isSolid() override;
    bool blocksLight() override;
    bool blocksMovement() override;
};

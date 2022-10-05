#pragma once
#include "Material.h"

class MaterialLogic : public Material
{
  public:
    MaterialLogic(MapColor color);
    bool isSolid() override;
    bool blocksLight() override;
    bool blocksMovement() override;

  private:
};

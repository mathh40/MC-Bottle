#pragma once
#include "../../material/Material.h"
class IBlockState;

class BlockMaterialMatcher
{
  public:
    static BlockMaterialMatcher forMaterial(Material materialIn);
    bool operator()(IBlockState *p_apply_1_);

  private:
    BlockMaterialMatcher(Material materialIn);

    Material material;
};

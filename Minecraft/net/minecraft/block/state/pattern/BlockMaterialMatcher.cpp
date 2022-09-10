#include "BlockMaterialMatcher.h"

#include "../IBlockState.h"

BlockMaterialMatcher::BlockMaterialMatcher(Material materialIn) : material(materialIn) {

}

BlockMaterialMatcher BlockMaterialMatcher::forMaterial(Material materialIn) {
    return BlockMaterialMatcher(materialIn);
}

bool BlockMaterialMatcher::operator()(IBlockState* p_apply_1_) {
    return p_apply_1_ != nullptr && p_apply_1_->getMaterial() == material;
}

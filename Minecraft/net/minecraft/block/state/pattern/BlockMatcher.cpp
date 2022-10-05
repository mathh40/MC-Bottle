#include "BlockMatcher.h"

BlockMatcher::BlockMatcher(Block *blockType) : block(blockType)
{
}

BlockMatcher BlockMatcher::forBlock(Block *blockType)
{
    return BlockMatcher(blockType);
}

bool BlockMatcher::operator()(IBlockState *p_apply_1_) const
{
    return p_apply_1_ != nullptr && p_apply_1_->getBlock() == block;
}

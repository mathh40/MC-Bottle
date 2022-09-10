#pragma once
#include "../../Block.h"

class BlockMatcher
{
public:
    static BlockMatcher forBlock(Block* blockType);

    bool operator()(IBlockState* p_apply_1_) const;

  private:
    BlockMatcher(Block* blockType);

    Block* block;
};

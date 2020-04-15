#include "ItemAnvilBlock.h"

ItemAnvilBlock::ItemAnvilBlock(Block *block)
    :ItemMultiTexture(block, block, {"intact", "slightlyDamaged", "veryDamaged"})
{
}

int32_t ItemAnvilBlock::getMetadata(int32_t damage) const
{
    return damage << 2;
}

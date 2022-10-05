#include "ItemPiston.h"

ItemPiston::ItemPiston(Block *block) : ItemBlock(block)
{
}

int32_t ItemPiston::getMetadata(int32_t damage) const
{
    return 7;
}

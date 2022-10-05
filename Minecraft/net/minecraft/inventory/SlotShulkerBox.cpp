#include "SlotShulkerBox.h"

#include "../item/ItemStack.h"
#include "Block.h"
#include "Util.h"

SlotShulkerBox::SlotShulkerBox(IInventory *p_i47265_1_, int32_t slotIndexIn, int32_t xPosition, int32_t yPosition)
    : Slot(p_i47265_1_, slotIndexIn, xPosition, yPosition)
{
}

bool SlotShulkerBox::isItemValid(ItemStack stack)
{
    return !Util:: instanceof <BlockShulkerBox>(Block::getBlockFromItem(stack.getItem()));
}

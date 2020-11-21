#include "IBehaviorDispenseItem.h"

ItemStack DefaultBehaviorDispenseItem::dispense(IBlockSource *source, const ItemStack &stack) {
    return stack;
}

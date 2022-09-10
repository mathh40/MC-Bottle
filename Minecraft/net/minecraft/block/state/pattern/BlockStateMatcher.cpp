#include "BlockStateMatcher.h"

#include "../IBlockState.h"

BlockStateMatcher::BlockStateMatcher(BlockStateContainer blockStateIn) : blockstate(blockStateIn) {

}

BlockStateMatcher BlockStateMatcher::forBlock(Block* blockIn) {
    return BlockStateMatcher(blockIn->getBlockState());
}

bool BlockStateMatcher::operator()(IBlockState* p_apply_1_) {
    if (p_apply_1_ != nullptr && p_apply_1_->getBlock()==(blockstate.getBlock())) {
         if (propertyPredicates.empty()) {
            return true;
         } else {
            Iterator var2 = propertyPredicates.entrySet().iterator();

            Entry entry;
            do {
               if (!var2.hasNext()) {
                  return true;
               }

               entry = (Entry)var2.next();
            } while(matches(p_apply_1_, (IProperty)entry.getKey(), (Predicate)entry.getValue()));

            return false;
         }
      } else {
         return false;
      }
}



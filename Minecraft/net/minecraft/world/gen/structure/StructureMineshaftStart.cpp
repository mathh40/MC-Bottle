#include "StructureMineshaftStart.h"
#include "StructureMineshaftPieces.h"

StructureMineshaftStart::StructureMineshaftStart(World* p_i47149_1_, pcg32& p_i47149_2_, int32_t p_i47149_3_,
	int32_t p_i47149_4_, MapGenMineshaft::Type p_i47149_5_)
   :StructureStart(p_i47149_3_, p_i47149_4_)
{
   mineShaftType = p_i47149_5_;
   StructureMineshaftPieces::Room structuremineshaftpieces$room(0, p_i47149_2_, (p_i47149_3_ << 4) + 2, (p_i47149_4_ << 4) + 2, mineShaftType);
   components.emplace_back(structuremineshaftpieces$room);
   structuremineshaftpieces$room.buildComponent(&structuremineshaftpieces$room, components, p_i47149_2_);
   updateBoundingBox();
   if (p_i47149_5_ == MapGenMineshaft::Type::MESA) 
   {
      auto j = p_i47149_1_->getSeaLevel() - boundingBox.maxY + boundingBox.getYSize() / 2 - -5;
      boundingBox.offset(0, j, 0);

      for(auto& structurecomponent : components)
      {
         structurecomponent.offset(0, j, 0);
      }
   }
   else 
   {
      markAvailableHeight(p_i47149_1_, p_i47149_2_, 10);
   }
}

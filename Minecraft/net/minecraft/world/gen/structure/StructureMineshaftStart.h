#include "MapGenMineshaft.h"
#include "StructureStart.h"

class StructureMineshaftStart : public StructureStart
{
  public:
    StructureMineshaftStart() = default;
    StructureMineshaftStart(World *p_i47149_1_, pcg32 &p_i47149_2_, int32_t p_i47149_3_, int32_t p_i47149_4_,
                            MapGenMineshaft::Type p_i47149_5_);

  private:
    MapGenMineshaft::Type mineShaftType;
};

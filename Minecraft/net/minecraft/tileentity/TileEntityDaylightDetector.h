#include "ITickable.h"
#include "TileEntity.h"

class TileEntityDaylightDetector : public TileEntity, ITickable
{
  public:
    void update() override;

  private:
};

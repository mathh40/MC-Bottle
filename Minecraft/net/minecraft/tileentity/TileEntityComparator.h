#include "TileEntity.h"

class TileEntityComparator : public TileEntity
{
  public:
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    int32_t getOutputSignal() const;
    void setOutputSignal(int32_t outputSignalIn);

  private:
    int32_t outputSignal;
};

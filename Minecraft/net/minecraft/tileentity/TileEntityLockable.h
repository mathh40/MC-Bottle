#include "../world/ILockableContainer.h"
#include "../world/LockCode.h"
#include "TileEntity.h"

class TileEntityLockable : public TileEntity, ILockableContainer
{
  public:
    TileEntityLockable();
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    bool isLocked() override;
    LockCode getLockCode() override;
    void setLockCode(LockCode &code) override;
    ITextComponent *getDisplayName() override;

  private:
    LockCode code;
};

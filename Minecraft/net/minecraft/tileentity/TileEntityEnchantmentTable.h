#include "ITickable.h"
#include "TileEntity.h"

class TileEntityEnchantmentTable : public TileEntity, ITickable, IInteractionObject
{
  public:
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    void update() override;
    std::string getName() const override;
    bool hasCustomName() const override;
    void setCustomName(std::string customNameIn);
    ITextComponent *getDisplayName() override;
    Container createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);
    std::string getGuiID() const override;

    int32_t tickCount;
    float pageFlip;
    float pageFlipPrev;
    float flipT;
    float flipA;
    float bookSpread;
    float bookSpreadPrev;
    float bookRotation;
    float bookRotationPrev;
    float tRot;

  private:
    static pcg32 rand;
    std::string customName;
};

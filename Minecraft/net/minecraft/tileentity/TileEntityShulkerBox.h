#include "ITickable.h"
#include "TileEntityLockableLoot.h"

class DataFixer;

class TileEntityShulkerBox : public TileEntityLockableLoot, ITickable, ISidedInventory
{
  public:
    TileEntityShulkerBox();
    explicit TileEntityShulkerBox(std::optional<EnumDyeColor> colorIn);

    void update() override;
    TileEntityShulkerBox::AnimationStatus getAnimationStatus();
    AxisAlignedBB getBoundingBox(IBlockState *p_190584_1_);
    AxisAlignedBB getBoundingBox(EnumFacing p_190587_1_);
    int32_t getSizeInventory() const;
    int32_t getInventoryStackLimit() const;
    bool receiveClientEvent(int32_t id, int32_t type) override;
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    Container createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);
    std::string getGuiID() const;
    std::string getName() const;
    static void registerFixesShulkerBox(DataFixer fixer);
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void loadFromNbt(NBTTagCompound *compound);
    NBTTagCompound *saveToNbt(NBTTagCompound *compound);
    bool isEmpty();
    std::vector<int32_t> getSlotsForFac(EnumFacing side) const;
    bool canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction);
    bool canExtractItem(int index, ItemStack stack, EnumFacing direction);
    void clear() override;
    bool isCleared() const;
    float getProgress(float p_190585_1_) const;
    EnumDyeColor getColor();
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    bool isDestroyedByCreativePlayer() const;
    void setDestroyedByCreativePlayer(bool p_190579_1_);
    bool shouldDrop();

    enum class AnimationStatus
    {
        CLOSED,
        OPENING,
        OPENED,
        CLOSING
    };

  protected:
    void updateAnimation();
    std::vector<ItemStack> getItems();

  private:
    static int SLOTS[27];
    std::vector<ItemStack> items;
    bool hasBeenCleared;
    int32_t openCount;
    TileEntityShulkerBox::AnimationStatus animationStatus;
    float progress;
    float progressOld;
    std::optional<EnumDyeColor> color;
    bool destroyedByCreativePlayer;
    AxisAlignedBB getTopBoundingBox(EnumFacing p_190588_1_);
    void moveCollidedEntities();
};

#include "ITickable.h"
#include "NBTTagCompound.h"
#include "TileEntityLockable.h"
#include "multivec.h"

#include <optional>
#include <string>
#include <unordered_set>

class TileEntityBeacon : public TileEntityLockable, ITickable, ISidedInventory
{
  public:
    class BeamSegment
    {
      private:
        std::vector<float> colors;
        int32_t height;

      protected:
        void incrementHeight();

      public:
        BeamSegment(std::vector<float> colorsIn);
        std::vector<float> getColors() const;
        int32_t getHeight() const;
    };

    static multivec<Potion> EFFECTS_LIST;

    TileEntityBeacon();
    void update() override;
    void updateBeacon();
    std::vector<BeamSegment> getBeamSegments() const;
    float shouldBeamRender();
    int32_t getLevels() const;
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;
    double getMaxRenderDistanceSquared() const;
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    int32_t getSizeInventory() const;
    bool isEmpty();
    ItemStack getStackInSlot(int32_t index) const;
    ItemStack decrStackSize(int32_t index, int32_t count);
    ItemStack removeStackFromSlot(int32_t index);
    void setInventorySlotContents(int32_t index, ItemStack stack);
    std::string getName() const override;
    bool hasCustomName() const override;
    void setName(std::string name);
    int32_t getInventoryStackLimit() const;
    bool isUsableByPlayer(EntityPlayer *player);
    void openInventory(EntityPlayer *player);
    void closeInventory(EntityPlayer *player);
    bool isItemValidForSlot(int32_t index, ItemStack stack);
    std::string getGuiID() const override;
    Container createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn);
    int32_t getField(int32_t id);
    void setField(int32_t id, int32_t value);
    int32_t getFieldCount() const;
    void clear();
    bool receiveClientEvent(int32_t id, int32_t type);
    std::vector<int32_t> getSlotsForFace(EnumFacing side);
    bool canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction);
    bool canExtractItem(int32_t index, ItemStack stack, EnumFacing direction);

  private:
    void addEffectsToPlayers();
    void updateSegmentColors();
    static std::optional<Potion> isBeaconEffect(int32_t p_184279_0_);

    static std::unordered_set<Potion> VALID_EFFECTS;
    std::vector<BeamSegment> beamSegments;
    int64_t beamRenderCounter;
    float beamRenderScale;
    bool isComplete;
    int32_t levels = -1;
    std::optional<Potion> primaryEffect;
    Potion secondaryEffect;
    std::optional<ItemStack> payment;
    std::string customName;
};

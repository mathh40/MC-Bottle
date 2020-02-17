#include "TileEntityLockableLoot.h"
#include "IHopper.h"
#include "ITickable.h"

class DataFixer;
class IInventory;
class EntityItem;

class TileEntityHopper :public TileEntityLockableLoot ,IHopper, ITickable
{
public:
	TileEntityHopper();
	static void registerFixesHopper(DataFixer fixer);
	void readFromNBT(NBTTagCompound* compound) override;
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	int32_t getSizeInventory() const;
	ItemStack decrStackSize(int32_t index, int32_t count) override;
	void setInventorySlotContents(int32_t index, ItemStack stack) override;
	std::string getName() const;
	int32_t getInventoryStackLimit() const;
	void update() override;
	bool isEmpty();
	bool pullItems(IHopper* hopper);
	bool putDropInInventoryAllSlots(IInventory* source, IInventory* destination, EntityItem* entity);
	ItemStack putStackInInventoryAllSlots(IInventory* source, IInventory* destination, ItemStack stack, std::optional<EnumFacing> direction);
	IInventory* getSourceInventory(IHopper* hopper);
	std::vector<Entity*> getCaptureItems(World* worldIn, double p_184292_1_, double p_184292_3_, double p_184292_5_);
	IInventory* getInventoryAtPosition(World* worldIn, double x, double y, double z);
	double getXPos() override;
	double getYPos() override;
	double getZPos() override;
	std::string getGuiID() const;
	Container createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn);
protected:
	std::vector<ItemStack> getItems() override;
private:

	bool updateHopper();
	bool isInventoryEmpty();
	bool isFull();
	bool transferItemsOut();
	bool isInventoryFull(IInventory* inventoryIn, EnumFacing side);
	bool isInventoryEmpty(IInventory* inventoryIn, EnumFacing side);
	bool pullItemFromSlot(IHopper* hopper, IInventory* inventoryIn, int32_t index, EnumFacing direction);
	bool canInsertItemInSlot(IInventory* inventoryIn, ItemStack stack, int32_t index, EnumFacing side);
	bool canExtractItemFromSlot(IInventory* inventoryIn, ItemStack stack, int32_t index, EnumFacing side);
	ItemStack insertStack(IInventory* source, IInventory* destination, ItemStack stack, int32_t index, EnumFacing direction);
	IInventory* getInventoryForHopperTransfer();
	bool canCombine(ItemStack stack1, ItemStack stack2);
	void setTransferCooldown(int32_t ticks);
	bool isOnTransferCooldown() const;
	bool mayTransfer() const;

	std::vector<ItemStack> inventory;
	int32_t transferCooldown;
	int64_t tickedGameTime;
};

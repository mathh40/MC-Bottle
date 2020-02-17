#include "ITickable.h"
#include "TileEntityLockable.h"

class DataFixer;

class TileEntityBrewingStand :public TileEntityLockable,ITickable,ISidedInventory
{
public:
	TileEntityBrewingStand();
	std::string getName() const override;
	bool hasCustomName() const override;
	static void setName(std::string_view name);
	int32_t getSizeInventory() const;
	bool isEmpty() const;
	void update() override;
	std::array<bool,3> createFilledSlotsArray();
	static void registerFixesBrewingStand(DataFixer fixer);
	void readFromNBT(NBTTagCompound* compound) override;
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	ItemStack getStackInSlot(int32_t index);
	ItemStack decrStackSize(int32_t index, int32_t count);
	ItemStack removeStackFromSlot(int32_t index);
	void setInventorySlotContents(int32_t index, ItemStack stack);
	int32_t getInventoryStackLimit() const;
	bool isUsableByPlayer(EntityPlayer* player);
	void openInventory(EntityPlayer* player);
	void closeInventory(EntityPlayer* player);
	bool isItemValidForSlot(int32_t index, ItemStack stack);
	std::vector<int32_t> getSlotsForFace(EnumFacing side) const;
	bool canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction);
	bool canExtractItem(int32_t index, ItemStack stack, EnumFacing direction);
	std::string getGuiID() const;
	Container createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn);
	int32_t getField(int32_t id) const;
	void setField(int32_t id, int32_t value);
	int32_t getFieldCount() const;
	void clear();
	
private:
	bool canBrew();
	void brewPotions();
	
	static std::array<int32_t,1> SLOTS_FOR_UP;
	static std::array<int32_t,4> SLOTS_FOR_DOWN;
	static std::array<int32_t,4> OUTPUT_SLOTS;	
	std::vector<ItemStack> brewingItemStacks;
	int32_t brewTime;
	std::array<bool,3> filledSlots;
	Item ingredientID;
	std::string customName;
	int32_t fuel;
};

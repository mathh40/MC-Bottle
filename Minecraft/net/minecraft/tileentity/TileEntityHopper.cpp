#include "TileEntityHopper.h"
#include "datafix/DataFixer.h"
#include "EntitySelectors.h"
#include "TileEntityChest.h"

TileEntityHopper::TileEntityHopper()
	:inventory(5, ItemStack::EMPTY),transferCooldown(-1)
{
	
}

void TileEntityHopper::registerFixesHopper(DataFixer fixer)
{
	fixer.registerWalker(FixTypes.BLOCK_ENTITY, new ItemStackDataLists(TileEntityHopper.class, new String[]{"Items"}));
}

void TileEntityHopper::readFromNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::readFromNBT(compound);
	std::vector<ItemStack>(getSizeInventory(),ItemStack::EMPTY).swap(inventory);
	if (!checkLootAndRead(compound)) {
		ItemStackHelper::loadAllItems(compound, inventory);
	}

	if (compound->hasKey("CustomName", 8)) 
	{
		customName = compound->getString("CustomName");
	}

	transferCooldown = compound->getInteger("TransferCooldown");
}

NBTTagCompound* TileEntityHopper::writeToNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::writeToNBT(compound);
	if (!checkLootAndWrite(compound)) 
	{
		ItemStackHelper::saveAllItems(compound, inventory);
	}

	compound->setInteger("TransferCooldown", transferCooldown);
	if (hasCustomName()) 
	{
		compound->setString("CustomName", customName);
	}

	return compound;
}

int32_t TileEntityHopper::getSizeInventory() const
{
	return inventory.size();
}

ItemStack TileEntityHopper::decrStackSize(int32_t index, int32_t count)
{
	fillWithLoot(nullptr);
	ItemStack itemstack = ItemStackHelper::getAndSplit(getItems(), index, count);
	return itemstack;
}

void TileEntityHopper::setInventorySlotContents(int32_t index, ItemStack stack)
{
	fillWithLoot(nullptr);
	getItems()[index] = stack;
	if (stack.getCount() > getInventoryStackLimit()) {
		stack.setCount(getInventoryStackLimit());
	}
}

std::string TileEntityHopper::getName() const
{
	return hasCustomName() ? customName : "container.hopper";
}

int32_t TileEntityHopper::getInventoryStackLimit() const
{
	return 64;
}

void TileEntityHopper::update()
{
	if (world != nullptr && !world->isRemote) 
	{
		--transferCooldown;
		tickedGameTime = world->getTotalWorldTime();
		if (!isOnTransferCooldown()) 
		{
			setTransferCooldown(0);
			updateHopper();
		}
	}
}

bool TileEntityHopper::isEmpty()
{
	return isInventoryEmpty();
}

bool TileEntityHopper::pullItems(IHopper* hopper)
{
	IInventory* iinventory = getSourceInventory(hopper);
	if (iinventory != nullptr) 
	{
		EnumFacing enumfacing = EnumFacing::DOWN;
		if (isInventoryEmpty(iinventory, enumfacing)) 
		{
			return false;
		}

		if (iinventory instanceof ISidedInventory) 
		{
			ISidedInventory isidedinventory = (ISidedInventory)iinventory;
			int[] aint = isidedinventory.getSlotsForFace(enumfacing);
			int[] var5 = aint;
			int var6 = aint.length;

			for(int var7 = 0; var7 < var6; ++var7) {
				int i = var5[var7];
				if (pullItemFromSlot(hopper, iinventory, i, enumfacing)) 
				{
					return true;
				}
			}
		} else {
			int j = iinventory.getSizeInventory();

			for(int k = 0; k < j; ++k) {
				if (pullItemFromSlot(hopper, iinventory, k, enumfacing)) 
				{
					return true;
				}
			}
		}
	}
	else 
	{
		Iterator var9 = getCaptureItems(hopper.getWorld(), hopper.getXPos(), hopper.getYPos(), hopper.getZPos()).iterator();

		while(var9.hasNext()) {
			EntityItem entityitem = (EntityItem)var9.next();
			if (putDropInInventoryAllSlots((IInventory)nullptr, hopper, entityitem)) {
				return true;
			}
		}
	}

	return false;
}

bool TileEntityHopper::updateHopper()
{
	if (world != nullptr && !world->isRemote) 
	{
		if (!isOnTransferCooldown() && BlockHopper.isEnabled(getBlockMetadata())) 
		{
			bool flag = false;
			if (!isInventoryEmpty()) 
			{
				flag = transferItemsOut();
			}

			if (!isFull()) 
			{
				flag = pullItems(this) || flag;
			}

			if (flag) 
			{
				setTransferCooldown(8);
				markDirty();
				return true;
			}
		}

		return false;
	}
	else 
	{
		return false;
	}
}

bool TileEntityHopper::isInventoryEmpty()
{
	bool empty = false;

	for(auto stack : inventory)
	{
		empty = stack.isEmpty();
	}
	return empty;
}

bool TileEntityHopper::isFull()
{
	auto var1 = inventory.begin();

	ItemStack itemstack;
	do 
	{
		if (var1 == inventory.end()) 
		{
			return true;
		}

		itemstack = *(var1++);
	}
	while(!itemstack.isEmpty() && itemstack.getCount() == itemstack.getMaxStackSize());

	return false;
}

bool TileEntityHopper::transferItemsOut()
{
	IInventory* iinventory = getInventoryForHopperTransfer();
	if (iinventory == nullptr) 
	{
		return false;
	}
	else 
	{
		EnumFacing enumfacing = BlockHopper::getFacing(getBlockMetadata()).getOpposite();
		if (isInventoryFull(iinventory, enumfacing)) 
		{
			return false;
		}
		else 
		{
			for(int32_t i = 0; i < getSizeInventory(); ++i) 
			{
				if (!getStackInSlot(i).isEmpty()) 
				{
					ItemStack itemstack = getStackInSlot(i).copy();
					ItemStack itemstack1 = putStackInInventoryAllSlots(iinventory, decrStackSize(i, 1), enumfacing);
					if (itemstack1.isEmpty()) 
					{
						iinventory->markDirty();
						return true;
					}

					setInventorySlotContents(i, itemstack);
				}
			}

			return false;
		}
	}
}

bool TileEntityHopper::isInventoryFull(IInventory* inventoryIn, EnumFacing side)
{
	if (Util::instanceof<ISidedInventory>(inventoryIn)) 
	{
		ISidedInventory* isidedinventory = (ISidedInventory*)inventoryIn;
		int[] aint = isidedinventory.getSlotsForFace(side);
		int[] var12 = aint;
		int var6 = aint.size();

		for(int var7 = 0; var7 < var6; ++var7) 
		{
			int k = var12[var7];
			ItemStack itemstack1 = isidedinventory.getStackInSlot(k);
			if (itemstack1.isEmpty() || itemstack1.getCount() != itemstack1.getMaxStackSize()) 
			{
				return false;
			}
		}
	}
	else 
	{
		int i = inventoryIn.getSizeInventory();

		for(int j = 0; j < i; ++j) 
		{
			ItemStack itemstack = inventoryIn.getStackInSlot(j);
			if (itemstack.isEmpty() || itemstack.getCount() != itemstack.getMaxStackSize()) 
			{
				return false;
			}
		}
	}

	return true;
}

bool TileEntityHopper::isInventoryEmpty(IInventory* inventoryIn, EnumFacing side)
{
	if (Util::instanceof<ISidedInventory>(inventoryIn)) 
	{
		ISidedInventory isidedinventory = (ISidedInventory)inventoryIn;
		int[] aint = isidedinventory.getSlotsForFace(side);
		int[] var4 = aint;
		int var5 = aint.length;

		for(int var6 = 0; var6 < var5; ++var6) {
			int i = var4[var6];
			if (!isidedinventory.getStackInSlot(i).isEmpty()) {
				return false;
			}
		}
	}
	else 
	{
		int j = inventoryIn.getSizeInventory();

		for(int k = 0; k < j; ++k) {
			if (!inventoryIn.getStackInSlot(k).isEmpty()) {
				return false;
			}
		}
	}

	return true;
}

bool TileEntityHopper::pullItemFromSlot(IHopper* hopper, IInventory* inventoryIn, int32_t index, EnumFacing direction)
{
	ItemStack itemstack = inventoryIn.getStackInSlot(index);
	if (!itemstack.isEmpty() && canExtractItemFromSlot(inventoryIn, itemstack, index, direction)) 
	{
		ItemStack itemstack1 = itemstack.copy();
		ItemStack itemstack2 = putStackInInventoryAllSlots(inventoryIn, hopper, inventoryIn.decrStackSize(index, 1), std::nullopt_t);
		if (itemstack2.isEmpty()) 
		{
			inventoryIn->markDirty();
			return true;
		}

		inventoryIn->setInventorySlotContents(index, itemstack1);
	}

	return false;
}

bool TileEntityHopper::canInsertItemInSlot(IInventory* inventoryIn, ItemStack stack, int32_t index, EnumFacing side)
{
	if (!inventoryIn->isItemValidForSlot(index, stack)) 
	{
		return false;
	}
	else 
	{
		return !(Util::instanceof<ISidedInventory>(inventoryIn)) || ((ISidedInventory*)inventoryIn)->canInsertItem(index, stack, side);
	}
}

bool TileEntityHopper::canExtractItemFromSlot(IInventory* inventoryIn, ItemStack stack, int32_t index, EnumFacing side)
{
	return !(Util::instanceof< ISidedInventory>(inventoryIn)) || ((ISidedInventory)inventoryIn).canExtractItem(index, stack, side);
}

ItemStack TileEntityHopper::insertStack(IInventory* source, IInventory* destination, ItemStack stack, int32_t index,
	EnumFacing direction)
{
	ItemStack itemstack = destination.getStackInSlot(index);
	if (canInsertItemInSlot(destination, stack, index, direction)) {
		boolean flag = false;
		boolean flag1 = destination.isEmpty();
		if (itemstack.isEmpty()) {
			destination.setInventorySlotContents(index, stack);
			stack = ItemStack.EMPTY;
			flag = true;
		} else if (canCombine(itemstack, stack)) {
			int i = stack.getMaxStackSize() - itemstack.getCount();
			int j = Math.min(stack.getCount(), i);
			stack.shrink(j);
			itemstack.grow(j);
			flag = j > 0;
		}

		if (flag) {
			if (flag1 && destination instanceof TileEntityHopper) {
				TileEntityHopper tileentityhopper1 = (TileEntityHopper)destination;
				if (!tileentityhopper1.mayTransfer()) {
					int k = 0;
					if (source != nullptr && source instanceof TileEntityHopper) {
						TileEntityHopper tileentityhopper = (TileEntityHopper)source;
						if (tileentityhopper1.tickedGameTime >= tileentityhopper.tickedGameTime) {
							k = 1;
						}
					}

					tileentityhopper1.setTransferCooldown(8 - k);
				}
			}

			destination.markDirty();
		}
	}

	return stack;
}

IInventory* TileEntityHopper::getInventoryForHopperTransfer()
{
	EnumFacing enumfacing = BlockHopper.getFacing(getBlockMetadata());
      return getInventoryAtPosition(getWorld(), getXPos() + (double)enumfacing.getXOffset(), getYPos() + (double)enumfacing.getYOffset(), getZPos() + (double)enumfacing.getZOffset());
}

bool TileEntityHopper::canCombine(ItemStack stack1, ItemStack stack2)
{
	if (stack1.getItem() != stack2.getItem()) 
	{
		return false;
	}
	else if (stack1.getMetadata() != stack2.getMetadata()) 
	{
		return false;
	}
	else 
	{
		return stack1.getCount() > stack1.getMaxStackSize() ? false : ItemStack::areItemStackTagsEqual(stack1, stack2);
	}
}

void TileEntityHopper::setTransferCooldown(int32_t ticks)
{
	transferCooldown = ticks;
}

bool TileEntityHopper::isOnTransferCooldown() const
{
	return transferCooldown > 0;
}

bool TileEntityHopper::mayTransfer() const
{
	return transferCooldown > 8;
}

bool TileEntityHopper::putDropInInventoryAllSlots(IInventory* source, IInventory* destination, EntityItem* entity)
{
	boolean flag = false;
	if (entity == nullptr) 
	{
		return false;
	}
	else 
	{
		ItemStack itemstack = entity->getItem().copy();
		ItemStack itemstack1 = putStackInInventoryAllSlots(source, destination, itemstack, std::nullopt_t);
		if (itemstack1.isEmpty()) 
		{
			flag = true;
			entity->setDead();
		}
		else 
		{
			entity->setItem(itemstack1);
		}

		return flag;
	}
}

ItemStack TileEntityHopper::putStackInInventoryAllSlots(IInventory* source, IInventory* destination, ItemStack stack,
                                                        std::optional<EnumFacing> direction)
{
	if (Util::instanceof<ISidedInventory>(destination) && direction.has_value()) 
	{
		ISidedInventory* isidedinventory = (ISidedInventory*)destination;
		int[] aint = isidedinventory.getSlotsForFace(direction);

		for(int k = 0; k < aint.length && !stack.isEmpty(); ++k) {
			stack = insertStack(source, destination, stack, aint[k], direction);
		}
	}
	else 
	{
		int i = destination.getSizeInventory();

		for(int j = 0; j < i && !stack.isEmpty(); ++j) {
			stack = insertStack(source, destination, stack, j, direction);
		}
	}

	return stack;
}

IInventory* TileEntityHopper::getSourceInventory(IHopper* hopper)
{
	return getInventoryAtPosition(hopper->getWorld(), hopper->getXPos(), hopper->getYPos() + 1.0, hopper->getZPos());
}

std::vector<Entity*> TileEntityHopper::getCaptureItems(World* worldIn, double p_184292_1_, double p_184292_3_,
	double p_184292_5_)
{
	return worldIn->getEntitiesWithinAABB<EntityItem>(AxisAlignedBB(p_184292_1_ - 0.5, p_184292_3_, p_184292_5_ - 0.5, p_184292_1_ + 0.5, p_184292_3_ + 1.5, p_184292_5_ + 0.5), EntitySelectors::IS_ALIVE);
}

IInventory* TileEntityHopper::getInventoryAtPosition(World* worldIn, double x, double y, double z)
{
	IInventory* iinventory = nullptr;
	auto i = MathHelper::floor(x);
	auto j = MathHelper::floor(y);
	auto k = MathHelper::floor(z);
	BlockPos blockpos = BlockPos(i, j, k);
	Block block = worldIn->getBlockState(blockpos).getBlock();
	if (block.hasTileEntity()) 
	{
		TileEntity* tileentity = worldIn->getTileEntity(blockpos);
		if (Util::instanceof<IInventory>(tileentity)) 
		{
			iinventory = (IInventory*)tileentity;
			if (Util::instanceof<TileEntityChest>(iinventory) && Util::instanceof<BlockChest>(block)) 
			{
				iinventory = ((BlockChest)block).getContainer(worldIn, blockpos, true);
			}
		}
	}

	if (iinventory == nullptr) 
	{
		auto list = worldIn->getEntitiesInAABBexcluding((Entity)nullptr, AxisAlignedBB(x - 0.5, y - 0.5, z - 0.5, x + 0.5, y + 0.5, z + 0.5), EntitySelectors::HAS_INVENTORY);
		if (!list.isEmpty()) 
		{
			iinventory = (IInventory)list.get(worldIn->rand(list.size()));
		}
	}

	return (IInventory*)iinventory;
}

double TileEntityHopper::getXPos()
{
	return (double)pos.getx() + 0.5;
}

double TileEntityHopper::getYPos()
{
	return (double)pos.gety() + 0.5;
}

double TileEntityHopper::getZPos()
{
	return (double)pos.getz() + 0.5;
}

std::string TileEntityHopper::getGuiID() const
{
	return "minecraft:hopper";
}

Container TileEntityHopper::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
	fillWithLoot(playerIn);
	return ContainerHopper(playerInventory, this, playerIn);
}

std::vector<ItemStack> TileEntityHopper::getItems()
{
	return inventory;
}

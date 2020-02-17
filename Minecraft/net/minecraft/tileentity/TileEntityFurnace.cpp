#include "TileEntityFurnace.h"
#include "datafix/DataFixer.h"

const int32_t TileEntityFurnace::SLOTS_TOP[1] = {0};
const int32_t TileEntityFurnace::SLOTS_BOTTOM[2] = {2, 1};
const int32_t TileEntityFurnace::SLOTS_SIDES[1] = {1};

TileEntityFurnace::TileEntityFurnace()
	:furnaceItemStacks(3,ItemStack::EMPTY)
{
}

int32_t TileEntityFurnace::getSizeInventory()
{
	return furnaceItemStacks.size();
}

bool TileEntityFurnace::isEmpty()
{
	bool empty = false;

	for(auto stack : furnaceItemStacks)
	{
		empty = stack.isEmpty();
	}
	return empty;
}

ItemStack TileEntityFurnace::getStackInSlot(int32_t index)
{
	return furnaceItemStacks[index];
}

ItemStack TileEntityFurnace::decrStackSize(int32_t index, int32_t count)
{
	return ItemStackHelper::getAndSplit(furnaceItemStacks, index, count);
}

ItemStack TileEntityFurnace::removeStackFromSlot(int32_t index)
{
	return ItemStackHelper::getAndRemove(furnaceItemStacks, index);
}

void TileEntityFurnace::setInventorySlotContents(int32_t index, ItemStack stack)
{
	ItemStack itemstack = furnaceItemStacks[index];
	boolean flag = !stack.isEmpty() && stack.isItemEqual(itemstack) && ItemStack::areItemStackTagsEqual(stack, itemstack);
	furnaceItemStacks[index] = stack;
	if (stack.getCount() > getInventoryStackLimit()) 
	{
		stack.setCount(getInventoryStackLimit());
	}

	if (index == 0 && !flag) 
	{
		totalCookTime = getCookTime(stack);
		cookTime = 0;
		markDirty();
	}
}

std::string TileEntityFurnace::getName() const
{
	return hasCustomName() ? furnaceCustomName : "container.furnace";
}

bool TileEntityFurnace::hasCustomName() const
{
	return !furnaceCustomName.empty();
}

void TileEntityFurnace::setCustomInventoryName(std::string_view p_145951_1_)
{
	furnaceCustomName = p_145951_1_;
}

void TileEntityFurnace::registerFixesFurnace(DataFixer fixer)
{
	fixer.registerWalker(FixTypes::BLOCK_ENTITY, ItemStackDataLists(TileEntityFurnace.class, new String[]{"Items"}));
}

void TileEntityFurnace::readFromNBT(NBTTagCompound* compound)
{
	TileEntityLockable::readFromNBT(compound);
	std::vector<ItemStack>(getSizeInventory(),ItemStack::EMPTY).swap(furnaceItemStacks);
	ItemStackHelper::loadAllItems(compound, furnaceItemStacks);
	furnaceBurnTime = compound->getShort("BurnTime");
	cookTime = compound->getShort("CookTime");
	totalCookTime = compound->getShort("CookTimeTotal");
	currentItemBurnTime = getItemBurnTime(furnaceItemStacks[1]);
	if (compound->hasKey("CustomName", 8)) 
	{
		furnaceCustomName = compound->getString("CustomName");
	}	
}

NBTTagCompound* TileEntityFurnace::writeToNBT(NBTTagCompound* compound)
{
	TileEntityLockable::writeToNBT(compound);
	compound->setShort("BurnTime", (short)furnaceBurnTime);
	compound->setShort("CookTime", (short)cookTime);
	compound->setShort("CookTimeTotal", (short)totalCookTime);
	ItemStackHelper.saveAllItems(compound, furnaceItemStacks);
	if (hasCustomName()) 
	{
		compound->setString("CustomName", furnaceCustomName);
	}

	return compound;	
}

int32_t TileEntityFurnace::getInventoryStackLimit() const
{
	return 64;
}

bool TileEntityFurnace::isBurning() const
{
	return furnaceBurnTime > 0;
}
bool TileEntityFurnace::isBurning(IInventory* inventory)
{
	return inventory->getField(0) > 0;
}

void TileEntityFurnace::update()
{
	bool flag = isBurning();
	bool flag1 = false;
	if (isBurning()) 
	{
		--furnaceBurnTime;
	}

	if (!world->isRemote) 
	{
		ItemStack itemstack = furnaceItemStacks[1];
		if (!isBurning() && (itemstack.isEmpty() || furnaceItemStacks[0].isEmpty())) 
		{
			if (!isBurning() && cookTime > 0) 
			{
				cookTime = MathHelper::clamp<int16_t>(cookTime - 2, 0, totalCookTime);
			}
		}
		else 
		{
			if (!isBurning() && canSmelt()) 
			{
				furnaceBurnTime = getItemBurnTime(itemstack);
				currentItemBurnTime = furnaceBurnTime;
				if (isBurning()) 
				{
					flag1 = true;
					if (!itemstack.isEmpty()) 
					{
						Item item = itemstack.getItem();
						itemstack.shrink(1);
						if (itemstack.isEmpty()) 
						{
							Item item1 = item.getContainerItem();
							furnaceItemStacks[1] == (item1 == null ? ItemStack::EMPTY : ItemStack(item1));
						}
					}
				}
			}

			if (isBurning() && canSmelt()) 
			{
				++cookTime;
				if (cookTime == totalCookTime) 
				{
					cookTime = 0;
					totalCookTime = getCookTime(furnaceItemStacks[0]);
					smeltItem();
					flag1 = true;
				}
			}
			else 
			{
				cookTime = 0;
			}
		}

		if (flag != isBurning()) 
		{
			flag1 = true;
			BlockFurnace.setState(isBurning(), world, pos);
		}
	}

	if (flag1) 
	{
		markDirty();
	}
}

int32_t TileEntityFurnace::getCookTime(ItemStack stack) const
{
	return 200;
}

void TileEntityFurnace::smeltItem()
{
	if (canSmelt()) 
	{
		ItemStack itemstack = furnaceItemStacks[0];
		ItemStack itemstack1 = FurnaceRecipes.instance().getSmeltingResult(itemstack);
		ItemStack itemstack2 = furnaceItemStacks[2];
		if (itemstack2.isEmpty()) 
		{
			furnaceItemStacks[2] = itemstack1.copy();
		}
		else if (itemstack2.getItem() == itemstack1.getItem()) 
		{
			itemstack2.grow(1);
		}

		if (itemstack.getItem() == Item.getItemFromBlock(Blocks::SPONGE) && itemstack.getMetadata() == 1 && !((ItemStack)furnaceItemStacks[1]).isEmpty() && ((ItemStack)furnaceItemStacks[1]).getItem() == Items::BUCKET) 
		{
			furnaceItemStacks[1] = ItemStack(Items::WATER_BUCKET);
		}

		itemstack.shrink(1);
	}
}

int32_t TileEntityFurnace::getItemBurnTime(ItemStack stack)
{
	if (stack.isEmpty()) 
	{
		return 0;
	}
	else 
	{
		Item item = stack.getItem();
		if (item == Item.getItemFromBlock(Blocks.WOODEN_SLAB)) {
			return 150;
		} else if (item == Item.getItemFromBlock(Blocks.WOOL)) {
			return 100;
		} else if (item == Item.getItemFromBlock(Blocks.CARPET)) {
			return 67;
		} else if (item == Item.getItemFromBlock(Blocks.LADDER)) {
			return 300;
		} else if (item == Item.getItemFromBlock(Blocks.WOODEN_BUTTON)) {
			return 100;
		} else if (Block.getBlockFromItem(item).getDefaultState().getMaterial() == Material.WOOD) {
			return 300;
		} else if (item == Item.getItemFromBlock(Blocks.COAL_BLOCK)) {
			return 16000;
		} else if (item instanceof ItemTool && "WOOD".equals(((ItemTool)item).getToolMaterialName())) {
			return 200;
		} else if (item instanceof ItemSword && "WOOD".equals(((ItemSword)item).getToolMaterialName())) {
			return 200;
		} else if (item instanceof ItemHoe && "WOOD".equals(((ItemHoe)item).getMaterialName())) {
			return 200;
		} else if (item == Items.STICK) {
			return 100;
		} else if (item != Items.BOW && item != Items.FISHING_ROD) {
			if (item == Items.SIGN) {
				return 200;
			} else if (item == Items.COAL) {
				return 1600;
			} else if (item == Items.LAVA_BUCKET) {
				return 20000;
			} else if (item != Item.getItemFromBlock(Blocks.SAPLING) && item != Items.BOWL) {
				if (item == Items.BLAZE_ROD) {
					return 2400;
				} else if (item instanceof ItemDoor && item != Items.IRON_DOOR) {
					return 200;
				} else {
					return item instanceof ItemBoat ? 400 : 0;
				}
			} else {
				return 100;
			}
		} else {
			return 300;
		}
	}
}

bool TileEntityFurnace::isItemFuel(ItemStack stack)
{
	return getItemBurnTime(stack) > 0;
}

bool TileEntityFurnace::isUsableByPlayer(EntityPlayer* player)
{
	if (world->getTileEntity(pos) != this) 
	{
		return false;
	}
	else 
	{
		return player->getDistanceSq((double)pos.getx() + 0.5, (double)pos.gety() + 0.5, (double)pos.getz() + 0.5) <= 64.0;
	}
}

void TileEntityFurnace::openInventory(EntityPlayer* player)
{
}

void TileEntityFurnace::closeInventory(EntityPlayer* player)
{
}

bool TileEntityFurnace::isItemValidForSlot(int32_t index, ItemStack stack)
{
	if (index == 2) 
	{
		return false;
	}
	else if (index != 1) 
	{
		return true;
	}
	else 
	{
		ItemStack itemstack = (ItemStack)furnaceItemStacks[1];
		return isItemFuel(stack) || SlotFurnaceFuel.isBucket(stack) && itemstack.getItem() != Items::BUCKET;
	}
}

std::vector<int32_t> TileEntityFurnace::getSlotsForFace(EnumFacing side)
{
	if (side == EnumFacing::DOWN) 
	{
		return {std::begin(SLOTS_BOTTOM),std::end(SLOTS_BOTTOM)};
	}
	else 
	{
		if(side == EnumFacing::UP)
		{
			return {std::begin(SLOTS_TOP),std::end(SLOTS_TOP)};
		}
		else
		{
			return {std::begin(SLOTS_SIDES),std::end(SLOTS_SIDES)};
		}		
	}
}

bool TileEntityFurnace::canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction)
{
	return isItemValidForSlot(index, itemStackIn);
}

bool TileEntityFurnace::canExtractItem(int32_t index, ItemStack stack, EnumFacing direction)
{
	if (direction == EnumFacing::DOWN && index == 1) 
	{
		Item item = stack.getItem();
		if (item != Items::WATER_BUCKET && item != Items::BUCKET) 
		{
			return false;
		}
	}

	return true;
}

std::string TileEntityFurnace::getGuiID() const
{
	return "minecraft:furnace";
}

int32_t TileEntityFurnace::getField(int32_t id) const
{
	switch(id)
	{
	case 0:
		return furnaceBurnTime;
	case 1:
		return currentItemBurnTime;
	case 2:
		return cookTime;
	case 3:
		return totalCookTime;
	default:
		return 0;
	}
}

void TileEntityFurnace::setField(int32_t id, int32_t value)
{
	switch(id)
	{
	case 0:
		furnaceBurnTime = value;
		break;
	case 1:
		currentItemBurnTime = value;
		break;
	case 2:
		cookTime = value;
		break;
	case 3:
		totalCookTime = value;
	}
}

int32_t TileEntityFurnace::getFieldCount() const
{
	return 4;
}

void TileEntityFurnace::clear()
{
	furnaceItemStacks.clear();
}

Container createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
	return ContainerFurnace(playerInventory, *this);
}

bool TileEntityFurnace::canSmelt()
{
	if (furnaceItemStacks[0].isEmpty()) 
	{
		return false;
	}
	else 
	{
		ItemStack itemstack = FurnaceRecipes.instance().getSmeltingResult((ItemStack)furnaceItemStacks[0]);
		if (itemstack.isEmpty()) 
		{
			return false;
		}
		else 
		{
			ItemStack itemstack1 = (ItemStack)furnaceItemStacks[2];
			if (itemstack1.isEmpty()) 
			{
				return true;
			}
			else if (!itemstack1.isItemEqual(itemstack)) 
			{
				return false;
			}
			else if (itemstack1.getCount() < getInventoryStackLimit() && itemstack1.getCount() < itemstack1.getMaxStackSize()) 
			{
				return true;
			}
			else 
			{
				return itemstack1.getCount() < itemstack.getMaxStackSize();
			}
		}
	}
}

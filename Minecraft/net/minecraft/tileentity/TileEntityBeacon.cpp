#include "TileEntityBeacon.h"
#include "spdlog/fmt/bundled/format.h"

TileEntityBeacon::TileEntityBeacon()
	:payment(ItemStack::EMPTY)
{
	
}

void TileEntityBeacon::update()
{
	if (world->getTotalWorldTime() % 80L == 0L) 
	{
		updateBeacon();
	}
}

void TileEntityBeacon::updateBeacon()
{
	if (world != nullptr) 
	{
		updateSegmentColors();
		addEffectsToPlayers();
	}
}

std::vector<BeamSegment> TileEntityBeacon::getBeamSegments() const
{
	return beamSegments;
}

float TileEntityBeacon::shouldBeamRender()
{
	if (!isComplete) 
	{
		return 0.0F;
	}
	else 
	{
		int i = (int)(world->getTotalWorldTime() - beamRenderCounter);
		beamRenderCounter = world->getTotalWorldTime();
		if (i > 1) 
		{
			beamRenderScale -= (float)i / 40.0F;
			if (beamRenderScale < 0.0F) 
			{
				beamRenderScale = 0.0F;
			}
		}

		beamRenderScale += 0.025F;
		if (beamRenderScale > 1.0F) 
		{
			beamRenderScale = 1.0F;
		}

		return beamRenderScale;
	}
}

int32_t TileEntityBeacon::getLevels() const
{
	return levels;
}

std::optional<SPacketUpdateTileEntity> TileEntityBeacon::getUpdatePacket()
{
	return SPacketUpdateTileEntity(pos, 3, getUpdateTag());
}

NBTTagCompound* TileEntityBeacon::getUpdateTag()
{
	return writeToNBT(new NBTTagCompound());
}

double TileEntityBeacon::getMaxRenderDistanceSquared() const
{
	return 65536.0;
}

void TileEntityBeacon::readFromNBT(NBTTagCompound* compound)
{
	TileEntityLockable::readFromNBT(compound);
	primaryEffect = isBeaconEffect(compound->getInteger("Primary"));
	secondaryEffect = isBeaconEffect(compound->getInteger("Secondary"));
	levels = compound->getInteger("Levels");
}

NBTTagCompound* TileEntityBeacon::writeToNBT(NBTTagCompound* compound)
{
	TileEntityLockable::writeToNBT(compound);
	compound->setInteger("Primary", Potion::getIdFromPotion(primaryEffect));
	compound->setInteger("Secondary", Potion::getIdFromPotion(secondaryEffect));
	compound->setInteger("Levels", levels);
	return compound;
}

int32_t TileEntityBeacon::getSizeInventory() const
{
	return 1;
}

bool TileEntityBeacon::isEmpty()
{
	return payment.value().isEmpty();
}

ItemStack TileEntityBeacon::getStackInSlot(int32_t index) const
{
	return index == 0 ? payment : ItemStack::EMPTY;
}

ItemStack TileEntityBeacon::decrStackSize(int32_t index, int32_t count)
{
	if (index == 0 && !payment.isEmpty()) 
	{
		if (count >= payment.getCount()) 
		{
			ItemStack itemstack = payment;
			payment = ItemStack::EMPTY;
			return itemstack;
		}
		else 
		{
			return payment.splitStack(count);
		}
	}
	else 
	{
		return ItemStack::EMPTY;
	}
}

ItemStack TileEntityBeacon::removeStackFromSlot(int32_t index)
{
	if (index == 0) 
	{
		ItemStack itemstack = payment;
		payment = ItemStack::EMPTY;
		return itemstack;
	}
	else 
	{
		return ItemStack::EMPTY;
	}
}

void TileEntityBeacon::setInventorySlotContents(int32_t index, ItemStack stack)
{
	if (index == 0) 
	{
		payment = stack;
	}
}

std::string TileEntityBeacon::getName() const
{
	return hasCustomName() ? customName : "container.beacon";
}

bool TileEntityBeacon::hasCustomName() const
{
	return !customName.empty();
}

void TileEntityBeacon::setName(std::string name)
{
	customName = name;
}

int32_t TileEntityBeacon::getInventoryStackLimit() const
{
	return 1;
}

bool TileEntityBeacon::isUsableByPlayer(EntityPlayer* player)
{
	if (world->getTileEntity(pos) != this) 
	{
		return false;
	}
	else 
	{
		return player->getDistanceSq((double)pos.getX() + 0.5, (double)pos.getY() + 0.5, (double)pos.getZ() + 0.5) <= 64.0;
	}
}

void TileEntityBeacon::openInventory(EntityPlayer* player)
{
}

void TileEntityBeacon::closeInventory(EntityPlayer* player)
{
}

bool TileEntityBeacon::isItemValidForSlot(int32_t index, ItemStack stack)
{
	return stack.getItem() == Items.EMERALD || stack.getItem() == Items.DIAMOND || stack.getItem() == Items.GOLD_INGOT || stack.getItem() == Items.IRON_INGOT;
}

std::string TileEntityBeacon::getGuiID() const
{
	return "minecraft:beacon";
}

Container TileEntityBeacon::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
	return ContainerBeacon(playerInventory, this);
}

int32_t TileEntityBeacon::getField(int32_t id)
{
	switch(id)
	{
	case 0:
		return levels;
	case 1:
		return Potion::getIdFromPotion(primaryEffect);
	case 2:
		return Potion::getIdFromPotion(secondaryEffect);
	default:
		return 0;
	}
}

void TileEntityBeacon::setField(int32_t id, int32_t value)
{
	switch(id)
	{
	case 0:
		levels = value;
		break;
	case 1:
		primaryEffect = isBeaconEffect(value);
		break;
	case 2:
		secondaryEffect = isBeaconEffect(value);
	}
}

int32_t TileEntityBeacon::getFieldCount() const
{
	return 3;
}

void TileEntityBeacon::clear()
{
	payment = ItemStack::EMPTY;
}

bool TileEntityBeacon::receiveClientEvent(int32_t id, int32_t type)
{
	if (id == 1) 
	{
		updateBeacon();
		return true;
	}
	else 
	{
		return TileEntityLockable::receiveClientEvent(id, type);
	}
}

std::vector<int32_t> TileEntityBeacon::getSlotsForFace(EnumFacing side)
{
	return std::vector<int32_t>(1);
}

bool TileEntityBeacon::canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction)
{
	return false;
}

bool TileEntityBeacon::canExtractItem(int32_t index, ItemStack stack, EnumFacing direction)
{
	return false;
}

void TileEntityBeacon::addEffectsToPlayers()
{
	if (isComplete && levels > 0 && !world->isRemote && primaryEffect.has_value()) 
	{
		auto d0 = (double)(levels * 10 + 10);
		auto i = 0;
		if (levels >= 4 && primaryEffect == secondaryEffect) 
		{
			i = 1;
		}

		auto j = (9 + levels * 2) * 20;
		auto k = pos.getX();
		auto l = pos.getY();
		auto i1 = pos.getZ();
		AxisAlignedBB axisalignedbb = (AxisAlignedBB((double)k, (double)l, (double)i1, (double)(k + 1), (double)(l + 1), (double)(i1 + 1))).grow(d0).expand(0.0D, (double)this.world.getHeight(), 0.0D);
		auto list = world->getEntitiesWithinAABB<EntityPlayer>(axisalignedbb);
		Iterator var10 = list.iterator();

		EntityPlayer entityplayer1;
		while(var10.hasNext()) 
		{
			entityplayer1 = (EntityPlayer)var10.next();
			entityplayer1.addPotionEffect(PotionEffect(primaryEffect, j, i, true, true));
		}

		if (levels >= 4 && primaryEffect != secondaryEffect && secondaryEffect != nullptr) 
		{
			var10 = list.iterator();

			while(var10.hasNext()) 
			{
				entityplayer1 = (EntityPlayer)var10.next();
				entityplayer1.addPotionEffect(PotionEffect(secondaryEffect, j, 0, true, true));
			}
		}
	}
}

void TileEntityBeacon::updateSegmentColors()
{
	auto i = pos.getX();
	auto j = pos.getY();
	auto k = pos.getZ();
	auto l = levels;
	levels = 0;
	beamSegments.clear();
	isComplete = true;
	TileEntityBeacon::BeamSegment tileentitybeacon$beamsegment = TileEntityBeacon::BeamSegment(EnumDyeColor.WHITE.getColorComponentValues());
	beamSegments.push_back(tileentitybeacon$beamsegment);
	bool flag = true;
	MutableBlockPos blockpos$mutableblockpos;

	int l1;
	for(l1 = j + 1; l1 < 256; ++l1) 
	{
		IBlockState* iblockstate = world->getBlockState(blockpos$mutableblockpos.setPos(i, l1, k));
		float[] afloat;
		if (iblockstate.getBlock() == Blocks.STAINED_GLASS) 
		{
			afloat = ((EnumDyeColor)iblockstate.getValue(BlockStainedGlass.COLOR)).getColorComponentValues();
		}
		else 
		{
			if (iblockstate.getBlock() != Blocks.STAINED_GLASS_PANE) 
			{
				if (iblockstate.getLightOpacity() >= 15 && iblockstate.getBlock() != Blocks.BEDROCK) 
				{
					isComplete = false;
					beamSegments.clear();
					break;
				}

				tileentitybeacon$beamsegment.incrementHeight();
				continue;
			}

			afloat = ((EnumDyeColor)iblockstate.getValue(BlockStainedGlassPane.COLOR)).getColorComponentValues();
		}

		if (!flag) 
		{
			afloat = new float[]{(tileentitybeacon$beamsegment.getColors()[0] + afloat[0]) / 2.0F, (tileentitybeacon$beamsegment.getColors()[1] + afloat[1]) / 2.0F, (tileentitybeacon$beamsegment.getColors()[2] + afloat[2]) / 2.0F};
		}

		if (Arrays.equals(afloat, tileentitybeacon$beamsegment.getColors())) 
		{
			tileentitybeacon$beamsegment.incrementHeight();
		}
		else 
		{
			tileentitybeacon$beamsegment = new TileEntityBeacon.BeamSegment(afloat);
			beamSegments.add(tileentitybeacon$beamsegment);
		}

		flag = false;
	}

	if (isComplete) 
	{
		for(l1 = 1; l1 <= 4; levels = l1++) {
			int i2 = j - l1;
			if (i2 < 0) {
				break;
			}

			bool flag1 = true;

			for(int j1 = i - l1; j1 <= i + l1 && flag1; ++j1) 
			{
				for(int k1 = k - l1; k1 <= k + l1; ++k1) 
				{
					Block block = world->getBlockState(BlockPos(j1, i2, k1)).getBlock();
					if (block != Blocks::EMERALD_BLOCK && block != Blocks.GOLD_BLOCK && block != Blocks.DIAMOND_BLOCK && block != Blocks.IRON_BLOCK) 
					{
						flag1 = false;
						break;
					}
				}
			}

			if (!flag1) 
			{
				break;
			}
		}

		if (levels == 0) 
		{
			isComplete = false;
		}
	}

	if (!world.isRemote && l < levels) 
	{
		Iterator var14 = world.getEntitiesWithinAABB(EntityPlayerMP.class, (new AxisAlignedBB((double)i, (double)j, (double)k, (double)i, (double)(j - 4), (double)k)).grow(10.0D, 5.0D, 10.0D)).iterator();

		while(var14.hasNext()) 
		{
			EntityPlayerMP entityplayermp = (EntityPlayerMP)var14.next();
			CriteriaTriggers.CONSTRUCT_BEACON.trigger(entityplayermp, this);
		}
	}
}

std::optional<Potion> TileEntityBeacon::isBeaconEffect(int32_t p_184279_0_)
{
	Potion potion = Potion::getPotionById(p_184279_0_);
	return VALID_EFFECTS.find(potion) != VALID_EFFECTS.end() ? potion : std::nullopt;
}

void TileEntityBeacon::BeamSegment::incrementHeight()
{
	++height;
}

std::vector<float> TileEntityBeacon::BeamSegment::getColors() const
{
	return colors;
}

int32_t TileEntityBeacon::BeamSegment::getHeight() const
{
	return height;
}

TileEntityBeacon::BeamSegment::BeamSegment(std::vector<float> colorsIn)
	:colors(colorsIn),height(1)
{
}

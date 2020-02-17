#include "TileEntityShulkerBox.h"
#include "datafix/DataFixer.h"

TileEntityShulkerBox::TileEntityShulkerBox()
	:TileEntityShulkerBox(std::nullopt)
{
}

TileEntityShulkerBox::TileEntityShulkerBox(std::optional<EnumDyeColor> colorIn)
	:items(27,ItemStack::EMPTY),animationStatus(TileEntityShulkerBox::AnimationStatus::CLOSED),color(colorIn)
{
}

void TileEntityShulkerBox::update()
{
	updateAnimation();
	if (animationStatus == TileEntityShulkerBox::AnimationStatus::OPENING || animationStatus == TileEntityShulkerBox::AnimationStatus::CLOSING) 
	{
      moveCollidedEntities();
	}
}

TileEntityShulkerBox::AnimationStatus TileEntityShulkerBox::getAnimationStatus()
{
   return animationStatus;
}

AxisAlignedBB TileEntityShulkerBox::getBoundingBox(IBlockState* p_190584_1_)
{
   return getBoundingBox((EnumFacing)p_190584_1_.getValue(BlockShulkerBox::FACING));
}

AxisAlignedBB TileEntityShulkerBox::getBoundingBox(EnumFacing p_190587_1_)
{
   return Block::FULL_BLOCK_AABB.expand((double)(0.5F * getProgress(1.0F) * (float)p_190587_1_.getXOffset()), (double)(0.5F * getProgress(1.0F) * (float)p_190587_1_.getYOffset()), (double)(0.5F * getProgress(1.0F) * (float)p_190587_1_.getZOffset()));
}

int32_t TileEntityShulkerBox::getSizeInventory() const
{
   return items.size();
}

int32_t TileEntityShulkerBox::getInventoryStackLimit() const
{
   return 64;
}

bool TileEntityShulkerBox::receiveClientEvent(int32_t id, int32_t type)
{
   if (id == 1) 
   {
      openCount = type;
      if (type == 0) 
      {
         animationStatus = AnimationStatus::CLOSING;
      }

      if (type == 1) 
      {
         animationStatus = AnimationStatus::OPENING;
      }

      return true;
   }
   else 
   {
      return TileEntityLockableLoot::receiveClientEvent(id, type);
   }
}

void TileEntityShulkerBox::openInventory(EntityPlayer* player)
{
   if (!player->isSpectator())
   {
      if (openCount < 0) 
      {
         openCount = 0;
      }

      ++openCount;
      world->addBlockEvent(pos, getBlockType(), 1, openCount);
      if (openCount == 1) {
         world->playSound(nullptr, pos, SoundEvents::BLOCK_SHULKER_BOX_OPEN, SoundCategory::BLOCKS, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
      }
   }
}

Container TileEntityShulkerBox::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
   return ContainerShulkerBox(playerInventory, this, playerIn);
}

std::string TileEntityShulkerBox::getGuiID() const
{
   return "minecraft:shulker_box";
}

std::string TileEntityShulkerBox::getName() const
{
   return hasCustomName() ? customName : "container.shulkerBox";
}

void TileEntityShulkerBox::registerFixesShulkerBox(DataFixer fixer)
{
   fixer.registerWalker(FixTypes.BLOCK_ENTITY, new ItemStackDataLists(TileEntityShulkerBox.class, new String[]{"Items"}));
}

void TileEntityShulkerBox::readFromNBT(NBTTagCompound* compound)
{
   TileEntityLockableLoot::readFromNBT(compound);
   loadFromNbt(compound);
}

NBTTagCompound* TileEntityShulkerBox::writeToNBT(NBTTagCompound* compound)
{
   TileEntityLockableLoot::writeToNBT(compound);
   return saveToNbt(compound);
}

void TileEntityShulkerBox::loadFromNbt(NBTTagCompound* compound)
{
   std::vector<ItemStack>(getSizeInventory(), ItemStack::EMPTY).swap(items);
   if (!checkLootAndRead(compound) && compound->hasKey("Items", 9)) 
   {
      ItemStackHelper::loadAllItems(compound, items);
   }

   if (compound->hasKey("CustomName", 8)) {
      customName = compound->getString("CustomName");
   }
}

NBTTagCompound* TileEntityShulkerBox::saveToNbt(NBTTagCompound* compound)
{
   if (!checkLootAndWrite(compound)) 
   {
      ItemStackHelper::saveAllItems(compound, items, false);
   }

   if (hasCustomName()) 
   {
      compound->setString("CustomName", customName);
   }

   if (!compound->hasKey("Lock") && isLocked()) 
   {
      getLockCode().toNBT(compound);
   }

   return compound;
}

bool TileEntityShulkerBox::isEmpty()
{
   for(auto stack : items)
   {
      if(!stack.isEmpty())
      {
         return false;
      }
   }
   return true;
}

std::vector<int32_t> TileEntityShulkerBox::getSlotsForFac(EnumFacing side) const
{
   return {std::begin(SLOTS),std::end(SLOTS)};
}

bool TileEntityShulkerBox::canInsertItem(int32_t index, ItemStack itemStackIn, EnumFacing direction)
{
   return !Util::instanceof<BlockShulkerBox>(Block::getBlockFromItem(itemStackIn.getItem()));
}

bool TileEntityShulkerBox::canExtractItem(int index, ItemStack stack, EnumFacing direction)
{
   return true;
}

void TileEntityShulkerBox::clear()
{
   hasBeenCleared = true;
   TileEntityLockableLoot::clear();
}

bool TileEntityShulkerBox::isCleared() const
{
   return hasBeenCleared;
}

EnumDyeColor TileEntityShulkerBox::getColor()
{
   if (color == std::nullopt) 
   {
      color = BlockShulkerBox.getColorFromBlock(getBlockType());
   }

   return color;
}

std::optional<SPacketUpdateTileEntity> TileEntityShulkerBox::getUpdatePacket()
{
   return SPacketUpdateTileEntity(pos, 10, getUpdateTag());
}

bool TileEntityShulkerBox::isDestroyedByCreativePlayer() const
{
   return destroyedByCreativePlayer;
}

void TileEntityShulkerBox::setDestroyedByCreativePlayer(bool p_190579_1_)
{
   destroyedByCreativePlayer = p_190579_1_;
}

bool TileEntityShulkerBox::shouldDrop()
{
   return !isDestroyedByCreativePlayer() || !isEmpty() || hasCustomName() || lootTable != nullptr;
}

float TileEntityShulkerBox::getProgress(float p_190585_1_) const
{
   return progressOld + (progress - progressOld) * p_190585_1_;
}

void TileEntityShulkerBox::closeInventory(EntityPlayer* player)
{
   if (!player->isSpectator()) 
   {
      --openCount;
      world->addBlockEvent(pos, getBlockType(), 1, openCount);
      if (openCount <= 0) {
         world->playSound(nullptr, pos, SoundEvents::BLOCK_SHULKER_BOX_CLOSE, SoundCategory::BLOCKS, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
      }
   }
}

void TileEntityShulkerBox::updateAnimation()
{
   progressOld = progress;
   switch(animationStatus)
   {
   case AnimationStatus::CLOSED:
      progress = 0.0F;
      break;
   case AnimationStatus::OPENING:
      progress += 0.1F;
      if (progress >= 1.0F) {
         moveCollidedEntities();
         animationStatus = TileEntityShulkerBox::AnimationStatus::OPENED;
         progress = 1.0F;
      }
      break;
   case AnimationStatus::CLOSING:
      progress -= 0.1F;
      if (progress <= 0.0F) {
         animationStatus = TileEntityShulkerBox::AnimationStatus::CLOSED;
         progress = 0.0F;
      }
      break;
   case AnimationStatus::OPENED:
      progress = 1.0F;
   }
}

std::vector<ItemStack> TileEntityShulkerBox::getItems()
{
   return items;
}

AxisAlignedBB TileEntityShulkerBox::getTopBoundingBox(EnumFacing p_190588_1_)
{
   EnumFacing enumfacing = p_190588_1_.getOpposite();
   return getBoundingBox(p_190588_1_).contract((double)enumfacing.getXOffset(), (double)enumfacing.getYOffset(), (double)enumfacing.getZOffset());
}

void TileEntityShulkerBox::moveCollidedEntities()
{
   auto iblockstate = world->getBlockState(getPos());
   if (Util::instanceof< BlockShulkerBox>(iblockstate->getBlock())) 
   {
      EnumFacing enumfacing = (EnumFacing)iblockstate->getValue(BlockShulkerBox::FACING);
      AxisAlignedBB axisalignedbb = getTopBoundingBox(enumfacing).offset(pos);
      std::vector<Entity*> list = world->getEntitiesWithinAABBExcludingEntity(nullptr, axisalignedbb);
      if (!list.empty()) 
      {
         for(int i = 0; i < list.size(); ++i) 
         {
            Entity* entity = list[i];
            if (entity->getPushReaction() != EnumPushReaction::IGNORE) 
            {
               double d0 = 0.0;
               double d1 = 0.0;
               double d2 = 0.0;
               AxisAlignedBB axisalignedbb1 = entity->getEntityBoundingBox();
               switch(enumfacing.getAxis())
               {
               case Axis::X:
                  if (enumfacing.getAxisDirection() == AxisDirection::POSITIVE) 
                  {
                     d0 = axisalignedbb.getmaxX() - axisalignedbb1.getminX();
                  } else {
                     d0 = axisalignedbb1.getmaxX() - axisalignedbb.getminX();
                  }

                  d0 += 0.01;
                  break;
               case Axis::Y:
                  if (enumfacing.getAxisDirection() == AxisDirection::POSITIVE) 
                  {
                     d1 = axisalignedbb.getmaxY() - axisalignedbb1.getminY();
                  } else {
                     d1 = axisalignedbb1.getmaxY()- axisalignedbb.getminY();
                  }

                  d1 += 0.01;
                  break;
               case Axis::Z:
                  if (enumfacing.getAxisDirection() == AxisDirection::POSITIVE) 
                  {
                     d2 = axisalignedbb.getmaxZ() - axisalignedbb1.getminZ();
                  } else {
                     d2 = axisalignedbb1.getmaxZ() - axisalignedbb.getminZ();
                  }

                  d2 += 0.01;
               }

               entity->move(MoverType::SHULKER_BOX, d0 * (double)enumfacing.getXOffset(), d1 * (double)enumfacing.getYOffset(), d2 * (double)enumfacing.getZOffset());
            }
         }
      }
   }
}

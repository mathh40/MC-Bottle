#include "EntityItemFrame.h"

#include "DamageSource.h"
#include "../../item/ItemMap.h"
#include "datafix/DataFixer.h"
#include "storage/MapData.h"

DataParameter EntityItemFrame::ITEM = EntityDataManager::createKey(EntityItemFrame.class, DataSerializers::ITEM_STACK);
DataParameter EntityItemFrame::ROTATION = EntityDataManager::createKey(EntityItemFrame.class, DataSerializers::VARINT);

EntityItemFrame::EntityItemFrame(World *worldIn)
    :EntityHanging(worldIn){
}

EntityItemFrame::EntityItemFrame(World *worldIn, BlockPos pos, EnumFacing facing)
    :EntityHanging(worldIn,pos){
    updateFacingWithBoundingBox(facing);
}

float EntityItemFrame::getCollisionBorderSize() {
    return 0.0F;
}

bool EntityItemFrame::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
         return false;
      } else if (!source.isExplosion() && !getDisplayedItem().isEmpty()) {
         if (!world->isRemote) {
            dropItemOrSelf(source.getTrueSource(), false);
            playSound(SoundEvents.ENTITY_ITEMFRAME_REMOVE_ITEM, 1.0F, 1.0F);
            setDisplayedItem(ItemStack::EMPTY);
         }

         return true;
      } else {
         return EntityHanging::attackEntityFrom(source, amount);
      }
}

int32_t EntityItemFrame::getWidthPixels() {
    return 12;
}

int32_t EntityItemFrame::getHeightPixels() {
    return 12;
}

bool EntityItemFrame::isInRangeToRenderDist(double distance) {
    double d0 = 16.0;
    d0 = d0 * 64.0 * getRenderDistanceWeight();
    return distance < d0 * d0;
}

void EntityItemFrame::onBroken(Entity *brokenEntity) {
    playSound(SoundEvents::ENTITY_ITEMFRAME_BREAK, 1.0F, 1.0F);
    dropItemOrSelf(brokenEntity, true);
}

void EntityItemFrame::playPlaceSound() {
    playSound(SoundEvents::ENTITY_ITEMFRAME_PLACE, 1.0F, 1.0F);
}

void EntityItemFrame::dropItemOrSelf(Entity *entityIn, bool p_146065_2_) {
    if (world->getGameRules().getBoolean("doEntityDrops")) {
         ItemStack itemstack = getDisplayedItem();
         if (Util::instanceof<EntityPlayer>(entityIn)) {
            EntityPlayer* entityplayer = (EntityPlayer*)entityIn;
            if (entityplayer->capabilities.isCreativeMode) {
               removeFrameFromMap(itemstack);
               return;
            }
         }

         if (p_146065_2_) {
            entityDropItem(ItemStack(Items::ITEM_FRAME), 0.0F);
         }

         if (!itemstack.isEmpty() && MathHelper::nextFloat(rand) < itemDropChance) {
            itemstack = itemstack.copy();
            removeFrameFromMap(itemstack);
            entityDropItem(itemstack, 0.0F);
         }
      }
}

ItemStack EntityItemFrame::getDisplayedItem() {
    return getDataManager().get(ITEM);
}

void EntityItemFrame::setDisplayedItem(ItemStack stack) {
    setDisplayedItemWithUpdate(stack, true);
}

void EntityItemFrame::notifyDataManagerChange(DataParameter key) {
    if (key == ITEM) {
         ItemStack itemstack = getDisplayedItem();
         if (!itemstack.isEmpty() && itemstack.getItemFrame() != this) {
            itemstack.setItemFrame(this);
         }
      }
}

int32_t EntityItemFrame::getRotation() {
    return getDataManager().get(ROTATION);
}

void EntityItemFrame::setItemRotation(int32_t rotationIn) {
    setRotation(rotationIn, true);
}

void EntityItemFrame::registerFixesItemFrame(DataFixer fixer) {
    fixer.registerWalker(FixTypes::ENTITY, new ItemStackData(EntityItemFrame.class, {"Item"}));
}

void EntityItemFrame::writeEntityToNBT(NBTTagCompound *compound) {
    if (!getDisplayedItem().isEmpty()) {
         compound->setTag("Item", getDisplayedItem().writeToNBT(new NBTTagCompound()));
         compound->setByte("ItemRotation", (std::byte)getRotation());
         compound->setFloat("ItemDropChance", itemDropChance);
      }

      EntityHanging::writeEntityToNBT(compound);
}

void EntityItemFrame::readEntityFromNBT(NBTTagCompound *compound) {
    NBTTagCompound* nbttagcompound = compound->getCompoundTag("Item");
      if (nbttagcompound != nullptr && !nbttagcompound->isEmpty()) {
         setDisplayedItemWithUpdate(ItemStack(nbttagcompound), false);
         setRotation(compound->getByte("ItemRotation"), false);
         if (compound->hasKey("ItemDropChance", 99)) {
            itemDropChance = compound->getFloat("ItemDropChance");
         }
      }

      EntityHanging::readEntityFromNBT(compound);
}

bool EntityItemFrame::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    ItemStack itemstack = player->getHeldItem(hand);
      if (!world->isRemote) {
         if (getDisplayedItem().isEmpty()) {
            if (!itemstack.isEmpty()) {
               setDisplayedItem(itemstack);
               if (!player->capabilities.isCreativeMode) {
                  itemstack.shrink(1);
               }
            }
         } else {
            playSound(SoundEvents::ENTITY_ITEMFRAME_ROTATE_ITEM, 1.0F, 1.0F);
            setItemRotation(getRotation() + 1);
         }
      }

      return true;
}

int32_t EntityItemFrame::getAnalogOutput() {
    return getDisplayedItem().isEmpty() ? 0 : getRotation() % 8 + 1;
}

void EntityItemFrame::entityInit() {
    getDataManager().registe(ITEM, ItemStack::EMPTY);
    getDataManager().registe(ROTATION, 0);
}

void EntityItemFrame::removeFrameFromMap(ItemStack stack) {
    if (!stack.isEmpty()) {
         if (stack.getItem() == Items::FILLED_MAP) {
            MapData mapdata = ((ItemMap*)stack.getItem())->getMapData(stack, world);
            mapdata.mapDecorations.erase("frame-" + getEntityId());
         }

         stack.setItemFrame(nullptr);
      }
}

void EntityItemFrame::setDisplayedItemWithUpdate(ItemStack stack, bool p_174864_2_) {
    if (!stack.isEmpty()) {
         stack = stack.copy();
         stack.setCount(1);
         stack.setItemFrame(this);
      }

      getDataManager().set(ITEM, stack);
      getDataManager().setDirty(ITEM);
      if (!stack.isEmpty()) {
         playSound(SoundEvents::ENTITY_ITEMFRAME_ADD_ITEM, 1.0F, 1.0F);
      }

      if (p_174864_2_ && hangingPosition != nullptr) {
         world->updateComparatorOutputLevel(hangingPosition, Blocks::AIR);
      }
}

void EntityItemFrame::setRotation(int32_t rotationIn, bool p_174865_2_) {
    getDataManager().set(ROTATION, rotationIn % 8);
      if (p_174865_2_ && hangingPosition != nullptr) {
         world->updateComparatorOutputLevel(hangingPosition, Blocks::AIR);
      }
}

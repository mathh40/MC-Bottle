#include "EntityPlayerMP.h"

#include "CooldownTrackerServer.h"
#include "EntityDamageSource.h"
#include "ReportedException.h"
#include "WorldProvider.h"
#include "WorldServer.h"
#include "../EntityList.h"
#include "../../inventory/ContainerChest.h"
#include "../../inventory/ContainerHorseInventory.h"
#include "../../inventory/ContainerMerchant.h"
#include "../../inventory/SlotCrafting.h"
#include "../../item/ItemMapBase.h"
#include "../../item/crafting/CraftingManager.h"
#include "../../scoreboard/IScoreCriteria.h"
#include "../../scoreboard/ScoreObjective.h"
#include "../../scoreboard/ScorePlayerTeam.h"
#include "../../scoreboard/Team.h"
#include "../../tileentity/TileEntityCommandBlock.h"
#include "../../tileentity/TileEntitySign.h"
#include "../../village/MerchantRecipeList.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"
#include "datafix/IDataWalker.h"
#include "storage/WorldInfo.h"
#include "storage/loot/ILootContainer.h"
#include "text/ChatType.h"

class EntityPlayerMPDataWalker : public IDataWalker {
public:
    NBTTagCompound* process(IDataFixer* fixer, NBTTagCompound* compound, int32_t versionIn) {
            if (compound->hasKey("RootVehicle", 10)) {
               NBTTagCompound* nbttagcompound = compound->getCompoundTag("RootVehicle");
               if (nbttagcompound->hasKey("Entity", 10)) {
                  nbttagcompound->setTag("Entity", fixer.process(FixTypes::ENTITY, nbttagcompound->getCompoundTag("Entity"), versionIn));
               }
            }

            return compound;
    }
};

std::shared_ptr<spdlog::logger> EntityLivingBase::LOGGER = spdlog::get("Minecraft")->clone("EntityPlayerMP");

EntityPlayerMP::EntityPlayerMP(MinecraftServer* serverIn, WorldServer* worldIn, GameProfile profile, PlayerInteractionManager interactionManagerIn)
    :EntityPlayer(worldIn, profile),interactionManager(interactionManagerIn){
      interactionManagerIn.player = this;
      BlockPos blockpos = worldIn->getSpawnPoint();
      if (worldIn->provider->hasSkyLight() && worldIn->getWorldInfo().getGameType() != GameType::ADVENTURE) {
         auto i = MathHelper::max(0, serverIn->getSpawnRadius(worldIn));
         auto j = MathHelper::floor(worldIn->getWorldBorder().getClosestDistance((double)blockpos.getx(), (double)blockpos.getz()));
         if (j < i) {
            i = j;
         }

         if (j <= 1) {
            i = 1;
         }

         blockpos = worldIn->getTopSolidOrLiquidBlock(blockpos.add(rand(i * 2 + 1) - i, 0, rand(i * 2 + 1) - i));
      }

      server = serverIn;
      statsFile = serverIn->getPlayerList().getPlayerStatsFile(this);
      advancements = serverIn->getPlayerList().getPlayerAdvancements(this);
      stepHeight = 1.0F;
      moveToBlockPosAndAngles(blockpos, 0.0F, 0.0F);

      while(!worldIn->getCollisionBoxes(this, getEntityBoundingBox())->isEmpty() && posY < 255.0) {
         setPosition(posX, posY + 1.0, posZ);
      }
}

void EntityPlayerMP::readEntityFromNBT(NBTTagCompound *compound) {
    EntityPlayer::readEntityFromNBT(compound);
      if (compound->hasKey("playerGameType", 99)) {
         if (getServer()->getForceGamemode()) {
            interactionManager.setGameType(getServer()->getGameType());
         } else {
            interactionManager.setGameType(GameType::getByID(compound->getInteger("playerGameType")));
         }
      }

      if (compound->hasKey("enteredNetherPosition", 10)) {
         NBTTagCompound* nbttagcompound = compound->getCompoundTag("enteredNetherPosition");
         enteredNetherPosition = Vec3d(nbttagcompound->getDouble("x"), nbttagcompound->getDouble("y"), nbttagcompound->getDouble("z"));
      }

      seenCredits = compound->getBoolean("seenCredits");
      if (compound->hasKey("recipeBook", 10)) {
         recipeBook.read(compound->getCompoundTag("recipeBook"));
      }
}

void EntityPlayerMP::registerFixesPlayerMP(DataFixer p_191522_0_) {
     p_191522_0_.registerWalker(FixTypes::PLAYER, new EntityPlayerMPDataWalker());
}

void EntityPlayerMP::writeEntityToNBT(NBTTagCompound *compound) {
    EntityPlayer::writeEntityToNBT(compound);
      compound->setInteger("playerGameType", interactionManager.getGameType().getID());
      compound->setBoolean("seenCredits", seenCredits);
      if (enteredNetherPosition != nullptr) {
         NBTTagCompound* nbttagcompound = new NBTTagCompound();
         nbttagcompound->setDouble("x", enteredNetherPosition.getx());
         nbttagcompound->setDouble("y", enteredNetherPosition.gety());
         nbttagcompound->setDouble("z", enteredNetherPosition.getz());
         compound->setTag("enteredNetherPosition", nbttagcompound);
      }

      Entity* entity1 = getLowestRidingEntity();
      Entity* entity = getRidingEntity();
      if (entity != nullptr && entity1 != this && entity1->getRecursivePassengersByType(EntityPlayerMP.class).size() == 1) {
         NBTTagCompound* nbttagcompound1 = new NBTTagCompound();
         NBTTagCompound* nbttagcompound2 = new NBTTagCompound();
         entity1->writeToNBTOptional(nbttagcompound2);
         nbttagcompound1->setUniqueId("Attach", entity->getUniqueID());
         nbttagcompound1->setTag("Entity", nbttagcompound2);
         compound->setTag("RootVehicle", nbttagcompound1);
      }

      compound->setTag("recipeBook", recipeBook.write());
}

void EntityPlayerMP::addExperienceLevel(int32_t levels) {
    EntityPlayer::addExperienceLevel(levels);
      lastExperience = -1;
}

void EntityPlayerMP::onEnchant(ItemStack enchantedItem, int32_t cost) {
    EntityPlayer::onEnchant(enchantedItem, cost);
      lastExperience = -1;
}

void EntityPlayerMP::addSelfToInternalCraftingInventory() {
    openContainer->addListener(this);
}

void EntityPlayerMP::sendEnterCombat() {
    EntityPlayer::sendEnterCombat();
      connection.sendPacket(new SPacketCombatEvent(getCombatTracker(), SPacketCombatEvent::Event::ENTER_COMBAT));
}

void EntityPlayerMP::sendEndCombat() {
    EntityPlayer::sendEndCombat();
      connection.sendPacket(new SPacketCombatEvent(getCombatTracker(), SPacketCombatEvent::Event::END_COMBAT));
}

void EntityPlayerMP::onUpdate() {
    interactionManager.updateBlockRemoving();
      --respawnInvulnerabilityTicks;
      if (hurtResistantTime > 0) {
         --hurtResistantTime;
      }

      openContainer->detectAndSendChanges();
      if (!world->isRemote && !openContainer->canInteractWith(this)) {
         closeScreen();
         openContainer = inventoryContainer.get();
      }

      while(!entityRemoveQueue.empty()) {
         auto i = MathHelper::min<int32_t>(entityRemoveQueue.size(), std::numeric_limits<int32_t>::max());
            std::vector<int32_t> aint(i);
         auto iterator = entityRemoveQueue.begin();
         auto j = 0;
         while(iterator != entityRemoveQueue.begin() && j < i) {
            aint[j++] = *iterator;
             iterator = entityRemoveQueue.erase(iterator);
         }

         connection.sendPacket(new SPacketDestroyEntities(aint));
      }

      Entity* entity = getSpectatingEntity();
      if (entity != this) {
         if (entity->isEntityAlive()) {
            setPositionAndRotation(entity->posX, entity->posY, entity->posZ, entity->rotationYaw, entity->rotationPitch);
            server->getPlayerList().serverUpdateMovingPlayer(this);
            if (isSneaking()) {
               setSpectatingEntity(this);
            }
         } else {
            setSpectatingEntity(this);
         }
      }

      CriteriaTriggers::TICK.trigger(this);
      if (levitationStartPos.has_value()) {
         CriteriaTriggers::LEVITATION.trigger(this, levitationStartPos, ticksExisted - levitatingSince);
      }

      advancements.flushDirty(this);
}

void EntityPlayerMP::onUpdateEntity() {
    try {
         EntityPlayer::onUpdate();

         for(auto i = 0; i < inventory.getSizeInventory(); ++i) {
            ItemStack itemstack = inventory.getStackInSlot(i);
            if (!itemstack.isEmpty() && itemstack.getItem()->isMap()) {
               Packet* packet = ((ItemMapBase*)itemstack.getItem())->createMapDataPacket(itemstack, world, this);
               if (packet != nullptr) {
                  connection.sendPacket(packet);
               }
            }
         }

         if (getHealth() != lastHealth || lastFoodLevel != foodStats.getFoodLevel() || foodStats.getSaturationLevel() == 0.0F != wasHungry) {
            connection.sendPacket(new SPacketUpdateHealth(getHealth(), foodStats.getFoodLevel(), foodStats.getSaturationLevel()));
            lastHealth = getHealth();
            lastFoodLevel = foodStats.getFoodLevel();
            wasHungry = foodStats.getSaturationLevel() == 0.0F;
         }

         if (getHealth() + getAbsorptionAmount() != lastHealthScore) {
            lastHealthScore = getHealth() + getAbsorptionAmount();
            updateScorePoints(IScoreCriteria::HEALTH, MathHelper::ceil(lastHealthScore));
         }

         if (foodStats.getFoodLevel() != lastFoodScore) {
            lastFoodScore = foodStats.getFoodLevel();
            updateScorePoints(IScoreCriteria::FOOD, MathHelper::ceil((float)lastFoodScore));
         }

         if (getAir() != lastAirScore) {
            lastAirScore = getAir();
            updateScorePoints(IScoreCriteria::AIR, MathHelper::ceil((float)lastAirScore));
         }

         if (getTotalArmorValue() != lastArmorScore) {
            lastArmorScore = getTotalArmorValue();
            updateScorePoints(IScoreCriteria::ARMOR, MathHelper::ceil((float)lastArmorScore));
         }

         if (experienceTotal != lastExperienceScore) {
            lastExperienceScore = experienceTotal;
            updateScorePoints(IScoreCriteria::XP, MathHelper::ceil((float)lastExperienceScore));
         }

         if (experienceLevel != lastLevelScore) {
            lastLevelScore = experienceLevel;
            updateScorePoints(IScoreCriteria::LEVEL, MathHelper::ceil((float)lastLevelScore));
         }

         if (experienceTotal != lastExperience) {
            lastExperience = experienceTotal;
            connection.sendPacket(new SPacketSetExperience(experience, experienceTotal, experienceLevel));
         }

         if (ticksExisted % 20 == 0) {
            CriteriaTriggers.LOCATION.trigger(this);
         }

      } catch (std::exception& var4) {
         CrashReport crashreport = CrashReport::makeCrashReport(var4, "Ticking player");
         CrashReportCategory crashreportcategory = crashreport.makeCategory("Player being ticked");
         addEntityCrashInfo(crashreportcategory);
         throw new ReportedException(crashreport);
      }
}

void EntityPlayerMP::onDeath(DamageSource::DamageSource cause) {
    bool flag = world->getGameRules().getBoolean("showDeathMessages");
      connection.sendPacket(new SPacketCombatEvent(getCombatTracker(), SPacketCombatEvent::Event::ENTITY_DIED, flag));
      if (flag) {
         Team* team = getTeam();
         if (team != nullptr && team->getDeathMessageVisibility() != Team::EnumVisible::ALWAYS) {
            if (team->getDeathMessageVisibility() == Team::EnumVisible::HIDE_FOR_OTHER_TEAMS) {
               server->getPlayerList().sendMessageToAllTeamMembers(this, getCombatTracker().getDeathMessage());
            } else if (team->getDeathMessageVisibility() == Team::EnumVisible::HIDE_FOR_OWN_TEAM) {
               server->getPlayerList().sendMessageToTeamOrAllPlayers(this, getCombatTracker().getDeathMessage());
            }
         } else {
            server->getPlayerList().sendMessage(getCombatTracker().getDeathMessage());
         }
      }

      spawnShoulderEntities();
      if (!world->getGameRules().getBoolean("keepInventory") && !isSpectator()) {
         destroyVanishingCursedItems();
         inventory.dropAllItems();
      }

      auto var6 = world->getScoreboard().getObjectivesFromCriteria(IScoreCriteria::DEATH_COUNT);

    for (auto scoreobjective : var6){
         Score score = getWorldScoreboard().getOrCreateScore(getName(), scoreobjective);
         score.incrementScore();
      }

      EntityLivingBase* entitylivingbase = getAttackingEntity();
      if (entitylivingbase != nullptr) {
         EntityList::EntityEggInfo entitylist$entityegginfo = EntityList::ENTITY_EGGS.get(EntityList::getKey((Entity*)entitylivingbase));
         if (entitylist$entityegginfo != nullptr) {
            addStat(entitylist$entityegginfo.entityKilledByStat);
         }

         entitylivingbase->awardKillScore(this, scoreValue, cause);
      }

      addStat(&StatList::DEATHS);
      takeStat(&StatList::TIME_SINCE_DEATH);
      extinguish();
      setFlag(0, false);
      getCombatTracker().reset();
}

void EntityPlayerMP::awardKillScore(Entity *p_191956_1_, int32_t p_191956_2_, DamageSource::DamageSource p_191956_3_) {
    if (p_191956_1_ != this) {
         EntityPlayer::awardKillScore(p_191956_1_, p_191956_2_, p_191956_3_);
         addScore(p_191956_2_);
         auto collection = getWorldScoreboard().getObjectivesFromCriteria(IScoreCriteria::TOTAL_KILL_COUNT);
         if (Util::instanceof<EntityPlayer>(p_191956_1_)) {
            addStat(&StatList::PLAYER_KILLS);
            collection.addAll(getWorldScoreboard().getObjectivesFromCriteria(IScoreCriteria::PLAYER_KILL_COUNT));
         } else {
            addStat(&StatList::MOB_KILLS);
         }

         collection.addAll(awardTeamKillScores(p_191956_1_));
         Iterator var5 = collection.iterator();

         while(var5.hasNext()) {
            ScoreObjective scoreobjective = (ScoreObjective)var5.next();
            getWorldScoreboard().getOrCreateScore(getName(), scoreobjective).incrementScore();
         }

         CriteriaTriggers.PLAYER_KILLED_ENTITY.trigger(this, p_191956_1_, p_191956_3_);
      }
}

bool EntityPlayerMP::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
         return false;
      } else {
         bool flag = server->isDedicatedServer() && canPlayersAttack() && "fall" == (source.damageType);
         if (!flag && respawnInvulnerabilityTicks > 0 && source != DamageSource::OUT_OF_WORLD) {
            return false;
         } else {
            if (Util::instanceof<DamageSource::EntityDamageSource>(source)) {
               Entity* entity = source.getTrueSource();
               if (Util::instanceof<EntityPlayer>(entity) && !canAttackPlayer((EntityPlayer*)entity)) {
                  return false;
               }

               if (Util::instanceof<EntityArrow>(entity)) {
                  EntityArrow* entityarrow = (EntityArrow*)entity;
                  if (Util::instanceof<EntityPlayer>(entityarrow->shootingEntity) && !canAttackPlayer((EntityPlayer*)entityarrow->shootingEntity)) {
                     return false;
                  }
               }
            }

            return EntityPlayer::attackEntityFrom(source, amount);
         }
      }
}

bool EntityPlayerMP::canAttackPlayer(EntityPlayer *other) {
    return !canPlayersAttack() ? false : EntityPlayer::canAttackPlayer(other);
}

Entity * EntityPlayerMP::changeDimension(int32_t dimensionIn) {
    invulnerableDimensionChange = true;
      if (dimension == 0 && dimensionIn == -1) {
         enteredNetherPosition = Vec3d(posX, posY, posZ);
      } else if (dimension != -1 && dimensionIn != 0) {
         enteredNetherPosition = std::nullopt;
      }

      if (dimension == 1 && dimensionIn == 1) {
         world->removeEntity(this);
         if (!queuedEndExit) {
            queuedEndExit = true;
            connection.sendPacket(new SPacketChangeGameState(4, seenCredits ? 0.0F : 1.0F));
            seenCredits = true;
         }

         return this;
      } else {
         if (dimension == 0 && dimensionIn == 1) {
            dimensionIn = 1;
         }

         server->getPlayerList().changePlayerDimension(this, dimensionIn);
         connection.sendPacket(new SPacketEffect(1032, BlockPos::ORIGIN, 0, false));
         lastExperience = -1;
         lastHealth = -1.0F;
         lastFoodLevel = -1;
         return this;
      }
}

bool EntityPlayerMP::isSpectatedByPlayer(EntityPlayerMP *player) {
    if (player->isSpectator()) {
         return getSpectatingEntity() == this;
      } else {
         return isSpectator() ? false : EntityPlayer::isSpectatedByPlayer(player);
      }
}

void EntityPlayerMP::onItemPickup(Entity *entityIn, int32_t quantity) {
    EntityPlayer::onItemPickup(entityIn, quantity);
      openContainer->detectAndSendChanges();
}

EntityPlayer::SleepResult EntityPlayerMP::trySleep(const BlockPos &bedLocation) {
    EntityPlayer::SleepResult entityplayer$sleepresult = EntityPlayer::trySleep(bedLocation);
      if (entityplayer$sleepresult == EntityPlayer::SleepResult::OK) {
         addStat(&StatList::SLEEP_IN_BED);
         Packet* packet = new SPacketUseBed(this, bedLocation);
         getServerWorld().getEntityTracker().sendToTracking(this, packet);
         connection.setPlayerLocation(posX, posY, posZ, rotationYaw, rotationPitch);
         connection.sendPacket(packet);
         CriteriaTriggers::SLEPT_IN_BED.trigger(this);
      }

      return entityplayer$sleepresult;
}

void EntityPlayerMP::wakeUpPlayer(bool immediately, bool updateWorldFlag, bool setSpawn) {
    if (isPlayerSleeping()) {
         getServerWorld().getEntityTracker().sendToTrackingAndSelf(this, new SPacketAnimation(this, 2));
      }

      EntityPlayer::wakeUpPlayer(immediately, updateWorldFlag, setSpawn);
      if (connection != nullptr) {
         connection.setPlayerLocation(posX, posY, posZ, rotationYaw, rotationPitch);
      }
}

bool EntityPlayerMP::startRiding(Entity *entityIn, bool force) {
    Entity* entity = getRidingEntity();
      if (!EntityPlayer::startRiding(entityIn, force)) {
         return false;
      } else {
         Entity* entity1 = getRidingEntity();
         if (entity1 != entity && connection != nullptr) {
            connection.setPlayerLocation(posX, posY, posZ, rotationYaw, rotationPitch);
         }

         return true;
      }
}

void EntityPlayerMP::dismountRidingEntity() {
    Entity* entity = getRidingEntity();
      EntityPlayer::dismountRidingEntity();
      Entity* entity1 = getRidingEntity();
      if (entity1 != entity && connection != nullptr) {
         connection.setPlayerLocation(posX, posY, posZ, rotationYaw, rotationPitch);
      }
}

bool EntityPlayerMP::isEntityInvulnerable(DamageSource::DamageSource source) {
    return EntityPlayer::isEntityInvulnerable(source) || isInvulnerableDimensionChange();
}

void EntityPlayerMP::handleFalling(double y, bool onGroundIn) {
    auto i = MathHelper::floor(posX);
      auto j = MathHelper::floor(posY - 0.20000000298023224);
      auto k = MathHelper::floor(posZ);
      BlockPos blockpos = BlockPos(i, j, k);
      IBlockState* iblockstate = world->getBlockState(blockpos);
      if (iblockstate->getMaterial() == Material::AIR) {
         BlockPos blockpos1 = blockpos.down();
         IBlockState* iblockstate1 = world->getBlockState(blockpos1);
         Block* block = iblockstate1->getBlock();
         if (Util::instanceof<BlockFence>(block) || Util::instanceof<BlockWall>(block) || Util::instanceof<BlockFenceGate>(block)) {
            blockpos = blockpos1;
            iblockstate = iblockstate1;
         }
      }

      EntityPlayer::updateFallState(y, onGroundIn, iblockstate, blockpos);
}

void EntityPlayerMP::openEditSign(TileEntitySign *signTile) {
    signTile->setPlayer(this);
    connection.sendPacket(new SPacketSignEditorOpen(signTile->getPos()));
}

void EntityPlayerMP::displayGui(IInteractionObject *guiOwner) {
    if (Util::instanceof<ILootContainer>(guiOwner) && ((ILootContainer*)guiOwner)->getLootTable().has_value() && isSpectator()) {
         sendStatusMessage((new TextComponentTranslation("container.spectatorCantOpen", {})).setStyle((new Style())->setColor(TextFormatting::RED)), true);
      } else {
         getNextWindowId();
         connection.sendPacket(new SPacketOpenWindow(currentWindowId, guiOwner->getGuiID(), guiOwner->getDisplayName()));
         openContainer = guiOwner->createContainer(&inventory, this);
         openContainer->windowId = currentWindowId;
         openContainer->addListener(this);
      }
}

void EntityPlayerMP::displayGUIChest(IInventory *chestInventory) {
    if (Util::instanceof<ILootContainer>(chestInventory) && ((ILootContainer*)chestInventory)->getLootTable().has_value() && isSpectator()) {
         sendStatusMessage((new TextComponentTranslation("container.spectatorCantOpen", new Object[0])).setStyle((new Style())->setColor(TextFormatting::RED)), true);
      } else {
         if (openContainer != inventoryContainer) {
            closeScreen();
         }

         if (Util::instanceof<ILockableContainer>(chestInventory)) {
            ILockableContainer* ilockablecontainer = (ILockableContainer*)chestInventory;
            if (ilockablecontainer->isLocked() && !canOpen(ilockablecontainer->getLockCode()) && !isSpectator()) {
               connection.sendPacket(new SPacketChat(new TextComponentTranslation("container.isLocked", new Object[]{chestInventory->getDisplayName()}), ChatType::GAME_INFO));
               connection.sendPacket(new SPacketSoundEffect(SoundEvents::BLOCK_CHEST_LOCKED, SoundCategory::BLOCKS, posX, posY, posZ, 1.0F, 1.0F));
               return;
            }
         }

         getNextWindowId();
         if (Util::instanceof<IInteractionObject>(chestInventory)) {
            connection.sendPacket(new SPacketOpenWindow(currentWindowId, ((IInteractionObject*)chestInventory)->getGuiID(), chestInventory->getDisplayName(), chestInventory->getSizeInventory()));
            openContainer = ((IInteractionObject*)chestInventory)->createContainer(&inventory, this);
         } else {
            connection.sendPacket(new SPacketOpenWindow(currentWindowId, "minecraft:container", chestInventory->getDisplayName(), chestInventory->getSizeInventory()));
            openContainer = new ContainerChest(&inventory, chestInventory, this);
         }

         openContainer->windowId = currentWindowId;
         openContainer->addListener(this);
      }
}

void EntityPlayerMP::displayVillagerTradeGui(IMerchant *villager) {
    getNextWindowId();
      openContainer = new ContainerMerchant(inventory, villager, world);
      openContainer->windowId = currentWindowId;
      openContainer->addListener(this);
      IInventory* iinventory = ((ContainerMerchant*)openContainer)->getMerchantInventory();
      ITextComponent* itextcomponent = villager->getDisplayName();
      connection.sendPacket(new SPacketOpenWindow(currentWindowId, "minecraft:villager", itextcomponent, iinventory->getSizeInventory()));
      MerchantRecipeList merchantrecipelist = villager->getRecipes(this);
      if (merchantrecipelist != nullptr) {
         PacketBuffer* packetbuffer = new PacketBuffer(Unpooled.buffer());
         packetbuffer.writeInt(currentWindowId);
         merchantrecipelist.writeToBuf(packetbuffer);
         connection.sendPacket(new SPacketCustomPayload("MC|TrList", packetbuffer));
      }
}

void EntityPlayerMP::openGuiHorseInventory(AbstractHorse *horse, IInventory *inventoryIn) {
    if (openContainer != inventoryContainer) {
         closeScreen();
      }

      getNextWindowId();
      connection.sendPacket(new SPacketOpenWindow(currentWindowId, "EntityHorse", inventoryIn->getDisplayName(), inventoryIn->getSizeInventory(), horse->getEntityId()));
      openContainer = new ContainerHorseInventory(inventory, inventoryIn, horse, this);
      openContainer->windowId = currentWindowId;
      openContainer->addListener(this);
}

void EntityPlayerMP::openBook(ItemStack stack, EnumHand hand) {
    Item* item = stack.getItem();
      if (item == Items::WRITTEN_BOOK) {
         PacketBuffer* packetbuffer = new PacketBuffer(Unpooled.buffer());
         packetbuffer.writeEnumValue(hand);
         connection.sendPacket(new SPacketCustomPayload("MC|BOpen", packetbuffer));
      }
}

void EntityPlayerMP::displayGuiCommandBlock(TileEntityCommandBlock *commandBlock) {
    commandBlock->setSendToClient(true);
    sendTileEntityUpdate(commandBlock);
}

void EntityPlayerMP::sendSlotContents(Container *containerToSend, int32_t slotInd, ItemStack stack) {
    if (!(Util::instanceof<SlotCrafting>(containerToSend->getSlot(slotInd)))) {
         if (containerToSend == inventoryContainer) {
            CriteriaTriggers::INVENTORY_CHANGED.trigger(this, inventory);
         }

         if (!isChangingQuantityOnly) {
            connection.sendPacket(new SPacketSetSlot(containerToSend->windowId, slotInd, stack));
         }
      }
}

void EntityPlayerMP::sendContainerToPlayer(Container *containerIn) {
    sendAllContents(containerIn, containerIn->getInventory());
}

void EntityPlayerMP::sendAllContents(Container *containerToSend, std::vector<ItemStack> itemsList) {
    connection.sendPacket(new SPacketWindowItems(containerToSend->windowId, itemsList));
    connection.sendPacket(new SPacketSetSlot(-1, -1, inventory.getItemStack()));
}

void EntityPlayerMP::sendWindowProperty(Container *containerIn, int32_t varToUpdate, int32_t newValue) {
    connection.sendPacket(new SPacketWindowProperty(containerIn->windowId, varToUpdate, newValue));
}

void EntityPlayerMP::sendAllWindowProperties(Container *containerIn, IInventory *inventory) {
    for(auto i = 0; i < inventory->getFieldCount(); ++i) {
         connection.sendPacket(new SPacketWindowProperty(containerIn->windowId, i, inventory->getField(i)));
      }
}

void EntityPlayerMP::closeScreen() {
    connection.sendPacket(new SPacketCloseWindow(openContainer->windowId));
    closeContainer();
}

void EntityPlayerMP::updateHeldItem() {
    if (!isChangingQuantityOnly) {
         connection.sendPacket(new SPacketSetSlot(-1, -1, inventory.getItemStack()));
      }
}

void EntityPlayerMP::closeContainer() {
    openContainer->onContainerClosed(this);
    openContainer = inventoryContainer.get();
}

void EntityPlayerMP::setEntityActionState(float strafe, float forward, bool jumping, bool sneaking) {
    if (isRiding()) {
         if (strafe >= -1.0F && strafe <= 1.0F) {
            moveStrafing = strafe;
         }

         if (forward >= -1.0F && forward <= 1.0F) {
            moveForward = forward;
         }

         isJumping = jumping;
         setSneaking(sneaking);
      }
}

void EntityPlayerMP::addStat(StatBase *stat, int32_t amount) {
    if (stat != nullptr) {
         statsFile.increaseStat(this, stat, amount);
         auto var3 = getWorldScoreboard().getObjectivesFromCriteria(stat->getCriteria());

        for (auto scoreobjective : var3){
            getWorldScoreboard().getOrCreateScore(getName(), scoreobjective).increaseScore(amount);
         }
      }
}

void EntityPlayerMP::takeStat(StatBase *stat) {
    if (stat != nullptr) {
         statsFile.unlockAchievement(this, stat, 0);
         auto var2 = getWorldScoreboard().getObjectivesFromCriteria(stat->getCriteria()).iterator();

         for (auto scoreobjective : var2){
            getWorldScoreboard().getOrCreateScore(getName(), scoreobjective).setScorePoints(0);
         }
      }
}

void EntityPlayerMP::unlockRecipes(std::vector<IRecipe*> p_192021_1_) {
    recipeBook.add(p_192021_1_, this);
}

void EntityPlayerMP::unlockRecipes(std::vector<ResourceLocation> p_193102_1_) {
    std::vector<IRecipe*> list;
      ResourceLocation[] var3 = p_193102_1_;
      int var4 = p_193102_1_.length;

      for(int var5 = 0; var5 < var4; ++var5) {
         ResourceLocation resourcelocation = var3[var5];
         list.add(CraftingManager::getRecipe(resourcelocation));
      }

      unlockRecipes(list);
}

void EntityPlayerMP::onInsideBlock(IBlockState *p_191955_1_) {
    CriteriaTriggers::ENTER_BLOCK.trigger(this, p_191955_1_);
}

std::unique_ptr<CooldownTracker> EntityPlayerMP::createCooldownTracker() {
    return std::make_unique<CooldownTrackerServer>(this);
}

void EntityPlayerMP::updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos) {
}

void EntityPlayerMP::frostWalk(const BlockPos &pos) {
    if (!isSpectator()) {
        EntityPlayer::frostWalk(pos);
    }

}

void EntityPlayerMP::updateScorePoints(IScoreCriteria* criteria, int32_t points) const {
    auto var3 = getWorldScoreboard().getObjectivesFromCriteria(criteria);

    for (const auto scoreobjective : var3){
         Score score = getWorldScoreboard().getOrCreateScore(getName(), scoreobjective);
         score.setScorePoints(points);
      }
}

std::vector<ScoreObjective> EntityPlayerMP::awardTeamKillScores(Entity *p_192038_1_) {
    std::string s = p_192038_1_ instanceof EntityPlayer ? p_192038_1_.getName() : p_192038_1_.getCachedUniqueIdString();
      ScorePlayerTeam scoreplayerteam = getWorldScoreboard().getPlayersTeam(getName());
      if (scoreplayerteam != nullptr) {
         int i = scoreplayerteam.getColor().getColorIndex();
         if (i >= 0 && i < IScoreCriteria::KILLED_BY_TEAM.size()) {
            Iterator var5 = getWorldScoreboard().getObjectivesFromCriteria(IScoreCriteria::KILLED_BY_TEAM[i]).iterator();

            while(var5.hasNext()) {
               ScoreObjective scoreobjective = (ScoreObjective)var5.next();
               Score score = getWorldScoreboard().getOrCreateScore(s, scoreobjective);
               score.incrementScore();
            }
         }
      }

      ScorePlayerTeam scoreplayerteam1 = getWorldScoreboard().getPlayersTeam(s);
      if (scoreplayerteam1 != nullptr) {
         int j = scoreplayerteam1.getColor().getColorIndex();
         if (j >= 0 && j < IScoreCriteria::TEAM_KILL.size()) {
            return getWorldScoreboard().getObjectivesFromCriteria(IScoreCriteria::TEAM_KILL[j]);
         }
      }

      return {};
}

bool EntityPlayerMP::canPlayersAttack() const {
    return server->isPVPEnabled();
}

void EntityPlayerMP::sendTileEntityUpdate(TileEntity *p_147097_1_) {
    if (p_147097_1_ != nullptr) {
         auto spacketupdatetileentity = p_147097_1_.getUpdatePacket();
         if (spacketupdatetileentity.has_value()) {
            connection.sendPacket(spacketupdatetileentity);
         }
      }
}

void EntityPlayerMP::getNextWindowId() {
    currentWindowId = currentWindowId % 100 + 1;
}

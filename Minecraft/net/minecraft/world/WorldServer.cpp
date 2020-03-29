#include "WorldServer.h"
#include "NextTickListEntry.h"
#include "../util/ReportedException.h"
#include "GameType.h"

std::shared_ptr<spdlog::logger> WorldServer::LOGGER = spdlog::get("Minecraft")->clone("WorldServer");

WorldServer::WorldServer(MinecraftServer* server, ISaveHandler* saveHandlerIn, WorldInfo& info, int dimensionId, Profiler& profilerIn)
	:World(saveHandlerIn, info, DimensionType::getById(dimensionId).createDimension(), profilerIn, false), server(server), entityTracker(this),
	playerChunkMap(this), chunkProvider(createChunkProvider()), worldTeleporter(this)
{
	provider.setWorld(this);
	calculateInitialSkylight();
	calculateInitialWeather();
	getWorldBorder().setSize(server->getMaxWorldSize());
}

World* WorldServer::init()
{
	mapStorage = MapStorage(saveHandler);
	std::string s = VillageCollection.fileNameForProvider(provider);
	auto villagecollection = (VillageCollection)mapStorage.getOrLoadData(VillageCollection.class, s);
	if (villagecollection == nullptr) 
	{
		villageCollection = VillageCollection(this);
		mapStorage.setData(s, villageCollection);
	}
	else 
	{
		villageCollection = villagecollection;
		villageCollection.setWorldsForAll(this);
	}

	worldScoreboard = ServerScoreboard(server);
	auto scoreboardsavedata = (ScoreboardSaveData)mapStorage.getOrLoadData(ScoreboardSaveData.class, "scoreboard");
	if (scoreboardsavedata == nullptr) 
	{
		scoreboardsavedata = ScoreboardSaveData();
		mapStorage.setData("scoreboard", scoreboardsavedata);
	}

	scoreboardsavedata.setScoreboard(worldScoreboard);
	((ServerScoreboard)worldScoreboard).addDirtyRunnable(WorldSavedDataCallableSave(scoreboardsavedata));
	lootTable = LootTableManager(new File(new File(saveHandler.getWorldDirectory(), "data"), "loot_tables"));
	advancementManager = AdvancementManager(new File(new File(saveHandler.getWorldDirectory(), "data"), "advancements"));
	functionManager = FunctionManager(new File(new File(saveHandler.getWorldDirectory(), "data"), "functions"), server);
	getWorldBorder().setCenter(worldInfo.getBorderCenterX(), worldInfo.getBorderCenterZ());
	getWorldBorder().setDamageAmount(worldInfo.getBorderDamagePerBlock());
	getWorldBorder().setDamageBuffer(worldInfo.getBorderSafeZone());
	getWorldBorder().setWarningDistance(worldInfo.getBorderWarningDistance());
	getWorldBorder().setWarningTime(worldInfo.getBorderWarningTime());
	if (worldInfo.getBorderLerpTime() > 0L) 
	{
		getWorldBorder().setTransition(worldInfo.getBorderSize(), worldInfo.getBorderLerpTarget(), worldInfo.getBorderLerpTime());
	}
	else 
	{
		getWorldBorder().setTransition(worldInfo.getBorderSize());
	}

	return this;
}

void WorldServer::tick()
{
	World::tick();
	if (getWorldInfo().isHardcoreModeEnabled() && getDifficulty() != EnumDifficulty::HARD) 
	{
		getWorldInfo().setDifficulty(EnumDifficulty::HARD);
	}

	provider.getBiomeProvider().cleanupCache();
	if (areAllPlayersAsleep()) {
		if (getGameRules().getBoolean("doDaylightCycle")) 
		{
			auto i = worldInfo.getWorldTime() + 24000;
			worldInfo.setWorldTime(i - i % 24000);
		}

		wakeAllPlayers();
	}

	profiler.startSection("mobSpawner");
	if (getGameRules().getBoolean("doMobSpawning") && worldInfo.getTerrainType() != WorldType.DEBUG_ALL_BLOCK_STATES) 
	{
		entitySpawner.findChunksForSpawning(this, spawnHostileMobs, spawnPeacefulMobs, worldInfo.getWorldTotalTime() % 400 == 0);
	}

	profiler.endStartSection("chunkSource");
	chunkProvider.tick();
	auto j = calculateSkylightSubtracted(1.0F);
	if (j != getSkylightSubtracted()) 
	{
		setSkylightSubtracted(j);
	}

	worldInfo.setWorldTotalTime(worldInfo.getWorldTotalTime() + 1);
	if (getGameRules().getBoolean("doDaylightCycle")) 
	{
		worldInfo.setWorldTime(worldInfo.getWorldTime() + 1);
	}

	profiler.endStartSection("tickPending");
	tickUpdates(false);
	profiler.endStartSection("tickBlocks");
	updateBlocks();
	profiler.endStartSection("chunkMap");
	playerChunkMap.tick();
	profiler.endStartSection("village");
	villageCollection.tick();
	villageSiege.tick();
	profiler.endStartSection("portalForcer");
	worldTeleporter.removeStalePortalLocations(getTotalWorldTime());
	profiler.endSection();
	sendQueuedBlockEvents();
}

std::optional<SpawnListEntry> WorldServer::getSpawnListEntryForTypeAt(EnumCreatureType creatureType, BlockPos& pos)
{
	auto list = getChunkProvider().getPossibleCreatures(creatureType, pos);
	return list != nullptr && !list.isEmpty() ? (Biome.SpawnListEntry)WeightedRandom.getRandomItem(rand, list) : std::nullopt;
}

bool WorldServer::canCreatureTypeSpawnHere(EnumCreatureType creatureType, SpawnListEntry spawnListEntry, BlockPos& pos)
{
	auto list = getChunkProvider().getPossibleCreatures(creatureType, pos);
	return list != nullptr && !list.isEmpty() ? list.contains(spawnListEntry) : false;
}

void WorldServer::updateAllPlayersSleepingFlag()
{
	allPlayersSleeping = false;
	if (!playerEntities.empty()) 
	{
		auto i = 0;
		auto j = 0;

		for (auto entityplayer : playerEntities)
		{
			if (entityplayer->isSpectator()) 
			{
				++i;
			}
			else if (entityplayer->isPlayerSleeping()) 
			{
				++j;
			}
		}

		allPlayersSleeping = j > 0 && j >= playerEntities.size() - i;
	}
}

bool WorldServer::areAllPlayersAsleep()
{
	if (allPlayersSleeping && !isRemote) 
	{
		auto entityplayer = playerEntities.begin();
		do 
		{
			if (entityplayer == playerEntities.end())
			{
				return true;
			}
		} 
		while (entityplayer->isSpectator() || entityplayer->isPlayerFullyAsleep());

		return false;
	}
	else 
	{
		return false;
	}
}

bool WorldServer::isBlockTickPending(BlockPos& pos, Block* blockType) const
{
	NextTickListEntry nextticklistentry(pos, blockType);
	auto ite =std::find_if(pendingTickListEntriesThisTick.begin(), pendingTickListEntriesThisTick.end(), [&](const NextTickListEntry list) {return nextticklistentry == list; });
	return ite != pendingTickListEntriesThisTick.end();
}

bool WorldServer::isUpdateScheduled(BlockPos& pos, Block* blk)
{
	NextTickListEntry nextticklistentry(pos, blk);
	return pendingTickListEntriesHashSet.find(nextticklistentry) != pendingTickListEntriesHashSet.end();
}

void WorldServer::scheduleUpdate(BlockPos& pos, Block* blockIn, int32_t delay)
{
	updateBlockTick(pos, blockIn, delay, 0);
}

void WorldServer::updateBlockTick(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority)
{
	Material material = blockIn.getDefaultState().getMaterial();
	if (scheduledUpdatesAreImmediate && material != Material::AIR) 
	{
		if (blockIn.requiresUpdates()) {
			if (isAreaLoaded(pos.add(-8, -8, -8), pos.add(8, 8, 8))) 
			{
				auto iblockstate = getBlockState(pos);
				if (iblockstate.getMaterial() != Material.AIR && iblockstate.getBlock() == blockIn) {
					iblockstate.getBlock().updateTick(this, pos, iblockstate, rand);
				}
			}

			return;
		}

		delay = 1;
	}

	NextTickListEntry nextticklistentry(pos, blockIn);
	if (isBlockLoaded(pos)) {
		if (material != Material::AIR) 
		{
			nextticklistentry.setScheduledTime(delay + worldInfo.getWorldTotalTime());
			nextticklistentry.setPriority(priority);
		}

		if (pendingTickListEntriesHashSet.find(nextticklistentry) == pendingTickListEntriesHashSet.end())
		{
			pendingTickListEntriesHashSet.emplace(nextticklistentry);
			pendingTickListEntriesTreeSet.emplace(nextticklistentry);
		}
	}
}

void WorldServer::scheduleBlockUpdate(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority)
{
	NextTickListEntry nextticklistentry(pos, blockIn);
	nextticklistentry.setPriority(priority);
	Material material = blockIn.getDefaultState().getMaterial();
	if (material != Material::AIR) 
	{
		nextticklistentry.setScheduledTime(delay + worldInfo.getWorldTotalTime());
	}

	if (pendingTickListEntriesHashSet.find(nextticklistentry) == pendingTickListEntriesHashSet.end())
	{
		pendingTickListEntriesHashSet.emplace(nextticklistentry);
		pendingTickListEntriesTreeSet.emplace(nextticklistentry);
	}
}

void WorldServer::updateEntities()
{
	if (playerEntities.empty()) 
	{
		if (updateEntityTick++ >= 300) 
		{
			return;
		}
	}
	else 
	{
		resetUpdateEntityTick();
	}

	provider.onWorldUpdateEntities();
	World::updateEntities();
}

void WorldServer::resetUpdateEntityTick()
{
	updateEntityTick = 0;
}

bool WorldServer::tickUpdates(bool runAllPending)
{
	if (worldInfo.getTerrainType() == WorldType.DEBUG_ALL_BLOCK_STATES) 
	{
		return false;
	}
	else 
	{
		auto i = pendingTickListEntriesTreeSet.size();
		if (i != pendingTickListEntriesHashSet.size()) 
		{
			throw std::logic_error("TickNextTick list out of synch");
		}
		else 
		{
			if (i > 65536) 
			{
				i = 65536;
			}

			profiler.startSection("cleaning");

			NextTickListEntry ;
			for(auto nextticklistentry1 : pendingTickListEntriesTreeSet)
			{
				if (!runAllPending && nextticklistentry1.scheduledTime > worldInfo.getWorldTotalTime()) 
				{
					break;
				}

				pendingTickListEntriesTreeSet.erase(nextticklistentry1);
				pendingTickListEntriesHashSet.erase(nextticklistentry1);
				pendingTickListEntriesThisTick.emplace_back(nextticklistentry1);
			}

			profiler.endSection();
			profiler.startSection("ticking");

			for (auto nextticklistentry1 : pendingTickListEntriesThisTick)
			{
				nextticklistentry1 = (NextTickListEntry)iterator.next();
				iterator.remove();
				if (isAreaLoaded(nextticklistentry1.position.add(0, 0, 0), nextticklistentry1.position.add(0, 0, 0))) 
				{
					auto iblockstate = getBlockState(nextticklistentry1.position);
					if (iblockstate.getMaterial() != Material::AIR && Block.isEqualTo(iblockstate.getBlock(), nextticklistentry1.getBlock())) {
						try 
						{
							iblockstate.getBlock().updateTick(this, nextticklistentry1.position, iblockstate, this.rand);
						}
						catch (Throwable var10) 
						{
							CrashReport crashreport = CrashReport.makeCrashReport(var10, "Exception while ticking a block");
							CrashReportCategory crashreportcategory = crashreport.makeCategory("Block being ticked");
							CrashReportCategory.addBlockInfo(crashreportcategory, nextticklistentry1.position, iblockstate);
							throw ReportedException(crashreport);
						}
					}
				}
				else 
				{
					scheduleUpdate(nextticklistentry1.position, nextticklistentry1.getBlock(), 0);
				}
			}

			profiler.endSection();
			pendingTickListEntriesThisTick.clear();
			return !pendingTickListEntriesTreeSet.empty();
		}
	}
}

void WorldServer::setInitialSpawnLocation()
{
	if (worldInfo.getSpawnY() <= 0) 
	{
		worldInfo.setSpawnY(getSeaLevel() + 1);
	}

	auto i = worldInfo.getSpawnX();
	auto j = worldInfo.getSpawnZ();
	auto k = 0;

	while (getGroundAboveSeaLevel(BlockPos(i, 0, j)).getMaterial() == Material.AIR) {
		i += rand(8) - rand(8);
		j += rand(8) - rand(8);
		++k;
		if (k == 10000) 
		{
			break;
		}
	}

	worldInfo.setSpawnX(i);
	worldInfo.setSpawnZ(j);
}

void WorldServer::wakeAllPlayers()
{
	allPlayersSleeping = false;

	std::vector<EntityPlayer*> plysleep;
	std::copy_if(playerEntities.begin(), playerEntities.end(), std::back_inserter(plysleep), [](const EntityPlayer * a) {return a->isPlayerSleeping(); });

	for (auto entityplayer : plysleep)
	{
		entityplayer->wakeUpPlayer(false, false, true);
	}

	if (getGameRules().getBoolean("doWeatherCycle")) 
	{
		resetRainAndThunder();
	}
}

bool WorldServer::isChunkLoaded(int32_t x, int32_t z, bool allowEmpty)
{
	return getChunkProvider().chunkExists(x, z);
}

void WorldServer::playerCheckLight()
{
	profiler.startSection("playerCheckLight");
	if (!playerEntities.empty()) 
	{
		int i = rand(playerEntities.size());
		auto entityplayer = playerEntities[i];
		int j = MathHelper::floor(entityplayer.posX) + rand(11) - 5;
		int k = MathHelper::floor(entityplayer.posY) + rand(11) - 5;
		int l = MathHelper::floor(entityplayer.posZ) + rand(11) - 5;
		BlockPos pos(j, k, l);
		checkLight(pos);
	}

	profiler.endSection();
}

void WorldServer::updateBlocks()
{
	playerCheckLight();
	if (worldInfo.getTerrainType() == WorldType.DEBUG_ALL_BLOCK_STATES) 
	{
		for (auto chunk : playerChunkMap)
		{
			chunk.onTick(false);
		}
	}
	else 
	{
		auto i = getGameRules().getInt("randomTickSpeed");
		auto flag = isRaining();
		auto flag1 = isThundering();
		profiler.startSection("pollingChunks");


		for (Iterator iterator = playerChunkMap.getChunkIterator(); iterator.hasNext(); profiler.endSection()) 
		{
			profiler.startSection("getChunk");
			Chunk chunk = (Chunk)iterator.next();
			auto j = chunk.x * 16;
			auto k = chunk.z * 16;
			profiler.endStartSection("checkNextLight");
			chunk.enqueueRelightChecks();
			profiler.endStartSection("tickChunk");
			chunk.onTick(false);
			profiler.endStartSection("thunder");
			int j2;
			BlockPos blockpos1;
			if (flag && flag1 && rand(100000) == 0) 
			{
				updateLCG = updateLCG * 3 + 1013904223;
				j2 = updateLCG >> 2;
				BlockPos pos(j + (j2 & 15), 0, k + (j2 >> 8 & 15));
				blockpos1 = adjustPosToNearbyEntity(pos);
				if (isRainingAt(blockpos1)) 
				{
					DifficultyInstance difficultyinstance = getDifficultyForLocation(blockpos1);
					if (getGameRules().getBoolean("doMobSpawning") && rand.nextDouble() < (double)difficultyinstance.getAdditionalDifficulty() * 0.01D) {
						EntitySkeletonHorse entityskeletonhorse = new EntitySkeletonHorse(this);
						entityskeletonhorse.setTrap(true);
						entityskeletonhorse.setGrowingAge(0);
						entityskeletonhorse.setPosition((double)blockpos1.getX(), (double)blockpos1.getY(), (double)blockpos1.getZ());
						spawnEntity(entityskeletonhorse);
						addWeatherEffect(new EntityLightningBolt(this, (double)blockpos1.getX(), (double)blockpos1.getY(), (double)blockpos1.getZ(), true));
					}
					else 
					{
						addWeatherEffect(new EntityLightningBolt(this, (double)blockpos1.getX(), (double)blockpos1.getY(), (double)blockpos1.getZ(), false));
					}
				}
			}

			profiler.endStartSection("iceandsnow");
			if (rand(16) == 0) 
			{
				updateLCG = updateLCG * 3 + 1013904223;
				j2 = updateLCG >> 2;
				BlockPos pos(j + (j2 & 15), 0, k + (j2 >> 8 & 15));
				blockpos1 = getPrecipitationHeight(pos);
				BlockPos blockpos2 = blockpos1.down();
				if (canBlockFreezeNoWater(blockpos2)) 
				{
					setBlockState(blockpos2, Blocks.ICE.getDefaultState());
				}

				if (flag && canSnowAt(blockpos1, true)) 
				{
					setBlockState(blockpos1, Blocks.SNOW_LAYER.getDefaultState());
				}

				if (flag && getBiome(blockpos2).canRain()) 
				{
					getBlockState(blockpos2).getBlock().fillWithRain(this, blockpos2);
				}
			}

			profiler.endStartSection("tickBlocks");
			if (i > 0) 
			{
				ExtendedBlockStorage[] var20 = chunk.getBlockStorageArray();
				int var21 = var20.length;

				for (int var23 = 0; var23 < var21; ++var23) {
					ExtendedBlockStorage extendedblockstorage = var20[var23];
					if (extendedblockstorage != Chunk.NULL_BLOCK_STORAGE && extendedblockstorage.needsRandomTick()) {
						for (int i1 = 0; i1 < i; ++i1) {
							updateLCG = updateLCG * 3 + 1013904223;
							int j1 = updateLCG >> 2;
							int k1 = j1 & 15;
							int l1 = j1 >> 8 & 15;
							int i2 = j1 >> 16 & 15;
							auto iblockstate = extendedblockstorage.get(k1, i2, l1);
							Block block = iblockstate.getBlock();
							profiler.startSection("randomTick");
							if (block.getTickRandomly()) 
							{
								BlockPos pos(k1 + j, i2 + extendedblockstorage.getYLocation(), l1 + k)
								block.randomTick(this, pos, iblockstate, rand);
							}

							profiler.endSection();
						}
					}
				}
			}
		}

		profiler.endSection();
	}
}

BlockPos WorldServer::adjustPosToNearbyEntity(BlockPos& pos)
{
	BlockPos blockpos = getPrecipitationHeight(pos);
	AxisAlignedBB axisalignedbb = (AxisAlignedBB(blockpos, BlockPos(blockpos.getx(), getHeight(), blockpos.getz()))).grow(3.0);
	auto list = getEntitiesWithinAABB<EntityLivingBase>(axisalignedbb, [&](EntityLivingBase* p_apply_1_)
	{
		return p_apply_1_ != nullptr && p_apply_1_->isEntityAlive() && canSeeSky(p_apply_1_->getPosition());
	});
	if (!list.empty()) 
	{
		return list[rand(list.size())].getPosition();
	}
	else 
	{
		if (blockpos.gety() == -1) 
		{
			blockpos = blockpos.up(2);
		}

		return blockpos;
	}
}

void WorldServer::tickPlayers()
{
	World::tickPlayers();
	profiler.endStartSection("players");

	for (auto entity : playerEntities)
	{
		auto entity1 = entity->getRidingEntity();
		if (entity1 != nullptr) 
		{
			if (!entity1.isDead && entity1.isPassenger(entity)) 
			{
				continue;
			}

			entity->dismountRidingEntity();
		}

		profiler.startSection("tick");
		if (!entity->isDead) 
		{
			try {
				updateEntity(entity);
			}
			catch (Throwable var7) {
				CrashReport crashreport = CrashReport.makeCrashReport(var7, "Ticking player");
				CrashReportCategory crashreportcategory = crashreport.makeCategory("Player being ticked");
				entity->addEntityCrashInfo(crashreportcategory);
				throw ReportedException(crashreport);
			}
		}

		profiler.endSection();
		profiler.startSection("remove");
		if (entity->isDead) 
		{
			auto j = entity->chunkCoordX;
			auto k = entity->chunkCoordZ;
			if (entity->addedToChunk && isChunkLoaded(j, k, true)) 
			{
				getChunk(j, k).removeEntity(entity);
			}

			loadedEntityList.erase(std::remove_if(loadedEntityList.begin(), loadedEntityList.end(), [](const Entity* lhs) { return lhs == entity; }));
			onEntityRemoved(entity);
		}

		profiler.endSection();
	}
}

void WorldServer::resetRainAndThunder()
{
	worldInfo.setRainTime(0);
	worldInfo.setRaining(false);
	worldInfo.setThunderTime(0);
	worldInfo.setThundering(false);
}

bool WorldServer::canSpawnNPCs()
{
	return server.getCanSpawnNPCs();
}

bool WorldServer::canSpawnAnimals()
{
	return server.getCanSpawnAnimals();
}

void WorldServer::setDebugWorldSettings()
{
	worldInfo.setMapFeaturesEnabled(false);
	worldInfo.setAllowCommands(true);
	worldInfo.setRaining(false);
	worldInfo.setThundering(false);
	worldInfo.setCleanWeatherTime(1000000000);
	worldInfo.setWorldTime(6000L);
	worldInfo.setGameType(GameType::SPECTATOR);
	worldInfo.setHardcore(false);
	worldInfo.setDifficulty(EnumDifficulty::PEACEFUL);
	worldInfo.setDifficultyLocked(true);
	getGameRules().setOrCreateGameRule("doDaylightCycle", "false");
}

List WorldServer::getPendingBlockUpdates(Chunk& chunkIn, bool remove)
{
	ChunkPos chunkpos = chunkIn.getPos();
	auto i = (chunkpos.getx() << 4) - 2;
	auto j = i + 16 + 2;
	auto k = (chunkpos.getz() << 4) - 2;
	auto l = k + 16 + 2;
	return getPendingBlockUpdates(StructureBoundingBox(i, 0, k, j, 256, l), remove);
}

void WorldServer::updateEntityWithOptionalForce(Entity* entityIn, bool forceUpdate)
{
	if (!canSpawnAnimals() && (Util::instanceof <EntityAnimal>(entityIn) || Util::instanceof< EntityWaterMob>(entityIn)))
	{
		entityIn.setDead();
	}

	if (!canSpawnNPCs() && Util::instanceof<INpc>(entityIn))
	{
		entityIn.setDead();
	}

	World::updateEntityWithOptionalForce(entityIn, forceUpdate);
}

bool WorldServer::isBlockModifiable(EntityPlayer* player, BlockPos& pos)
{
	return !server.isBlockProtected(this, pos, player) && getWorldBorder().contains(pos);
}

List WorldServer::getPendingBlockUpdates(StructureBoundingBox& structureBB, bool remove)
{
	List list;

	for (int i = 0; i < 2; ++i)
	{
		Iterator iterator;
		if (i == 0) 
		{
			iterator = pendingTickListEntriesTreeSet.iterator();
		}
		else 
		{
			iterator = pendingTickListEntriesThisTick.iterator();
		}


		while (iterator.hasNext()) 
		{
			NextTickListEntry nextticklistentry = (NextTickListEntry)iterator.next();
			BlockPos blockpos = nextticklistentry.position;
			if (blockpos.getx() >= structureBB.minX && blockpos.getx() < structureBB.maxX && blockpos.getz() >= structureBB.minZ && blockpos.getz() < structureBB.maxZ) 
			{
				if (remove) 
				{
					if (i == 0) 
					{
						pendingTickListEntriesHashSet.remove(nextticklistentry);
					}

					iterator.remove();
				}

				if (list == null) 
				{
					list = Lists.newArrayList();
				}

				list.add(nextticklistentry);
			}
		}
	}

	return list;
}

IChunkProvider WorldServer::createChunkProvider()
{
	IChunkLoader ichunkloader = saveHandler.getChunkLoader(provider);
	return ChunkProviderServer(this, ichunkloader, provider.createChunkGenerator());
}

void WorldServer::createBonusChest()
{
	WorldGeneratorBonusChest worldgeneratorbonuschest;

	for (auto i = 0; i < 10; ++i) 
	{
		auto j = worldInfo.getSpawnX() + rand(6) - rand(6);
		auto k = worldInfo.getSpawnZ() + rand(6) - rand(6);
		BlockPos pos(j, 0, k);
		BlockPos blockpos = getTopSolidOrLiquidBlock(pos).up();
		if (worldgeneratorbonuschest.generate(this, rand, blockpos)) 
		{
			break;
		}
	}
}

BlockPos WorldServer::getSpawnCoordinate()
{
	return provider.getSpawnCoordinate();
}

void WorldServer::saveAllChunks(bool all, IProgressUpdate* progressCallback)
{
	auto chunkproviderserver = getChunkProvider();
	if (chunkproviderserver.canSave()) 
	{
		if (progressCallback != null) 
		{
			progressCallback.displaySavingString("Saving level");
		}

		saveLevel();
		if (progressCallback != nullptr) 
		{
			progressCallback.displayLoadingString("Saving chunks");
		}

		chunkproviderserver.saveChunks(all);
		Iterator var4 = Lists.newArrayList(chunkproviderserver.getLoadedChunks()).iterator();

		while (var4.hasNext()) 
		{
			auto chunk = (Chunk)var4.next();
			if (chunk != null && !playerChunkMap.contains(chunk.x, chunk.z)) 
			{
				chunkproviderserver.queueUnload(chunk);
			}
		}
	}
}

void WorldServer::flushToDisk()
{
	auto chunkproviderserver = getChunkProvider();
	if (chunkproviderserver.canSave()) 
	{
		chunkproviderserver.flushToDisk();
	}
}

void WorldServer::saveLevel()
{
	checkSessionLock();

	for (auto worldserver : server.worlds)
	{
		if (Util::instanceof<WorldServerMulti>(worldserver))
		{
			((WorldServerMulti)worldserver).saveAdditionalData();
		}
	}

	worldInfo.setBorderSize(getWorldBorder().getDiameter());
	worldInfo.getBorderCenterX(getWorldBorder().getCenterX());
	worldInfo.getBorderCenterZ(getWorldBorder().getCenterZ());
	worldInfo.setBorderSafeZone(getWorldBorder().getDamageBuffer());
	worldInfo.setBorderDamagePerBlock(getWorldBorder().getDamageAmount());
	worldInfo.setBorderWarningDistance(getWorldBorder().getWarningDistance());
	worldInfo.setBorderWarningTime(getWorldBorder().getWarningTime());
	worldInfo.setBorderLerpTarget(getWorldBorder().getTargetSize());
	worldInfo.setBorderLerpTime(getWorldBorder().getTimeUntilTarget());
	saveHandler.saveWorldInfoWithPlayer(worldInfo, server.getPlayerList().getHostPlayerData());
	mapStorage.saveAllData();
}

void WorldServer::onEntityAdded(Entity* entityIn)
{
	World::onEntityAdded(entityIn);
	entitiesById.emplace(entityIn.getEntityId(), entityIn);
	entitiesByUuid.emplace(entityIn.getUniqueID(), entityIn);
	auto aentity = entityIn.getParts();
	if (!aentity.empty()) 
	{
		Entity[] var3 = aentity;
		int var4 = aentity.size();

		for (auto var5 = 0; var5 < var4; ++var5) 
		{
			Entity entity = var3[var5];
			entitiesById.emplace(entity->getEntityId(), entity);
		}
	}
}

void WorldServer::onEntityRemoved(Entity* entityIn)
{
	World::onEntityRemoved(entityIn);
	entitiesById.removeObject(entityIn.getEntityId());
	entitiesByUuid.remove(entityIn.getUniqueID());
	Entity[] aentity = entityIn.getParts();
	if (!aentity.empty()) 
	{
		Entity[] var3 = aentity;
		int var4 = aentity.length;

		for (int var5 = 0; var5 < var4; ++var5) 
		{
			Entity entity = var3[var5];
			entitiesById.removeObject(entity->getEntityId());
		}
	}
}

void WorldServer::updateWeather()
{
	bool flag = isRaining();
	World::updateWeather();
	if (prevRainingStrength != rainingStrength) 
	{
		server.getPlayerList().sendPacketToAllPlayersInDimension(SPacketChangeGameState(7, rainingStrength), provider.getDimensionType().getId());
	}

	if (prevThunderingStrength != thunderingStrength) 
	{
		server.getPlayerList().sendPacketToAllPlayersInDimension(SPacketChangeGameState(8, thunderingStrength), provider.getDimensionType().getId());
	}

	if (flag != isRaining()) 
	{
		if (flag) {
			server.getPlayerList().sendPacketToAllPlayers(SPacketChangeGameState(2, 0.0F));
		}
		else {
			server.getPlayerList().sendPacketToAllPlayers(SPacketChangeGameState(1, 0.0F));
		}

		server.getPlayerList().sendPacketToAllPlayers(SPacketChangeGameState(7, rainingStrength));
		server.getPlayerList().sendPacketToAllPlayers(SPacketChangeGameState(8, thunderingStrength));
	}
}

void WorldServer::initialize(WorldSettings& settings)
{
	if (!worldInfo.isInitialized()) 
	{
		try 
		{
			createSpawnPosition(settings);
			if (worldInfo.getTerrainType() == WorldType.DEBUG_ALL_BLOCK_STATES) 
			{
				setDebugWorldSettings();
			}

			World::initialize(settings);
		}
		catch (Throwable var6) 
		{
			CrashReport crashreport = CrashReport.makeCrashReport(var6, "Exception initializing level");

			try 
			{
				addWorldInfoToCrashReport(crashreport);
			}
			catch (Throwable var5) 
			{
			}

			throw ReportedException(crashreport);
		}

		worldInfo.setServerInitialized(true);
	}
}

void WorldServer::loadEntities(std::initializer_list<Entity*> entityCollection)
{
	for(auto entity : entityCollection)
	{
		if (canAddEntity(entity)) 
		{
			loadedEntityList.emplace_back(entity);
			onEntityAdded(entity);
		}
	}
}

bool WorldServer::canAddEntity(Entity* entityIn)
{
	if (entityIn.isDead) 
	{
		LOGGER->warn("Tried to add entity {} but it was marked as removed already", EntityList.getKey(entityIn));
		return false;
	}
	else 
	{
		auto uuid = entityIn.getUniqueID();
		auto ite = entitiesByUuid.find(uuid);
		if (ite != entitiesByUuid.end())
		{
			auto entity = ite->second;

			auto findentity = std::find_if(unloadedEntityList.begin(), unloadedEntityList.end(), [&](const Entity * lhs) {return lhs == entity});
			if (findentity != unloadedEntityList.end())
			{
				unloadedEntityList.erase(findentity);
			}
			else {
				if (!(Util::instanceof<EntityPlayer>(entityIn)))
				{
					LOGGER->warn("Keeping entity {} that already exists with UUID {}", EntityList.getKey(entity), uuid.toString());
					return false;
				}

				LOGGER->warn("Force-added player with duplicate UUID {}", uuid.toString());
			}

			removeEntityDangerously(entity);
		}

		return true;
	}
}

bool WorldServer::addWeatherEffect(Entity* entityIn)
{
	if (World::addWeatherEffect(entityIn)) 
	{
		server.getPlayerList().sendToAllNearExcept(nullptr, entityIn.posX, entityIn.posY, entityIn.posZ, 512.0, provider.getDimensionType().getId(), SPacketSpawnGlobalEntity(entityIn));
		return true;
	}
	else 
	{
		return false;
	}
}

void WorldServer::setEntityState(Entity* entityIn, std::byte state)
{
	getEntityTracker().sendToTrackingAndSelf(entityIn, SPacketEntityStatus(entityIn, state));
}

void WorldServer::createSpawnPosition(WorldSettings& settings)
{
	if (!provider.canRespawnHere()) 
	{
		worldInfo.setSpawn(BlockPos::ORIGIN.up(provider.getAverageGroundLevel()));
	}
	else if (worldInfo.getTerrainType() == WorldType.DEBUG_ALL_BLOCK_STATES) 
	{
		worldInfo.setSpawn(BlockPos::ORIGIN.up());
	}
	else 
	{
		findingSpawnPoint = true;
		BiomeProvider biomeprovider = provider.getBiomeProvider();
		List list = biomeprovider.getBiomesToSpawnIn();
		pcg32 random(getSeed());
		BlockPos blockpos = biomeprovider.findBiomePosition(0, 0, 256, list, random);
		auto i = 8;
		auto j = provider.getAverageGroundLevel();
		auto k = 8;
		if (blockpos) 
		{
			i = blockpos.getx();
			k = blockpos.getz();
		}
		else 
		{
			LOGGER->warn("Unable to find spawn biome");
		}

		auto l = 0;

		while (!provider.canCoordinateBeSpawn(i, k)) 
		{
			i += random(64) - random(64);
			k += random(64) - random(64);
			++l;
			if (l == 1000) 
			{
				break;
			}
		}

		worldInfo.setSpawn(BlockPos(i, j, k));
		findingSpawnPoint = false;
		if (settings.isBonusChestEnabled()) 
		{
			createBonusChest();
		}
	}
}

void WorldServer::sendQueuedBlockEvents()
{
	while (!blockEventQueue[blockEventCacheIndex].isEmpty()) 
	{
		auto i = blockEventCacheIndex;
		blockEventCacheIndex ^= 1;
		Iterator var2 = this.blockEventQueue[i].iterator();

		while (var2.hasNext()) {
			BlockEventData blockeventdata = (BlockEventData)var2.next();
			if (fireBlockEvent(blockeventdata)) {
				server.getPlayerList().sendToAllNearExcept((EntityPlayer)null, (double)blockeventdata.getPosition().getX(), (double)blockeventdata.getPosition().getY(), (double)blockeventdata.getPosition().getZ(), 64.0D, this.provider.getDimensionType().getId(), new SPacketBlockAction(blockeventdata.getPosition(), blockeventdata.getBlock(), blockeventdata.getEventID(), blockeventdata.getEventParameter()));
			}
		}

		blockEventQueue[i].clear();
	}
}

ChunkProviderServer WorldServer::getChunkProvider()
{
	return (ChunkProviderServer)World::getChunkProvider();
}

Explosion WorldServer::newExplosion(Entity* entityIn, double x, double y, double z, float strength, bool causesFire,
	bool damagesTerrain)
{
	Explosion explosion(this, entityIn, x, y, z, strength, causesFire, damagesTerrain);
	explosion.doExplosionA();
	explosion.doExplosionB(false);
	if (!damagesTerrain) 
	{
		explosion.clearAffectedBlockPositions();
	}

	for(auto entityplayer : playerEntities)
	{
		if (entityplayer.getDistanceSq(x, y, z) < 4096.0) 
		{
			((EntityPlayerMP)entityplayer).connection.sendPacket(SPacketExplosion(x, y, z, strength, explosion.getAffectedBlockPositions(), (Vec3d)explosion.getPlayerKnockbackMap().get(entityplayer)));
		}
	}

	return explosion;
}

void WorldServer::addBlockEvent(BlockPos& pos, Block* blockIn, int32_t eventID, int32_t eventParam)
{
	BlockEventData blockeventdata(pos, blockIn, eventID, eventParam);
	Iterator var6 = blockEventQueue[blockEventCacheIndex].iterator();

	BlockEventData blockeventdata1;
	do {
		if (!var6.hasNext()) {
			blockEventQueue[blockEventCacheIndex].add(blockeventdata);
			return;
		}

		blockeventdata1 = (BlockEventData)var6.next();
	} while (!blockeventdata1.equals(blockeventdata));
}

void WorldServer::flush()
{
	saveHandler.flush();
}

bool WorldServer::spawnEntity(Entity* entityIn)
{
	return canAddEntity(entityIn) ? World::spawnEntity(entityIn) : false;
}

bool WorldServer::fireBlockEvent(BlockEventData event)
{
	auto iblockstate = getBlockState(event.getPosition());
	return iblockstate.getBlock() == event.getBlock() ? iblockstate.onBlockEventReceived(this, event.getPosition(), event.getEventID(), event.getEventParameter()) : false;
}

MinecraftServer* WorldServer::getMinecraftServer()
{
	return server;
}

EntityTracker* WorldServer::getEntityTracker()
{
	return entityTracker;
}

PlayerChunkMap* WorldServer::getPlayerChunkMap()
{
	return playerChunkMap;
}

Teleporter* WorldServer::getDefaultTeleporter()
{
	return worldTeleporter;
}

TemplateManager WorldServer::getStructureTemplateManager()
{
	return saveHandler.getStructureTemplateManager();
}

void WorldServer::spawnParticle(EnumParticleTypes particleType, double xCoord, double yCoord, double zCoord,
	int32_t numberOfParticles, double xOffset, double yOffset, double zOffset, double particleSpeed,
	std::initializer_list<int32_t> particleArguments)
{
	spawnParticle(particleType, false, xCoord, yCoord, zCoord, numberOfParticles, xOffset, yOffset, zOffset, particleSpeed, particleArguments);
}

void WorldServer::spawnParticle(EnumParticleTypes particleType, bool longDistance, double xCoord, double yCoord,
	double zCoord, int32_t numberOfParticles, double xOffset, double yOffset, double zOffset, double particleSpeed,
	std::initializer_list<int32_t> particleArguments)
{
	SPacketParticles spacketparticles(particleType, longDistance, (float)xCoord, (float)yCoord, (float)zCoord, (float)xOffset, (float)yOffset, (float)zOffset, (float)particleSpeed, numberOfParticles, particleArguments);

	for (auto entityplayermp : playerEntities)
	{
		sendPacketWithinDistance(entityplayermp, longDistance, xCoord, yCoord, zCoord, spacketparticles);
	}
}

void WorldServer::spawnParticle(EntityPlayerMP* player, EnumParticleTypes particle, bool longDistance, double x,
	double y, double z, int32_t count, double xOffset, double yOffset, double zOffset, double speed,
	std::initializer_list<int32_t> particleArguments)
{
	SPacketParticles packet(particle, longDistance, (float)x, (float)y, (float)z, (float)xOffset, (float)yOffset, (float)zOffset, (float)speed, count, particleArguments);
	sendPacketWithinDistance(player, longDistance, x, y, z, packet);
}

Entity* WorldServer::getEntityFromUuid(xg::Guid uuid)
{
	return entitiesByUuid[uuid];
}

void WorldServer::sendPacketWithinDistance(EntityPlayerMP* player, bool longDistance, double x, double y, double z, Packet& packetIn)
{
	auto blockpos = player.getPosition();
	double d0 = blockpos.distanceSq(x, y, z);
	if (d0 <= 1024.0 || longDistance && d0 <= 262144.0) 
	{
		player.connection.sendPacket(packetIn);
	}
}

ListenableFuture WorldServer::addScheduledTask(Runnable runnableToSchedule)
{
	return server.addScheduledTask(runnableToSchedule);
}

bool WorldServer::isCallingFromMinecraftThread()
{
	return server.isCallingFromMinecraftThread();
}

BlockPos WorldServer::findNearestStructure(std::string_view structureName, BlockPos& position, bool findUnexplored)
{
	return getChunkProvider().getNearestStructurePos(this, structureName, position, findUnexplored);
}

AdvancementManager WorldServer::getAdvancementManager()
{
	return advancementManager;
}

FunctionManager WorldServer::getFunctionManager()
{
	return functionManager;
}

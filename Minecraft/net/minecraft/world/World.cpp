#include "World.h"
#include "../util/ReportedException.h"
#include "../util/EnumFacing.h"
#include "../util/math/MathHelper.h"
#include "../util/SoundCategory.h"
#include "../util/Util.h"
#include "../../../../spdlog/spdlog/fmt/bundled/format.h"
#include "../util/ITickable.h"
#include "../util/EntitySelectors.h"

World& World::init()
{
	return *this;
}

World::World(ISaveHandler saveHandlerIn, WorldInfo info, WorldProvider providerIn, Profiler profilerIn, bool client)
	:eventListeners{ pathListener }, spawnHostileMobs(true), spawnPeacefulMobs(true), saveHandler(saveHandlerIn), profiler(profilerIn), worldInfo(info), provider(providerIn), isRemote(client), worldBorder(providerIn.createWorldBorder())
{
	lightUpdateBlockList.reserve(15237248);
}

void World::onEntityAdded(Entity* entityIn)
{
	for (auto event : eventListeners)
	{
		event->onEntityAdded(entityIn);
	}
}

void World::onEntityRemoved(Entity* entityIn)
{
	for (auto event : eventListeners)
	{
		event->onEntityRemoved(entityIn);
	}
}

void World::tickPlayers()
{
}

void World::calculateInitialWeather()
{
	if (worldInfo.isRaining()) 
	{
		rainingStrength = 1.0F;
		if (worldInfo.isThundering()) 
		{
			thunderingStrength = 1.0F;
		}
	}
}

void World::updateWeather()
{
	if (provider.hasSkyLight() && !isRemote) 
	{
		bool flag = getGameRules().getBoolean("doWeatherCycle");
		if (flag) 
		{
			auto j2 = worldInfo.getCleanWeatherTime();
			if (j2 > 0) 
			{
				--j2;
				worldInfo.setCleanWeatherTime(j2);
				worldInfo.setThunderTime(worldInfo.isThundering() ? 1 : 2);
				worldInfo.setRainTime(worldInfo.isRaining() ? 1 : 2);
			}

			auto k2 = worldInfo.getThunderTime();
			if (k2 <= 0) 
			{
				if (worldInfo.isThundering()) 
				{
					worldInfo.setThunderTime(rand(12000) + 3600);
				}
				else 
				{
					worldInfo.setThunderTime(rand(168000) + 12000);
				}
			}
			else 
			{
				--k2;
				worldInfo.setThunderTime(k2);
				if (k2 <= 0) 
				{
					worldInfo.setThundering(!worldInfo.isThundering());
				}
			}

			auto l2 = worldInfo.getRainTime();
			if (l2 <= 0) 
			{
				if (worldInfo.isRaining()) 
				{
					worldInfo.setRainTime(rand(12000) + 12000);
				}
				else 
				{
					worldInfo.setRainTime(rand(168000) + 12000);
				}
			}
			else 
			{
				--l2;
				worldInfo.setRainTime(l2);
				if (l2 <= 0) 
				{
					worldInfo.setRaining(!worldInfo.isRaining());
				}
			}
		}

		prevThunderingStrength = thunderingStrength;
		if (worldInfo.isThundering()) 
		{
			thunderingStrength = (float)((double)thunderingStrength + 0.01);
		}
		else 
		{
			thunderingStrength = (float)((double)thunderingStrength - 0.01);
		}

		thunderingStrength = MathHelper::clamp(thunderingStrength, 0.0F, 1.0F);
		prevRainingStrength = rainingStrength;
		if (worldInfo.isRaining()) 
		{
			rainingStrength = (float)((double)rainingStrength + 0.01);
		}
		else {
			rainingStrength = (float)((double)rainingStrength - 0.01);
		}

		rainingStrength = MathHelper::clamp(rainingStrength, 0.0F, 1.0F);
	}
}

void World::playMoodSoundAndCheckLight(int32_t x, int32_t z, Chunk& chunkIn)
{
	chunkIn.enqueueRelightChecks();
}

void World::updateBlocks()
{
}

bool World::isValid(BlockPos& pos) const
{
	return !isOutsideBuildHeight(pos) && pos.getx() >= -30000000 && pos.getz() >= -30000000 && pos.getx() < 30000000 && pos.getz() < 30000000;
}

bool World::isOutsideBuildHeight(BlockPos& pos) const
{
	return pos.gety() < 0 || pos.gety() >= 256;
}

bool World::isAreaLoaded(int32_t xStart, int32_t yStart, int32_t zStart, int32_t xEnd, int32_t yEnd, int32_t zEnd,
                         bool allowEmpty)
{
	if (yEnd >= 0 && yStart < 256) 
	{
		xStart >>= 4;
		zStart >>= 4;
		xEnd >>= 4;
		zEnd >>= 4;

		for (auto i = xStart; i <= xEnd; ++i) 
		{
			for (auto j = zStart; j <= zEnd; ++j) 
			{
				if (!isChunkLoaded(i, j, allowEmpty)) 
				{
					return false;
				}
			}
		}

		return true;
	}
	else {
		return false;
	}
}

void World::spawnParticle(int particleID, bool ignoreRange, double xCood, double yCoord, double zCoord, double xSpeed,
	double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters)
{
	for (auto event : eventListeners)
	{
		event->spawnParticle(particleID, ignoreRange, xCood, yCoord, zCoord, xSpeed, ySpeed, zSpeed, parameters);
	}
}

bool World::getCollisionBoxes(Entity* entityIn, AxisAlignedBB aabb, bool p_191504_3_,
	std::optional<std::vector<AxisAlignedBB>>& outList)
{
	auto i = MathHelper::floor(aabb.getminX()) - 1;
	auto j = MathHelper::ceil(aabb.getmaxX()) + 1;
	auto k = MathHelper::floor(aabb.getminY()) - 1;
	auto l = MathHelper::ceil(aabb.getmaxY()) + 1;
	auto i1 = MathHelper::floor(aabb.getminZ()) - 1;
	auto j1 = MathHelper::ceil(aabb.getmaxZ()) + 1;
	auto worldborder = getWorldBorder();
	bool flag = entityIn != nullptr && entityIn->isOutsideBorder();
	bool flag1 = entityIn != nullptr && isInsideWorldBorder(entityIn);
	auto iblockstate = Blocks::STONE.getDefaultState();
	auto pooledmutableblockpos = PooledMutableBlockPos::retain();

	try {
		for (int32_t k1 = i; k1 < j; ++k1) 
		{
			for (int32_t l1 = i1; l1 < j1; ++l1) 
			{
				bool flag2 = k1 == i || k1 == j - 1;
				bool flag3 = l1 == i1 || l1 == j1 - 1;
				if ((!flag2 || !flag3) && isBlockLoaded(pooledmutableblockpos.setPos(k1, 64, l1))) 
				{
					for (int32_t i2 = k; i2 < l; ++i2) 
					{
						if (!flag2 && !flag3 || i2 != l - 1) 
						{
							bool flag5;
							if (p_191504_3_) {
								if (k1 < -30000000 || k1 >= 30000000 || l1 < -30000000 || l1 >= 30000000) 
								{
									bool lvt_21_2_ = true;
									flag5 = lvt_21_2_;
									return flag5;
								}
							}
							else if (entityIn != nullptr && flag == flag1) 
							{
								entityIn->setOutsideBorder(!flag1);
							}

							pooledmutableblockpos.setPos(k1, i2, l1);
							IBlockState* iblockstate1;
							if (!p_191504_3_ && !worldborder.contains(pooledmutableblockpos) && flag1) 
							{
								iblockstate1 = iblockstate;
							}
							else 
							{
								iblockstate1 = getBlockState(pooledmutableblockpos);
							}

							iblockstate1->addCollisionBoxToList(this, pooledmutableblockpos, aabb, outList, entityIn, false);
							if (p_191504_3_ && !outList.value().empty()) 
							{
								flag5 = true;
								bool var23 = flag5;
								return var23;
							}
						}
					}
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		pooledmutableblockpos.release();
	}

	return !outList.value().empty();
}

bool World::isWater(BlockPos& pos)
{
	return getBlockState(pos).getMaterial() == Material::WATER;
}

bool World::isAirBlock(const BlockPos& pos)
{
	return getBlockState(pos)->getMaterial() == Material::AIR;
}

bool World::isBlockLoaded(BlockPos& pos)
{
	return isBlockLoaded(pos, true);
}

bool World::isBlockLoaded(BlockPos& pos, bool allowEmpty)
{
	return isChunkLoaded(pos.getx() >> 4, pos.getz() >> 4, allowEmpty);
}

bool World::isAreaLoaded(BlockPos& from, BlockPos& to)
{
	return isAreaLoaded(from, to, true);
}

bool World::isAreaLoaded(StructureBoundingBox box)
{
	return isAreaLoaded(box, true);
}

bool World::isAreaLoaded(StructureBoundingBox box, bool allowEmpty)
{
	return isAreaLoaded(box.minX, box.minY, box.minZ, box.maxX, box.maxY, box.maxZ, allowEmpty);
}

bool World::isAreaLoaded(BlockPos& from, BlockPos& to, bool allowEmpty)
{
	return isAreaLoaded(from.getx(), from.gety(), from.getz(), to.getx(), to.gety(), to.getz(), allowEmpty);
}

bool World::isAreaLoaded(BlockPos& center, int32_t radius)
{
	return isAreaLoaded(center, radius, true);
}

bool World::isAreaLoaded(BlockPos& center, int32_t radius, bool allowEmpty)
{
	return isAreaLoaded(center.getx() - radius, center.gety() - radius, center.getz() - radius, center.getx() + radius, center.gety() + radius, center.getz() + radius, allowEmpty);
}

Biome& World::getBiome(const BlockPos& pos)
{
	if (isBlockLoaded(pos)) 
	{
		auto chunk = getChunk(pos);

		try {
			return chunk.getBiome(pos, provider.getBiomeProvider());
		}
		catch (Throwable var6) 
		{
			CrashReport crashreport = CrashReport.makeCrashReport(var6, "Getting biome");
			CrashReportCategory crashreportcategory = crashreport.makeCategory("Coordinates of biome request");
			crashreportcategory.addDetail("Location", new ICrashReportDetail()
			{
				public String call() throws Exception {
					return CrashReportCategory.getCoordinateInfo(pos);
				}
			});
			throw ReportedException(crashreport);
		}
	}
	else
	{
		return provider.getBiomeProvider().getBiome(pos, Biomes::PLAINS);
	}
}

BiomeProvider& World::getBiomeProvider()
{
	return provider.getBiomeProvider();
}

void World::initialize(WorldSettings settings)
{
	worldInfo.setServerInitialized(true);
}

MinecraftServer* World::getMinecraftServer()
{
	return nullptr;
}

void World::setInitialSpawnLocation()
{
	setSpawnPoint(BlockPos(8, 64, 8));
}

IBlockState* World::getGroundAboveSeaLevel(const BlockPos& pos)
{
	BlockPos blockpos;
	for (blockpos = BlockPos(pos.getx(), getSeaLevel(), pos.getz()); !isAirBlock(blockpos.up()); blockpos = blockpos.up())
	{
	}

	return getBlockState(blockpos);
}

Chunk& World::getChunk(BlockPos pos)
{
	return getChunk(pos.getx() >> 4, pos.getz() >> 4);
}
Chunk& World::getChunk(int32_t chunkX, int32_t chunkZ)
{
	return chunkProvider.provideChunk(chunkX, chunkZ);
}

bool World::isChunkGeneratedAt(int32_t x, int32_t z)
{
	return isChunkLoaded(x, z, false) ? true : chunkProvider.isChunkGeneratedAt(x, z);
}

bool World::setBlockState(BlockPos& pos, IBlockState* newState, int32_t flags)
{
	if (isOutsideBuildHeight(pos)) 
	{
		return false;
	}
	else if (!isRemote && worldInfo.getTerrainType() == WorldType::DEBUG_ALL_BLOCK_STATES) 
	{
		return false;
	}
	else 
	{
		auto chunk = getChunk(pos);
		auto block = newState->getBlock();
		auto iblockstate = chunk.setBlockState(pos, newState);
		if (iblockstate == nullptr) 
		{
			return false;
		}
		else 
		{
			if (newState->getLightOpacity() != iblockstate.getLightOpacity() || newState->getLightValue() != iblockstate.getLightValue()) 
			{
				profiler.startSection("checkLight");
				checkLight(pos);
				profiler.endSection();
			}

			if ((flags & 2) != 0 && (!isRemote || (flags & 4) == 0) && chunk.isPopulated()) 
			{
				notifyBlockUpdate(pos, iblockstate, newState, flags);
			}

			if (!isRemote && (flags & 1) != 0) 
			{
				notifyNeighborsRespectDebug(pos, iblockstate.getBlock(), true);
				if (newState.hasComparatorInputOverride())
				{
					updateComparatorOutputLevel(pos, block);
				}
			}
			else if (!isRemote && (flags & 16) == 0) 
			{
				updateObservingBlocksAt(pos, block);
			}

			return true;
		}
	}
}

bool World::setBlockToAir(BlockPos& pos)
{
	return setBlockState(pos, Blocks::AIR.getDefaultState(), 3);
}

bool World::destroyBlock(BlockPos& pos, bool dropBlock)
{
	auto iblockstate = getBlockState(pos);
	auto block = iblockstate->getBlock();
	if (iblockstate->getMaterial() == Material::AIR) 
	{
		return false;
	}
	else 
	{
		playEvent(2001, pos, Block::getStateId(iblockstate));
		if (dropBlock) {
			block->dropBlockAsItem(this, pos, iblockstate, 0);
		}

		return setBlockState(pos, Blocks::AIR.getDefaultState(), 3);
	}
}

bool World::setBlockState(BlockPos& pos, IBlockState* state)
{
	return setBlockState(pos, state, 3);
}

void World::notifyBlockUpdate(BlockPos& pos, IBlockState* oldState, IBlockState* newState, int32_t flags)
{
	for (auto i = 0; i < eventListeners.size(); ++i) 
	{
		eventListeners[i]->notifyBlockUpdate(this, pos, oldState, newState, flags);
	}
}

void World::notifyNeighborsRespectDebug(BlockPos& pos, Block* blockType, bool updateObservers)
{
	if (worldInfo.getTerrainType() != WorldType::DEBUG_ALL_BLOCK_STATES) 
	{
		notifyNeighborsOfStateChange(pos, blockType, updateObservers);
	}
}

void World::markBlocksDirtyVertical(int32_t x, int32_t z, int32_t y1, int32_t y2)
{
	if (y1 > y2) {
		auto j = y2;
		y2 = y1;
		y1 = j;
	}

	if (provider.hasSkyLight()) {
		for (auto j = y1; j <= y2; ++j) 
		{
			thicheckLightFor(EnumSkyBlock.SKY, BlockPos(x, j, z));
		}
	}

	markBlockRangeForRenderUpdate(x, y1, z, x, y2, z);
}

void World::markBlockRangeForRenderUpdate(BlockPos& rangeMin, BlockPos& rangeMax)
{
	markBlockRangeForRenderUpdate(rangeMin.getx(), rangeMin.gety(), rangeMin.getz(), rangeMax.getx(), rangeMax.gety(), rangeMax.getz());
}

void World::markBlockRangeForRenderUpdate(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2)
{
	for(auto event : eventListeners)
	{
		event->markBlockRangeForRenderUpdate(x1, y1, z1, x2, y2, z2);
	}
}

void World::updateObservingBlocksAt(BlockPos& pos, Block* blockType)
{
	observedNeighborChanged(pos.west(), blockType, pos);
	observedNeighborChanged(pos.east(), blockType, pos);
	observedNeighborChanged(pos.down(), blockType, pos);
	observedNeighborChanged(pos.up(), blockType, pos);
	observedNeighborChanged(pos.north(), blockType, pos);
	observedNeighborChanged(pos.south(), blockType, pos);
}

void World::notifyNeighborsOfStateChange(BlockPos& pos, Block* blockType, bool updateObservers)
{
	neighborChanged(pos.west(), blockType, pos);
	neighborChanged(pos.east(), blockType, pos);
	neighborChanged(pos.down(), blockType, pos);
	neighborChanged(pos.up(), blockType, pos);
	neighborChanged(pos.north(), blockType, pos);
	neighborChanged(pos.south(), blockType, pos);
	if (updateObservers)
	{
		updateObservingBlocksAt(pos, blockType);
	}
}

void World::notifyNeighborsOfStateExcept(BlockPos& pos, Block* blockType, EnumFacing skipSide)
{
	if (skipSide != EnumFacing::WEST) 
	{
		neighborChanged(pos.west(), blockType, pos);
	}

	if (skipSide != EnumFacing::EAST) 
	{
		neighborChanged(pos.east(), blockType, pos);
	}

	if (skipSide != EnumFacing::DOWN) 
	{
		neighborChanged(pos.down(), blockType, pos);
	}

	if (skipSide != EnumFacing::UP) 
	{
		neighborChanged(pos.up(), blockType, pos);
	}

	if (skipSide != EnumFacing::NORTH) 
	{
		neighborChanged(pos.north(), blockType, pos);
	}

	if (skipSide != EnumFacing::SOUTH) 
	{
		neighborChanged(pos.south(), blockType, pos);
	}
}

void World::neighborChanged(BlockPos& pos, const Block* blockIn, BlockPos& fromPos)
{
	if (!isRemote) {
		auto iblockstate = getBlockState(pos);

		try 
		{
			iblockstate->neighborChanged(this, pos, blockIn, fromPos);
		}
		catch (Throwable var8) {
			CrashReport crashreport = CrashReport.makeCrashReport(var8, "Exception while updating neighbours");
			CrashReportCategory crashreportcategory = crashreport.makeCategory("Block being updated");
			crashreportcategory.addDetail("Source block type", new ICrashReportDetail(){
			   public String call() throws Exception {
				  try {
					 return String.format("ID #%d (%s // %s)", Block.getIdFromBlock(blockIn), blockIn.getTranslationKey(), blockIn.getClass().getCanonicalName());
				  }
 catch (Throwable var2) {
 return "ID #" + Block.getIdFromBlock(blockIn);
}
}
				});
			CrashReportCategory.addBlockInfo(crashreportcategory, pos, iblockstate);
			throw new ReportedException(crashreport);
		}
	}
}

void World::observedNeighborChanged(BlockPos& pos, const Block* changedBlock, BlockPos& changedBlockPos)
{
	if (!isRemote) {
		auto iblockstate = getBlockState(pos);
		if (iblockstate->getBlock() == Blocks::OBSERVER) 
		{
			try 
			{
				((BlockObserver)iblockstate.getBlock()).observedNeighborChanged(iblockstate, this, pos, changedBlock, changedBlockPos);
			}
			catch (Throwable var8) {
				CrashReport crashreport = CrashReport.makeCrashReport(var8, "Exception while updating neighbours");
				CrashReportCategory crashreportcategory = crashreport.makeCategory("Block being updated");
				crashreportcategory.addDetail("Source block type", new ICrashReportDetail(){
				   public String call() throws Exception {
					  try {
						 return String.format("ID #%d (%s // %s)", Block.getIdFromBlock(changedBlock), changedBlock.getTranslationKey(), changedBlock.getClass().getCanonicalName());
					  }
  catch (Throwable var2) {
  return "ID #" + Block.getIdFromBlock(changedBlock);
}
}
					});
				CrashReportCategory.addBlockInfo(crashreportcategory, pos, iblockstate);
				throw new ReportedException(crashreport);
			}
		}
	}
}

bool World::isBlockTickPending(BlockPos& pos, Block* blockType) const
{
	return false;
}

bool World::canSeeSky(BlockPos& pos)
{
	return getChunk(pos).canSeeSky(pos);
}

bool World::canBlockSeeSky(BlockPos& pos)
{
	if (pos.gety() >= getSeaLevel()) 
	{
		return canSeeSky(pos);
	}
	else {
		BlockPos blockpos(pos.getx(), getSeaLevel(), pos.getz());
		if (!canSeeSky(blockpos)) {
			return false;
		}
		else {
			for (BlockPos blockpos1 = blockpos.down(); blockpos1.gety() > pos.gety(); blockpos1 = blockpos1.down()) 
			{
				auto iblockstate = getBlockState(blockpos1);
				if (iblockstate->getLightOpacity() > 0 && !iblockstate->getMaterial().isLiquid()) 
				{
					return false;
				}
			}

			return true;
		}
	}
}

int32_t World::getLight(BlockPos& pos)
{
	if (pos.gety() < 0) 
	{
		return 0;
	}
	else 
	{
		if (pos.gety() >= 256) 
		{
			pos = BlockPos(pos.getx(), 255, pos.getz());
		}

		return getChunk(pos).getLightSubtracted(pos, 0);
	}
}

int32_t World::getLightFromNeighbors(BlockPos& pos)
{
	return getLight(pos, true);
}

int32_t World::getLight(BlockPos& pos, bool checkNeighbors)
{
	if (pos.getx() >= -30000000 && pos.getz() >= -30000000 && pos.getx() < 30000000 && pos.getz() < 30000000) 
	{
		if (checkNeighbors && getBlockState(pos)->useNeighborBrightness()) {
			auto i1 =getLight(pos.up(), false);
			auto i = getLight(pos.east(), false);
			auto j = getLight(pos.west(), false);
			auto k = getLight(pos.south(), false);
			auto l = getLight(pos.north(), false);
			if (i > i1) 
			{
				i1 = i;
			}

			if (j > i1) 
			{
				i1 = j;
			}

			if (k > i1) 
			{
				i1 = k;
			}

			if (l > i1) 
			{
				i1 = l;
			}

			return i1;
		}
		else if (pos.gety() < 0) 
		{
			return 0;
		}
		else {
			if (pos.gety() >= 256) 
			{
				pos = BlockPos(pos.getx(), 255, pos.getz());
			}

			auto chunk = getChunk(pos);
			return chunk.getLightSubtracted(pos, skylightSubtracted);
		}
	}
	else 
	{
		return 15;
	}
}

BlockPos World::getHeight(BlockPos& pos)
{
	return BlockPos(pos.getx(), getHeight(pos.getx(), pos.getz()), pos.getz());
}

int32_t World::getHeight(int32_t x, int32_t z)
{
	int32_t i = 0;
	if (x >= -30000000 && z >= -30000000 && x < 30000000 && z < 30000000) {
		if (isChunkLoaded(x >> 4, z >> 4, true)) 
		{
			i = getChunk(x >> 4, z >> 4).getHeightValue(x & 15, z & 15);
		}
	}
	else {
		i = getSeaLevel() + 1;
	}

	return i;
}

int32_t World::getChunksLowestHorizon(int32_t x, int32_t z)
{
	if (x >= -30000000 && z >= -30000000 && x < 30000000 && z < 30000000) {
		if (!isChunkLoaded(x >> 4, z >> 4, true)) 
		{
			return 0;
		}
		else {
			auto chunk = getChunk(x >> 4, z >> 4);
			return chunk.getLowestHeight();
		}
	}
	else {
		return getSeaLevel() + 1;
	}
}

int32_t World::getLightFromNeighborsFor(EnumSkyBlock type, BlockPos& pos)
{
	if (!provider.hasSkyLight() && type == EnumSkyBlock::SKY)
	{
		return 0;
	}
	else 
	{
		if (pos.gety() < 0) {
			pos = BlockPos(pos.getx(), 0, pos.getz());
		}

		if (!isValid(pos)) 
		{
			return type.defaultLightValue;
		}
		else if (!isBlockLoaded(pos)) 
		{
			return type.defaultLightValue;
		}
		else if (getBlockState(pos)->useNeighborBrightness()) 
		{
			auto i1 =getLightFor(type, pos.up());
			auto i = getLightFor(type, pos.east());
			auto j = getLightFor(type, pos.west());
			auto k = getLightFor(type, pos.south());
			auto l = getLightFor(type, pos.north());
			if (i > i1) 
			{
				i1 = i;
			}

			if (j > i1) 
			{
				i1 = j;
			}

			if (k > i1) 
			{
				i1 = k;
			}

			if (l > i1) 
			{
				i1 = l;
			}

			return i1;
		}
		else 
		{
			auto chunk = getChunk(pos);
			return chunk.getLightFor(type, pos);
		}
	}
}

int32_t World::getLightFor(EnumSkyBlock type, BlockPos& pos)
{
	if (pos.gety() < 0) {
		pos = BlockPos(pos.getx(), 0, pos.getz());
	}

	if (!isValid(pos)) 
	{
		return type.defaultLightValue;
	}
	else if (!isBlockLoaded(pos)) 
	{
		return type.defaultLightValue;
	}
	else 
	{
		auto chunk = getChunk(pos);
		return chunk.getLightFor(type, pos);
	}
}

void World::setLightFor(EnumSkyBlock type, BlockPos& pos, int32_t lightValue)
{
	if (isValid(pos) && isBlockLoaded(pos)) 
	{
		auto chunk = getChunk(pos);
		chunk.setLightFor(type, pos, lightValue);
		notifyLightSet(pos);
	}
}

void World::notifyLightSet(BlockPos& pos)
{
	for(auto event : eventListeners)
	{
		event->notifyLightSet(pos);
	}
}

int32_t World::getCombinedLight(BlockPos& pos, int32_t lightValue)
{
	auto i = getLightFromNeighborsFor(EnumSkyBlock::SKY, pos);
	auto j = getLightFromNeighborsFor(EnumSkyBlock::BLOCK, pos);
	if (j < lightValue) 
	{
		j = lightValue;
	}

	return i << 20 | j << 4;
}

float World::getLightBrightness(const BlockPos& pos)
{
	return provider.getLightBrightnessTable()[getLightFromNeighbors(pos)];
}

IBlockState* World::getBlockState(const BlockPos& pos)
{
	if (isOutsideBuildHeight(pos))
	{
		return Blocks::AIR.getDefaultState();
	}
	else 
	{
		auto chunk = getChunk(pos);
		return chunk.getBlockState(pos);
	}
}

bool World::isDaytime() const
{
	return skylightSubtracted < 4;
}

std::optional<RayTraceResult> World::rayTraceBlocks(Vec3d& start, Vec3d& end)
{
	return rayTraceBlocks(start, end, false, false, false);
}

std::optional<RayTraceResult> World::rayTraceBlocks(Vec3d& start, Vec3d& end, bool stopOnLiquid)
{
	return rayTraceBlocks(start, end, stopOnLiquid, false, false);
}

std::optional<RayTraceResult> World::rayTraceBlocks(Vec3d& vec31, Vec3d& vec32, bool stopOnLiquid,
	bool ignoreBlockWithoutBoundingBox, bool returnLastUncollidableBlock)
{
	if (!std::isnan(vec31.getx()) && !std::isnan(vec31.gety()) && !std::isnan(vec31.getz()))
	{
		if (!std::isnan(vec32.getx()) && !std::isnan(vec32.gety()) && !std::isnan(vec32.getz()))
		{
			auto i = MathHelper::floor(vec32.getx());
			auto j = MathHelper::floor(vec32.gety());
			auto k = MathHelper::floor(vec32.getz());
			auto l = MathHelper::floor(vec31.getx());
			auto i1 = MathHelper::floor(vec31.gety());
			auto j1 = MathHelper::floor(vec31.getz());
			BlockPos blockpos(l, i1, j1);
			auto iblockstate = getBlockState(blockpos);
			auto block = iblockstate->getBlock();
			std::optional<RayTraceResult> raytraceresult2;
			if ((!ignoreBlockWithoutBoundingBox || iblockstate->getCollisionBoundingBox(this, blockpos) != Block::NULL_AABB) && block.canCollideCheck(iblockstate, stopOnLiquid))
			{
				raytraceresult2 = iblockstate->collisionRayTrace(this, blockpos, vec31, vec32);
				if (raytraceresult2)
				{
					return raytraceresult2;
				}
			}

			raytraceresult2 = std::nullopt;
			auto var16 = 200;

			while (var16-- >= 0)
			{
				if (std::isnan(vec31.getx()) || std::isnan(vec31.gety()) || std::isnan(vec31.getz()))
				{
					return std::nullopt;
				}

				if (l == i && i1 == j && j1 == k)
				{
					return returnLastUncollidableBlock ? raytraceresult2 : std::nullopt;
				}

				bool flag2 = true;
				bool flag = true;
				bool flag1 = true;
				double d0 = 999.0;
				double d1 = 999.0;
				double d2 = 999.0;
				if (i > l) {
					d0 = (double)l + 1.0;
				}
				else if (i < l) {
					d0 = (double)l + 0.0;
				}
				else {
					flag2 = false;
				}

				if (j > i1) {
					d1 = (double)i1 + 1.0;
				}
				else if (j < i1) {
					d1 = (double)i1 + 0.0;
				}
				else {
					flag = false;
				}

				if (k > j1) {
					d2 = (double)j1 + 1.0;
				}
				else if (k < j1) {
					d2 = (double)j1 + 0.0;
				}
				else {
					flag1 = false;
				}

				double d3 = 999.0;
				double d4 = 999.0;
				double d5 = 999.0;
				double d6 = vec32.getx() - vec31.getx();
				double d7 = vec32.gety() - vec31.gety();
				double d8 = vec32.getz() - vec31.getz();
				if (flag2)
				{
					d3 = (d0 - vec31.getx()) / d6;
				}

				if (flag)
				{
					d4 = (d1 - vec31.gety()) / d7;
				}

				if (flag1)
				{
					d5 = (d2 - vec31.getz()) / d8;
				}

				if (d3 == -0.0)
				{
					d3 = -1.0E-4;
				}

				if (d4 == -0.0)
				{
					d4 = -1.0E-4;
				}

				if (d5 == -0.0)
				{
					d5 = -1.0E-4;
				}

				EnumFacing enumfacing;
				if (d3 < d4&& d3 < d5) {
					enumfacing = i > l ? EnumFacing::WEST : EnumFacing::EAST;
					vec31 = Vec3d(d0, vec31.gety() + d7 * d3, vec31.getz() + d8 * d3);
				}
				else if (d4 < d5) {
					enumfacing = j > i1 ? EnumFacing::DOWN : EnumFacing::UP;
					vec31 = Vec3d(vec31.getx() + d6 * d4, d1, vec31.getz() + d8 * d4);
				}
				else {
					enumfacing = k > j1 ? EnumFacing::NORTH : EnumFacing::SOUTH;
					vec31 = Vec3d(vec31.getx() + d6 * d5, vec31.gety() + d7 * d5, d2);
				}

				l = MathHelper::floor(vec31.getx()) - (enumfacing == EnumFacing::EAST ? 1 : 0);
				i1 = MathHelper::floor(vec31.gety()) - (enumfacing == EnumFacing::UP ? 1 : 0);
				j1 = MathHelper::floor(vec31.getz()) - (enumfacing == EnumFacing::SOUTH ? 1 : 0);
				blockpos = BlockPos(l, i1, j1);
				auto iblockstate1 = getBlockState(blockpos);
				auto block1 = iblockstate1->getBlock();
				if (!ignoreBlockWithoutBoundingBox || iblockstate1->getMaterial() == Material::PORTAL || iblockstate1->getCollisionBoundingBox(this, blockpos) != Block::NULL_AABB)
				{
					if (block1->canCollideCheck(iblockstate1, stopOnLiquid)) {
						auto raytraceresult1 = iblockstate1->collisionRayTrace(this, blockpos, vec31, vec32);
						if (raytraceresult1)
						{
							return raytraceresult1;
						}
					}
					else {
						raytraceresult2 = RayTraceResult(RayTraceResult::Type.MISS, vec31, enumfacing, blockpos);
					}
				}
			}

			return returnLastUncollidableBlock ? raytraceresult2 : std::nullopt;
		}
		else 
		{
			return std::nullopt;
		}
	}
	else 
	{
		return std::nullopt;
	}
}

void World::playSound(std::optional<EntityPlayer> player, BlockPos pos, SoundEvent soundIn, SoundCategory category,
	float volume, float pitch)
{
	playSound(player, (double)pos.getx() + 0.5, (double)pos.gety() + 0.5, (double)pos.getz() + 0.5, soundIn, category, volume, pitch);
}

void World::playSound(std::optional<EntityPlayer> player, double x, double y, double z, SoundEvent soundIn,
	SoundCategory category, float volume, float pitch)
{
	for(auto event : eventListeners)
	{
		event->playSoundToAllNearExcept(player, soundIn, category, x, y, z, volume, pitch);
	}
}

void World::playSound(double x, double y, double z, SoundEvent soundIn, SoundCategory category, float volume,
	float pitch, bool distanceDelay)
{
}

void World::playRecord(BlockPos& blockPositionIn, std::optional<SoundEvent> soundEventIn)
{
	for (auto event : eventListeners)
	{
		event->playRecord(soundEventIn, blockPositionIn);
	}
}

void World::spawnParticle(EnumParticleTypes particleType, double xCoord, double yCoord, double zCoord, double xSpeed,
	double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters)
{
	spawnParticle(particleType.getParticleID(), particleType.getShouldIgnoreRange(), xCoord, yCoord, zCoord, xSpeed, ySpeed, zSpeed, parameters);
}

void World::spawnAlwaysVisibleParticle(int32_t id, double x, double y, double z, double xSpeed, double ySpeed,
	double zSpeed, std::initializer_list<int32_t> parameters)
{
	for (auto event : eventListeners)
	{
		event->spawnParticle(id, false, true, x, y, z, xSpeed, ySpeed, zSpeed, parameters);
	}
}

void World::spawnParticle(EnumParticleTypes particleType, bool ignoreRange, double xCoord, double yCoord, double zCoord,
	double xSpeed, double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters)
{
	spawnParticle(particleType.getParticleID(), particleType.getShouldIgnoreRange() || ignoreRange, xCoord, yCoord, zCoord, xSpeed, ySpeed, zSpeed, parameters);
}

bool World::addWeatherEffect(Entity* entityIn)
{
	weatherEffects.emplace_back(entityIn);
	return true;
}

bool World::spawnEntity(Entity* entityIn)
{
	auto i = MathHelper::floor(entityIn->posX / 16.0);
	auto j = MathHelper::floor(entityIn->posZ / 16.0);
	bool flag = entityIn->forceSpawn;
	if (Util::instanceof<EntityPlayer>(entityIn)) 
	{
		flag = true;
	}

	if (!flag && !isChunkLoaded(i, j, false)) 
	{
		return false;
	}
	else 
	{
		if (Util::instanceof<EntityPlayer>(entityIn)) 
		{
			auto entityplayer = reinterpret_cast<EntityPlayer>(entityIn);
			playerEntities.emplace_back(entityplayer);
			updateAllPlayersSleepingFlag();
		}

		getChunk(i, j).addEntity(entityIn);
		loadedEntityList.emplace_back(entityIn);
		onEntityAdded(entityIn);
		return true;
	}
}

void World::removeEntity(Entity* entityIn)
{
	if (entityIn->isBeingRidden()) 
	{
		entityIn->removePassengers();
	}

	if (entityIn->isRiding()) 
	{
		entityIn->dismountRidingEntity();
	}

	entityIn->setDead();
	if (Util::instanceof< EntityPlayer>(entityIn)) 
	{
		playerEntities.erase(std::find(playerEntities.begin(), playerEntities.end(), entityIn));
		updateAllPlayersSleepingFlag();
		onEntityRemoved(entityIn);
	}
}

void World::removeEntityDangerously(Entity* entityIn)
{
	entityIn->setDropItemsWhenDead(false);
	entityIn->setDead();
	if (Util::instanceof<EntityPlayer>(entityIn)) 
	{
		playerEntities.erase(std::find(playerEntities.begin(), playerEntities.end(), entityIn));
		updateAllPlayersSleepingFlag();
	}

	auto i = entityIn->chunkCoordX;
	auto j = entityIn->chunkCoordZ;
	if (entityIn->addedToChunk && isChunkLoaded(i, j, true)) 
	{
		getChunk(i, j).removeEntity(entityIn);
	}

	loadedEntityList.erase(std::find(loadedEntityList.begin(), loadedEntityList.end(), entityIn));
	onEntityRemoved(entityIn);
}

void World::addEventListener(IWorldEventListener* listener)
{
	eventListeners.emplace_back(listener);
}

void World::removeEventListener(IWorldEventListener* listener)
{
	eventListeners.erase(std::find(eventListeners.begin(), eventListeners.end(), listener));
}

std::optional<std::vector<AxisAlignedBB>> World::getCollisionBoxes(Entity* entityIn, AxisAlignedBB& aabb)
{
	std::optional<std::vector<AxisAlignedBB>> list;
	getCollisionBoxes(entityIn, aabb, false, list);
	if (entityIn != nullptr) 
	{
		auto list1 = getEntitiesWithinAABBExcludingEntity(entityIn, aabb.grow(0.25));
		for(auto entity : list1)
		{
			if (!entityIn->isRidingSameEntity(entity)) 
			{
				auto axisalignedbb = entity.getCollisionBoundingBox();
				if (axisalignedbb && axisalignedbb.intersects(aabb)) 
				{
					list->emplace_back(axisalignedbb);
				}

				axisalignedbb = entityIn->getCollisionBox(entity);
				if (axisalignedbb && axisalignedbb.intersects(aabb)) 
				{
					list->emplace_back(axisalignedbb);
				}
			}
		}
	}

	return list;
}

bool World::isInsideWorldBorder(Entity* entityToCheck)
{
	double d0 = worldBorder.minX();
	double d1 = worldBorder.minZ();
	double d2 = worldBorder.maxX();
	double d3 = worldBorder.maxZ();
	if (entityToCheck->isOutsideBorder()) 
	{
		++d0;
		++d1;
		--d2;
		--d3;
	}
	else 
	{
		--d0;
		--d1;
		++d2;
		++d3;
	}

	return entityToCheck->posX > d0&& entityToCheck->posX < d2&& entityToCheck->posZ > d1&& entityToCheck->posZ < d3;
}

bool World::collidesWithAnyBlock(AxisAlignedBB& bbox)
{
	std::optional<std::vector<AxisAlignedBB>> opt;
	return getCollisionBoxes(nullptr, bbox, true, opt);
}

int32_t World::calculateSkylightSubtracted(float partialTicks)
{
	float f = getCelestialAngle(partialTicks);
	float f1 = 1.0F - (MathHelper::cos(f * 6.2831855F) * 2.0F + 0.5F);
	f1 = MathHelper::clamp(f1, 0.0F, 1.0F);
	f1 = 1.0F - f1;
	f1 = (float)((double)f1 * (1.0 - (double)(getRainStrength(partialTicks) * 5.0F) / 16.0));
	f1 = (float)((double)f1 * (1.0 - (double)(getThunderStrength(partialTicks) * 5.0F) / 16.0));
	f1 = 1.0F - f1;
	return (int)(f1 * 11.0F);
}

float World::getSunBrightness(float partialTicks)
{
	float f = getCelestialAngle(partialTicks);
	float f1 = 1.0F - (MathHelper::cos(f * 6.2831855F) * 2.0F + 0.2F);
	f1 = MathHelper::clamp(f1, 0.0F, 1.0F);
	f1 = 1.0F - f1;
	f1 = (float)((double)f1 * (1.0 - (double)(getRainStrength(partialTicks) * 5.0F) / 16.0));
	f1 = (float)((double)f1 * (1.0 - (double)(getThunderStrength(partialTicks) * 5.0F) / 16.0));
	return f1 * 0.8F + 0.2F;
}

Vec3d World::getSkyColor(Entity* entityIn, float partialTicks)
{
	float f = getCelestialAngle(partialTicks);
	float f1 = MathHelper::cos(f * 6.2831855F) * 2.0F + 0.5F;
	f1 = MathHelper::clamp(f1, 0.0F, 1.0F);
	float f2 = 1.0F;
	float f3 = 1.0F;
	float f4 = 1.0F;
	float f5 = getRainStrength(partialTicks);
	float f9;
	float f10;
	if (f5 > 0.0F) {
		f9 = (f2 * 0.3F + f3 * 0.59F + f4 * 0.11F) * 0.6F;
		f10 = 1.0F - f5 * 0.95F;
		f2 = f2 * f10 + f9 * (1.0F - f10);
		f3 = f3 * f10 + f9 * (1.0F - f10);
		f4 = f4 * f10 + f9 * (1.0F - f10);
	}

	f2 *= f1 * 0.9F + 0.1F;
	f3 *= f1 * 0.9F + 0.1F;
	f4 *= f1 * 0.85F + 0.15F;
	f9 = getThunderStrength(partialTicks);
	if (f9 > 0.0F) {
		f10 = (f2 * 0.3F + f3 * 0.59F + f4 * 0.11F) * 0.2F;
		float f8 = 1.0F - f9 * 0.95F;
		f2 = f2 * f8 + f10 * (1.0F - f8);
		f3 = f3 * f8 + f10 * (1.0F - f8);
		f4 = f4 * f8 + f10 * (1.0F - f8);
	}

	return Vec3d((double)f2, (double)f3, (double)f4);
}

Vec3d World::getFogColor(float partialTicks)
{
	float f = getCelestialAngle(partialTicks);
	return provider.getFogColor(f, partialTicks);
}

BlockPos World::getPrecipitationHeight(BlockPos& pos)
{
	return getChunk(pos).getPrecipitationHeight(pos);
}

BlockPos World::getTopSolidOrLiquidBlock(BlockPos& pos)
{
	auto chunk = getChunk(pos);

	BlockPos blockpos;
	BlockPos blockpos1;
	for (blockpos = BlockPos(pos.getx(), chunk.getTopFilledSegment() + 16, pos.getz()); blockpos.gety() >= 0; blockpos = blockpos1) 
	{
		blockpos1 = blockpos.down();
		auto material = chunk.getBlockState(blockpos1).getMaterial();
		if (material.blocksMovement() && material != Material::LEAVES) 
		{
			break;
		}
	}

	return blockpos;
}

float World::getStarBrightness(float partialTicks)
{
	float f = getCelestialAngle(partialTicks);
	float f1 = 1.0F - (MathHelper::cos(f * 6.2831855F) * 2.0F + 0.25F);
	f1 = MathHelper::clamp(f1, 0.0F, 1.0F);
	return f1 * f1 * 0.5F;
}

bool World::isUpdateScheduled(BlockPos& pos, Block* blk) const
{
	return true;
}

void World::scheduleUpdate(BlockPos& pos, Block* blockIn, int32_t delay) const
{
}

void World::updateBlockTick(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority) const
{
}

void World::scheduleBlockUpdate(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority)
{
}

void World::updateEntities()
{
	profiler.startSection("entities");
	profiler.startSection("global");

	Entity* entity2;
	for (auto i1 = 0; i1 < weatherEffects.size(); ++i1) 
	{
		entity2 = weatherEffects[i1];

		try 
		{
			(++entity2)->ticksExisted;
			entity2->onUpdate();
		}
		catch (Throwable var9) {
			CrashReport crashreport = CrashReport.makeCrashReport(var9, "Ticking entity");
			CrashReportCategory crashreportcategory = crashreport.makeCategory("Entity being ticked");
			if (entity2 == nullptr) {
				crashreportcategory.addCrashSection("Entity", "~~NULL~~");
			}
			else {
				entity2.addEntityCrashInfo(crashreportcategory);
			}

			throw new ReportedException(crashreport);
		}

		if (entity2->isDead) 
		{
			weatherEffects.erase(weatherEffects.begin() + i1--);
		}
	}

	profiler.endStartSection("remove");
	Util::removeAll(loadedEntityList, unloadedEntityList);

	for (auto i1 = 0; i1 < unloadedEntityList.size(); ++i1) 
	{
		entity2 = unloadedEntityList[i1];
		auto j = entity2->chunkCoordX;
		auto l1 = entity2->chunkCoordZ;
		if (entity2->addedToChunk && isChunkLoaded(j, l1, true)) 
		{
			getChunk(j, l1).removeEntity(entity2);
		}
	}

	for (auto i1 = 0; i1 < unloadedEntityList.size(); ++i1) 
	{
		onEntityRemoved(unloadedEntityList[i1]);
	}

	unloadedEntityList.clear();
	tickPlayers();
	profiler.endStartSection("regular");

	CrashReportCategory crashreportcategory2;
	CrashReport crashreport2;
	for (auto i1 = 0; i1 < loadedEntityList.size(); ++i1) 
	{
		entity2 = loadedEntityList[i1];
		auto entity3 = entity2.getRidingEntity();
		if (entity3 != nullptr) 
		{
			if (!entity3.isDead && entity3.isPassenger(entity2)) {
				continue;
			}

			entity2.dismountRidingEntity();
		}

		profiler.startSection("tick");
		if (!entity2.isDead && !(Util::instanceof< EntityPlayerMP>(entity2))) 
		{
			try {
				updateEntity(entity2);
			}
			catch (Throwable var8) {
				crashreport2 = CrashReport.makeCrashReport(var8, "Ticking entity");
				crashreportcategory2 = crashreport2.makeCategory("Entity being ticked");
				entity2.addEntityCrashInfo(crashreportcategory2);
				throw new ReportedException(crashreport2);
			}
		}

		profiler.endSection();
		profiler.startSection("remove");
		if (entity2.isDead) {
			auto l1 = entity2.chunkCoordX;
			auto i2 = entity2.chunkCoordZ;
			if (entity2.addedToChunk && isChunkLoaded(l1, i2, true)) 
			{
				getChunk(l1, i2).removeEntity(entity2);
			}

			loadedEntityList.erase(loadedEntityList.begin() + i1--);
			onEntityRemoved(entity2);
		}

		profiler.endSection();
	}

	profiler.endStartSection("blockEntities");
	if (!tileEntitiesToBeRemoved.isEmpty()) 
	{
		Util::removeAll(tickableTileEntities, tileEntitiesToBeRemoved);
		Util::removeAll(loadedTileEntityList, tileEntitiesToBeRemoved);
		tileEntitiesToBeRemoved.clear();
	}

	processingLoadedTiles = true;

	for(auto tileentity : tickableTileEntities)
	{
		if (!tileentity.isInvalid() && tileentity.hasWorld()) {
			BlockPos blockpos = tileentity.getPos();
			if (isBlockLoaded(blockpos) && worldBorder.contains(blockpos)) 
			{
				try {
					profiler.func_194340_a(() -> {
						return String.valueOf(TileEntity.getKey(tileentity.getClass()));
					});
					((ITickable)tileentity).update();
					profiler.endSection();
				}
				catch (Throwable var7) {
					crashreport2 = CrashReport.makeCrashReport(var7, "Ticking block entity");
					crashreportcategory2 = crashreport2.makeCategory("Block entity being ticked");
					tileentity.addInfoToCrashReport(crashreportcategory2);
					throw new ReportedException(crashreport2);
				}
			}
		}

		if (tileentity->isInvalid()) 
		{
			if (isBlockLoaded(tileentity->getPos())) 
			{
				getChunk(tileentity->getPos()).removeTileEntity(tileentity->getPos());
			}
		}
	}
	const auto remite = std::remove_if(tickableTileEntities.begin(), tickableTileEntities.end(), [](TileEntity * x) {return x->isInvalid(); });
	tickableTileEntities.erase(remite, tickableTileEntities.end());
	
	processingLoadedTiles = false;
	profiler.endStartSection("pendingBlockEntities");
	if (!addedTileEntityList.empty()) 
	{
		for (auto tileentity1 : addedTileEntityList)
		{
			if (!tileentity1.isInvalid()) {
				if (!loadedTileEntityList.contains(tileentity1)) {
					addTileEntity(tileentity1);
				}

				if (isBlockLoaded(tileentity1.getPos())) {
					auto chunk = getChunk(tileentity1.getPos());
					auto iblockstate = chunk.getBlockState(tileentity1.getPos());
					chunk.addTileEntity(tileentity1.getPos(), tileentity1);
					notifyBlockUpdate(tileentity1.getPos(), iblockstate, iblockstate, 3);
				}
			}
		}

		addedTileEntityList.clear();
	}

	profiler.endSection();
	profiler.endSection();
}

bool World::addTileEntity(TileEntity* tile)
{
	bool flag = loadedTileEntityList.emplace_back(tile);
	if (flag && Util::instanceof<ITickable>(tile)) 
	{
		tickableTileEntities.emplace_back(tile);
	}

	if (isRemote) 
	{
		auto blockpos1 = tile->getPos();
		auto iblockstate1 = getBlockState(blockpos1);
		notifyBlockUpdate(blockpos1, iblockstate1, iblockstate1, 2);
	}

	return flag;
}

void World::addTileEntities(std::initializer_list<TileEntity*> tileEntityCollection)
{
	if (processingLoadedTiles) 
	{
		addedTileEntityList.insert(addedTileEntityList.end(), tileEntityCollection);
	}
	else 
	{
		for(auto tileentity2 : tileEntityCollection)
		{
			addTileEntity(tileentity2);
		}
	}
}

void World::updateEntity(Entity* ent)
{
	updateEntityWithOptionalForce(ent, true);
}

void World::updateEntityWithOptionalForce(Entity* entityIn, bool forceUpdate)
{
	if (!(Util::instanceof<EntityPlayer>(entityIn))) 
	{
		auto i3 = MathHelper::floor(entityIn->posX);
		auto j3 = MathHelper::floor(entityIn->posZ);
		auto l2 = true;
		if (forceUpdate && !isAreaLoaded(i3 - 32, 0, j3 - 32, i3 + 32, 0, j3 + 32, true)) 
		{
			return;
		}
	}

	entityIn.lastTickPosX = entityIn.posX;
	entityIn.lastTickPosY = entityIn.posY;
	entityIn.lastTickPosZ = entityIn.posZ;
	entityIn.prevRotationYaw = entityIn.rotationYaw;
	entityIn.prevRotationPitch = entityIn.rotationPitch;
	if (forceUpdate && entityIn.addedToChunk) {
		++entityIn.ticksExisted;
		if (entityIn.isRiding()) {
			entityIn.updateRidden();
		}
		else {
			entityIn.onUpdate();
		}
	}

	profiler.startSection("chunkCheck");
	if (std::isnan(entityIn.posX) || std::isinf(entityIn.posX)) 
	{
		entityIn.posX = entityIn.lastTickPosX;
	}

	if (std::isnan(entityIn.posY) || std::isinf(entityIn.posY)) 
	{
		entityIn.posY = entityIn.lastTickPosY;
	}

	if (std::isnan(entityIn.posZ) || std::isinf(entityIn.posZ)) 
	{
		entityIn.posZ = entityIn.lastTickPosZ;
	}

	if (std::isnan((double)entityIn.rotationPitch) || std::isinf((double)entityIn.rotationPitch)) 
	{
		entityIn.rotationPitch = entityIn.prevRotationPitch;
	}

	if (std::isnan((double)entityIn.rotationYaw) || std::isinf((double)entityIn.rotationYaw)) 
	{
		entityIn.rotationYaw = entityIn.prevRotationYaw;
	}

	auto i3 = MathHelper::floor(entityIn.posX / 16.0);
	auto j3 = MathHelper::floor(entityIn.posY / 16.0);
	auto k3 = MathHelper::floor(entityIn.posZ / 16.0);
	if (!entityIn.addedToChunk || entityIn.chunkCoordX != i3 || entityIn.chunkCoordY != j3 || entityIn.chunkCoordZ != k3) 
	{
		if (entityIn.addedToChunk && isChunkLoaded(entityIn.chunkCoordX, entityIn.chunkCoordZ, true)) 
		{
			getChunk(entityIn.chunkCoordX, entityIn.chunkCoordZ).removeEntityAtIndex(entityIn, entityIn.chunkCoordY);
		}

		if (!entityIn.setPositionNonDirty() && !isChunkLoaded(i3, k3, true)) 
		{
			entityIn.addedToChunk = false;
		}
		else 
		{
			getChunk(i3, k3).addEntity(entityIn);
		}
	}

	profiler.endSection();
	if (forceUpdate && entityIn.addedToChunk) 
	{
		while (true) 
		{
			for(auto entity4 : entityIn.getPassengers())
			{
				if (!entity4.isDead && entity4.getRidingEntity() == entityIn)
				{
					updateEntity(entity4);
				}
				else 
				{
					entity4.dismountRidingEntity();
				}
			}

			return;
		}
	}
}

bool World::checkNoEntityCollision(AxisAlignedBB& bb)
{
	return checkNoEntityCollision(bb, std::nullopt);
}

bool World::checkBlockCollision(AxisAlignedBB& bb)
{
	auto j2 = MathHelper::floor(bb.getminX());
	auto k2 = MathHelper::ceil(bb.getmaxX());
	auto l2 = MathHelper::floor(bb.getminY());
	auto i3 = MathHelper::ceil(bb.getmaxY());
	auto j3 = MathHelper::floor(bb.getminZ());
	auto k3 = MathHelper::ceil(bb.getmaxZ());
	PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

	for (auto l3 = j2; l3 < k2; ++l3) 
	{
		for (auto i4 = l2; i4 < i3; ++i4) 
		{
			for (auto j4 = j3; j4 < k3; ++j4) 
			{
				auto iblockstate1 = getBlockState(pooledmutableblockpos.setPos(l3, i4, j4));
				if (iblockstate1->getMaterial() != Material::AIR) 
				{
					pooledmutableblockpos.release();
					return true;
				}
			}
		}
	}

	pooledmutableblockpos.release();
	return false;
}

bool World::containsAnyLiquid(AxisAlignedBB& bb)
{
	auto j2 = MathHelper::floor(bb.getminX());
	auto k2 = MathHelper::ceil(bb.getmaxX());
	auto l2 = MathHelper::floor(bb.getminY());
	auto i3 = MathHelper::ceil(bb.getmaxY());
	auto j3 = MathHelper::floor(bb.getminZ());
	auto k3 = MathHelper::ceil(bb.getmaxZ());
	PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

	for (auto l3 = j2; l3 < k2; ++l3) 
	{
		for (auto i4 = l2; i4 < i3; ++i4) 
		{
			for (auto j4 = j3; j4 < k3; ++j4) 
			{
				auto iblockstate1 = getBlockState(pooledmutableblockpos.setPos(l3, i4, j4));
				if (iblockstate1->getMaterial().isLiquid()) 
				{
					pooledmutableblockpos.release();
					return true;
				}
			}
		}
	}

	blockpos$pooledmutableblockpos.release();
	return false;
}

bool World::isFlammableWithin(AxisAlignedBB& bb)
{
	auto j2 = MathHelper::floor(bb.getminX());
	auto k2 = MathHelper::ceil(bb.getmaxX());
	auto l2 = MathHelper::floor(bb.getminY());
	auto i3 = MathHelper::ceil(bb.getmaxY());
	auto j3 = MathHelper::floor(bb.getminZ());
	auto k3 = MathHelper::ceil(bb.getmaxZ());
	if (isAreaLoaded(j2, l2, j3, k2, i3, k3, true)) 
	{
		PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();
		auto l3 = j2;

		while (true) 
		{
			if (l3 >= k2) 
			{
				pooledmutableblockpos.release();
				break;
			}

			for (auto i4 = l2; i4 < i3; ++i4) 
			{
				for (auto j4 = j3; j4 < k3; ++j4) 
				{
					auto block = getBlockState(pooledmutableblockpos.setPos(l3, i4, j4)).getBlock();
					if (block == Blocks::FIRE || block == Blocks::FLOWING_LAVA || block == Blocks::LAVA) 
					{
						pooledmutableblockpos.release();
						return true;
					}
				}
			}

			++l3;
		}
	}

	return false;
}

bool World::handleMaterialAcceleration(AxisAlignedBB& bb, Material& materialIn, Entity* entityIn)
{
	auto j2 = MathHelper::floor(bb.getminX());
	auto k2 = MathHelper::ceil(bb.getmaxX());
	auto l2 = MathHelper::floor(bb.getminY());
	auto i3 = MathHelper::ceil(bb.getmaxY());
	auto j3 = MathHelper::floor(bb.getminZ());
	auto k3 = MathHelper::ceil(bb.getmaxZ());
	if (!isAreaLoaded(j2, l2, j3, k2, i3, k3, true)) 
	{
		return false;
	}
	else 
	{
		bool flag = false;
		Vec3d vec3d = Vec3d::ZERO;
		PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

		for (auto l3 = j2; l3 < k2; ++l3) 
		{
			for (auto i4 = l2; i4 < i3; ++i4) 
			{
				for (auto j4 = j3; j4 < k3; ++j4) 
				{
					pooledmutableblockpos.setPos(l3, i4, j4);
					auto iblockstate1 = getBlockState(pooledmutableblockpos);
					auto block = iblockstate1.getBlock();
					if (iblockstate1.getMaterial() == materialIn) 
					{
						double d0 = (double)((float)(i4 + 1) - BlockLiquid.getLiquidHeightPercent((Integer)iblockstate1.getValue(BlockLiquid.LEVEL)));
						if ((double)i3 >= d0) 
						{
							flag = true;
							vec3d = block.modifyAcceleration(this, pooledmutableblockpos, entityIn, vec3d);
						}
					}
				}
			}
		}

		pooledmutableblockpos.release();
		if (vec3d.length() > 0.0 && entityIn.isPushedByWater()) 
		{
			vec3d = vec3d.normalize();
			double d1 = 0.014;
			entityIn.motionX += vec3d.getx() * 0.014;
			entityIn.motionY += vec3d.gety() * 0.014;
			entityIn.motionZ += vec3d.getz() * 0.014;
		}

		return flag;
	}
}

bool World::isMaterialInBB(AxisAlignedBB& bb, Material& materialIn)
{
	auto j2 = MathHelper::floor(bb.getminX());
	auto k2 = MathHelper::ceil(bb.getmaxX());
	auto l2 = MathHelper::floor(bb.getminY());
	auto i3 = MathHelper::ceil(bb.getmaxY());
	auto j3 = MathHelper::floor(bb.getminZ());
	auto k3 = MathHelper::ceil(bb.getmaxZ());
	PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

	for (auto l3 = j2; l3 < k2; ++l3)
	{
		for (auto i4 = l2; i4 < i3; ++i4)
		{
			for (auto j4 = j3; j4 < k3; ++j4)
			{
				if (getBlockState(pooledmutableblockpos.setPos(l3, i4, j4)).getMaterial() == materialIn) 
				{
					pooledmutableblockpos.release();
					return true;
				}
			}
		}
	}

	pooledmutableblockpos.release();
	return false;
}

Explosion World::createExplosion(Entity* entityIn, double x, double y, double z, float strength, bool damagesTerrain)
{
	return newExplosion(entityIn, x, y, z, strength, false, damagesTerrain);
}

Explosion World::newExplosion(Entity* entityIn, double x, double y, double z, float strength, bool causesFire,
	bool damagesTerrain)
{
	Explosion explosion(this, entityIn, x, y, z, strength, causesFire, damagesTerrain);
	explosion.doExplosionA();
	explosion.doExplosionB(true);
	return explosion;
}

float World::getBlockDensity(Vec3d vec, AxisAlignedBB& bb)
{
	double d0 = 1.0 / ((bb.getmaxX() - bb.getminX()) * 2.0 + 1.0);
	double d1 = 1.0 / ((bb.getmaxY() - bb.getminY()) * 2.0 + 1.0);
	double d2 = 1.0 / ((bb.getmaxZ() - bb.getminZ()) * 2.0 + 1.0);
	double d3 = (1.0 - MathHelper::floor(1.0 / d0) * d0) / 2.0;
	double d4 = (1.0 - MathHelper::floor(1.0 / d2) * d2) / 2.0;
	if (d0 >= 0.0 && d1 >= 0.0 && d2 >= 0.0) 
	{
		auto j2 = 0;
		auto k2 = 0;
		for (float f = 0.0F; f <= 1.0F; f = (float)((double)f + d0)) 
		{
			for (float f1 = 0.0F; f1 <= 1.0F; f1 = (float)((double)f1 + d1)) 
			{
				for (float f2 = 0.0F; f2 <= 1.0F; f2 = (float)((double)f2 + d2)) 
				{
					double d5 = bb.getminX() + (bb.getmaxX() - bb.getminX()) * (double)f;
					double d6 = bb.getminY() + (bb.getmaxY() - bb.getminY()) * (double)f1;
					double d7 = bb.getminZ() + (bb.getmaxZ() - bb.getminZ()) * (double)f2;
					if (!rayTraceBlocks(Vec3d(d5 + d3, d6, d7 + d4), vec)) {
						++j2;
					}

					++k2;
				}
			}
		}

		return (float)j2 / (float)k2;
	}
	else {
		return 0.0F;
	}
}

bool World::extinguishFire(EntityPlayer* player, BlockPos& pos, EnumFacing side)
{
	pos = pos.offset(side);
	if (getBlockState(pos).getBlock() == Blocks::FIRE) 
	{
		playEvent(player, 1009, pos, 0);
		setBlockToAir(pos);
		return true;
	}
	else 
	{
		return false;
	}
}

std::string World::getDebugLoadedEntities()
{
	return "All: " + loadedEntityList.size();
}

std::string World::getProviderName()
{
	return chunkProvider.makeString();
}

TileEntity* World::getTileEntity(BlockPos& pos)
{
	if (isOutsideBuildHeight(pos)) 
	{
		return nullptr;
	}
	else 
	{
		TileEntity* tileentity2 = nullptr;
		if (processingLoadedTiles) 
		{
			tileentity2 = getPendingTileEntityAt(pos);
		}

		if (tileentity2 == nullptr)
		{
			tileentity2 = getChunk(pos).getTileEntity(pos, Chunk.EnumCreateEntityType.IMMEDIATE);
		}

		if (tileentity2 == nullptr)
		{
			tileentity2 = getPendingTileEntityAt(pos);
		}

		return tileentity2;
	}
}

TileEntity* World::getPendingTileEntityAt(BlockPos& pos)
{
	for(auto tileentity2 : addedTileEntityList)
	{
		if (!tileentity2.isInvalid() && tileentity2.getPos().equals(pos)) 
		{
			return tileentity2;
		}
	}

	return nullptr;
}

void World::setTileEntity(BlockPos& pos, TileEntity* tileEntityIn)
{
	if (!isOutsideBuildHeight(pos) && tileEntityIn != nullptr && !tileEntityIn.isInvalid()) {
		if (processingLoadedTiles) 
		{
			tileEntityIn.setPos(pos);

			addedTileEntityList.erase(std::remove_if(addedTileEntityList.begin(), addedTileEntityList.end(), [&](TileEntity* x)
			                          {
				                          if (x->getPos() == pos)
				                          {
					                          x->invalidate();
					                          return true;
				                          }
				                          return false;
			                          }),
			                          addedTileEntityList.end());

			addedTileEntityList.emplace_back(tileEntityIn);
		}
		else 
		{
			getChunk(pos).addTileEntity(pos, tileEntityIn);
			addTileEntity(tileEntityIn);
		}
	}
}

void World::removeTileEntity(BlockPos& pos)
{
	auto tileentity2 = getTileEntity(pos);
	if (tileentity2 != nullptr && processingLoadedTiles) 
	{
		tileentity2->invalidate();
		addedTileEntityList.erase(std::find(addedTileEntityList.begin(), addedTileEntityList.end(), tileentity2));
	}
	else 
	{
		if (tileentity2 != nullptr) 
		{
			addedTileEntityList.erase(std::find(addedTileEntityList.begin(), addedTileEntityList.end(), tileentity2));
			loadedTileEntityList.erase(std::find(loadedTileEntityList.begin(), loadedTileEntityList.end(), tileentity2));
			tickableTileEntities.erase(std::find(tickableTileEntities.begin(), tickableTileEntities.end(), tileentity2));
		}

		getChunk(pos).removeTileEntity(pos);
	}
}

void World::markTileEntityForRemoval(TileEntity* tileEntityIn)
{
	tileEntitiesToBeRemoved.emplace_back(tileEntityIn);
}

bool World::isBlockFullCube(BlockPos& pos)
{
	auto axisalignedbb = getBlockState(pos).getCollisionBoundingBox(this, pos);
	return axisalignedbb.value() != Block::NULL_AABB && axisalignedbb.getAverageEdgeLength() >= 1.0;
}

bool World::isBlockNormalCube(BlockPos& pos, bool _default)
{
	if (isOutsideBuildHeight(pos)) 
	{
		return false;
	}
	else 
	{
		auto chunk1 = chunkProvider.getLoadedChunk(pos.getx() >> 4, pos.getz() >> 4);
		if (chunk1 != nullptr && !chunk1.empty()) 
		{
			auto iblockstate1 = getBlockState(pos);
			return iblockstate1.getMaterial().isOpaque() && iblockstate1.isFullCube();
		}
		else 
		{
			return _default;
		}
	}
}

void World::calculateInitialSkylight()
{
	auto j2 = calculateSkylightSubtracted(1.0F);
	if (j2 != skylightSubtracted) {
		skylightSubtracted = j2;
	}
}

void World::setAllowedSpawnTypes(bool hostile, bool peaceful)
{
	spawnHostileMobs = hostile;
	spawnPeacefulMobs = peaceful;
}

void World::tick()
{
	updateWeather();
}

void World::immediateBlockTick(BlockPos& pos, IBlockState* state, pcg32& random)
{
	scheduledUpdatesAreImmediate = true;
	state->getBlock().updateTick(this, pos, state, random);
	scheduledUpdatesAreImmediate = false;
}

bool World::canBlockFreezeWater(BlockPos& pos)
{
	return canBlockFreeze(pos, false);
}

bool World::canBlockFreezeNoWater(BlockPos& pos)
{
	return canBlockFreeze(pos, true);
}

bool World::canBlockFreeze(BlockPos& pos, bool noWaterAdj)
{
	auto biome = getBiome(pos);
	float f = biome.getTemperature(pos);
	if (f >= 0.15F) 
	{
		return false;
	}
	else 
	{
		if (pos.gety() >= 0 && pos.gety() < 256 && getLightFor(EnumSkyBlock::BLOCK, pos) < 10) {
			auto iblockstate1 = getBlockState(pos);
			Block block = iblockstate1->getBlock();
			if ((block == Blocks::WATER || block == Blocks::FLOWING_WATER) && iblockstate1.getValue(BlockLiquid::LEVEL) == 0) 
			{
				if (!noWaterAdj) 
				{
					return true;
				}

				bool flag = isWater(pos.west()) && isWater(pos.east()) && isWater(pos.north()) && isWater(pos.south());
				if (!flag) 
				{
					return true;
				}
			}
		}

		return false;
	}
}

bool World::canSnowAt(BlockPos& pos, bool checkLight)
{
	auto biome = getBiome(pos);
	float f = biome.getTemperature(pos);
	if (f >= 0.15F) 
	{
		return false;
	}
	else if (!checkLight) 
	{
		return true;
	}
	else 
	{
		if (pos.gety() >= 0 && pos.gety() < 256 && getLightFor(EnumSkyBlock::BLOCK, pos) < 10) 
		{
			auto iblockstate1 = getBlockState(pos);
			if (iblockstate1.getMaterial() == Material::AIR && Blocks::SNOW_LAYER.canPlaceBlockAt(this, pos)) 
			{
				return true;
			}
		}

		return false;
	}
}

bool World::checkLight(BlockPos& pos)
{
	bool flag = false;
	if (provider.hasSkyLight()) 
	{
		flag |= checkLightFor(EnumSkyBlock.SKY, pos);
	}

	flag |= checkLightFor(EnumSkyBlock.BLOCK, pos);
	return flag;
}

bool World::checkNoEntityCollision(AxisAlignedBB& bb, std::optional<Entity*> entityIn)
{
	auto list = getEntitiesWithinAABBExcludingEntity(std::nullopt, bb);

	for(auto entity4 : list)
	{
		if (!entity4.isDead && entity4.preventEntitySpawning && entity4 != entityIn && (!entityIn|| entity4.isRidingSameEntity(entityIn))) 
		{
			return false;
		}
	}

	return true;
}

int32_t World::getRawLight(BlockPos pos, EnumSkyBlock lightType)
{
	if (lightType == EnumSkyBlock.SKY && canSeeSky(pos)) 
	{
		return 15;
	}
	else 
	{
		auto iblockstate1 = getBlockState(pos);
		auto j2 = lightType == EnumSkyBlock.SKY ? 0 : iblockstate1.getLightValue();
		auto k2 = iblockstate1.getLightOpacity();
		if (k2 >= 15 && iblockstate1.getLightValue() > 0) 
		{
			k2 = 1;
		}

		if (k2 < 1) 
		{
			k2 = 1;
		}

		if (k2 >= 15) 
		{
			return 0;
		}
		else if (j2 >= 14) 
		{
			return j2;
		}
		else 
		{
			PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

			try 
			{
				for (auto enumfacing : EnumFacing::values())
				{
					pooledmutableblockpos.setPos(pos).move(enumfacing);
					auto l2 = getLightFor(lightType, pooledmutableblockpos) - k2;
					if (l2 > j2) 
					{
						j2 = l2;
					}

					if (j2 >= 14) 
					{
						auto var13 = j2;
						return var13;
					}
				}

				auto var17 = j2;
				return var17;
			}
			catch(std::exception& ex)
			{
				pooledmutableblockpos.release();
			}
		}
	}
}

bool World::checkLightFor(EnumSkyBlock lightType, BlockPos& pos)
{
	if (!isAreaLoaded(pos, 17, false)) 
	{
		return false;
	}
	else 
	{
		auto j2 = 0;
		auto k2 = 0;
		profiler.startSection("getBrightness");
		auto l2 = getLightFor(lightType, pos);
		auto i3 = getRawLight(pos, lightType);
		auto j3 = pos.getx();
		auto k3 = pos.gety();
		auto l3 = pos.getz();
		auto i4 = 0;
		auto k7 = 0;
		auto l7 = 0;
		auto i8 = 0;
		auto k8 = 0;
		auto k5 = 0;
		auto l5 = 0;
		auto i6 = 0;
		if (i3 > l2) 
		{
			lightUpdateBlockList[k2++] = 133152;
		}
		else if (i3 < l2) 
		{
			lightUpdateBlockList[k2++] = 133152 | l2 << 18;

		label92:
			while (true) 
			{
				int i5;
				do 
				{
					do 
					{
						BlockPos blockpos1;
						do 
						{
							if (j2 >= k2) {
								j2 = 0;
								break label92;
							}

							i4 = lightUpdateBlockList[j2++];
							k7 = (i4 & 63) - 32 + j3;
							l7 = (i4 >> 6 & 63) - 32 + k3;
							i8 = (i4 >> 12 & 63) - 32 + l3;
							i5 = i4 >> 18 & 15;
							blockpos1 = BlockPos(k7, l7, i8);
							k8 = getLightFor(lightType, blockpos1);
						} 
						while (k8 != i5);

						setLightFor(lightType, blockpos1, 0);
					} 
					while (i5 <= 0);

					k5 = MathHelper::abs(k7 - j3);
					l5 = MathHelper::abs(l7 - k3);
					i6 = MathHelper::abs(i8 - l3);
				} 
				while (k5 + l5 + i6 >= 17);

				PooledMutableBlockPos pooledmutableblockpos = PooledMutableBlockPos::retain();

				for(auto enumfacing : EnumFacing::values())
				{
					auto j6 = k7 + enumfacing.getXOffset();
					auto k6 = l7 + enumfacing.getYOffset();
					auto l6 = i8 + enumfacing.getZOffset();
					pooledmutableblockpos.setPos(j6, k6, l6);
					auto i7 = MathHelper::max(1, getBlockState(pooledmutableblockpos).getLightOpacity());
					k8 = getLightFor(lightType, pooledmutableblockpos);
					if (k8 == i5 - i7 && k2 < lightUpdateBlockList.size()) 
					{
						lightUpdateBlockList[k2++] = j6 - j3 + 32 | k6 - k3 + 32 << 6 | l6 - l3 + 32 << 12 | i5 - i7 << 18;
					}
				}

				pooledmutableblockpos.release();
			}
		}

		profiler.endSection();
		profiler.startSection("checkedPosition < toCheckCount");

		while (j2 < k2) 
		{
			i4 = lightUpdateBlockList[j2++];
			k7 = (i4 & 63) - 32 + j3;
			l7 = (i4 >> 6 & 63) - 32 + k3;
			i8 = (i4 >> 12 & 63) - 32 + l3;
			BlockPos blockpos2(k7, l7, i8);
			auto j8 = getLightFor(lightType, blockpos2);
			k8 = getRawLight(blockpos2, lightType);
			if (k8 != j8) 
			{
				setLightFor(lightType, blockpos2, k8);
				if (k8 > j8) 
				{
					k5 = MathHelper::abs(k7 - j3);
					l5 = MathHelper::abs(l7 - k3);
					i6 = MathHelper::abs(i8 - l3);
					bool flag = k2 < lightUpdateBlockList.size() - 6;
					if (k5 + l5 + i6 < 17 && flag) 
					{
						if (getLightFor(lightType, blockpos2.west()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 - 1 - j3 + 32 + (l7 - k3 + 32 << 6) + (i8 - l3 + 32 << 12);
						}

						if (getLightFor(lightType, blockpos2.east()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 + 1 - j3 + 32 + (l7 - k3 + 32 << 6) + (i8 - l3 + 32 << 12);
						}

						if (getLightFor(lightType, blockpos2.down()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 - j3 + 32 + (l7 - 1 - k3 + 32 << 6) + (i8 - l3 + 32 << 12);
						}

						if (getLightFor(lightType, blockpos2.up()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 - j3 + 32 + (l7 + 1 - k3 + 32 << 6) + (i8 - l3 + 32 << 12);
						}

						if (getLightFor(lightType, blockpos2.north()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 - j3 + 32 + (l7 - k3 + 32 << 6) + (i8 - 1 - l3 + 32 << 12);
						}

						if (getLightFor(lightType, blockpos2.south()) < k8) 
						{
							lightUpdateBlockList[k2++] = k7 - j3 + 32 + (l7 - k3 + 32 << 6) + (i8 + 1 - l3 + 32 << 12);
						}
					}
				}
			}
		}

		profiler.endSection();
		return true;
	}
}

bool World::tickUpdates(bool runAllPending)
{
	return false;
}

std::optional<> World::getPendingBlockUpdates(Chunk& chunkIn, bool remove)
{
	return std::nullopt;
}

std::optional<> World::getEntitiesWithinAABBExcludingEntity(Entity* entityIn, AxisAlignedBB& bb)
{
	return getEntitiesInAABBexcluding(entityIn, bb, EntitySelectors::NOT_SPECTATING);
}

std::optional<> World::getPendingBlockUpdates(StructureBoundingBox& structureBB, bool remove)
{
	return std::nullopt;
}

std::optional<> World::getEntitiesInAABBexcluding(Entity* entityIn, AxisAlignedBB& boundingBox, Predicate predicate)
{
	List list = Lists.newArrayList();
	auto j2 = MathHelper::floor((boundingBox.getminX() - 2.0) / 16.0);
	auto k2 = MathHelper::floor((boundingBox.getmaxX() + 2.0) / 16.0);
	auto l2 = MathHelper::floor((boundingBox.getminZ() - 2.0) / 16.0);
	auto i3 = MathHelper::floor((boundingBox.getmaxZ() + 2.0) / 16.0);

	for (auto j3 = j2; j3 <= k2; ++j3) 
	{
		for (auto k3 = l2; k3 <= i3; ++k3) 
		{
			if (isChunkLoaded(j3, k3, true)) 
			{
				getChunk(j3, k3).getEntitiesWithinAABBForEntity(entityIn, boundingBox, list, predicate);
			}
		}
	}

	return list;
}

Entity* World::getEntityByID(int32_t id)
{
	const auto ite = entitiesById.find(id);
	if (ite != entitiesById.end())
	{
		return ite->second;
	}
	else
		return nullptr;
}

std::vector<Entity*> World::getLoadedEntityList() const
{
	return loadedEntityList;
}

void World::markChunkDirty(BlockPos& pos, TileEntity* unusedTileEntity)
{
	if (isBlockLoaded(pos)) 
	{
		getChunk(pos).markDirty();
	}
}

void World::loadEntities(std::initializer_list<Entity*> entityCollection)
{
	loadedEntityList.insert(loadedEntityList.end(), entityCollection);

	for(auto entity4 : entityCollection)
	{
		onEntityAdded(entity4);
	}

}

void World::unloadEntities(std::initializer_list<Entity*> entityCollection)
{
	unloadedEntityList.insert(unloadedEntityList.end(),entityCollection);
}

bool World::mayPlace(Block* blockIn, BlockPos& pos, bool skipCollisionCheck, EnumFacing sidePlacedOn, Entity* placer)
{
	auto iblockstate1 = getBlockState(pos);
	auto axisalignedbb = skipCollisionCheck ? nullptr : blockIn->getDefaultState().getCollisionBoundingBox(this, pos);
	if (axisalignedbb != Block::NULL_AABB && !checkNoEntityCollision(axisalignedbb.offset(pos), placer)) 
	{
		return false;
	}
	else if (iblockstate1->getMaterial() == Material::CIRCUITS && blockIn == Blocks::ANVIL)
	{
		return true;
	}
	else 
	{
		return iblockstate1->getMaterial().isReplaceable() && blockIn.canPlaceBlockOnSide(this, pos, sidePlacedOn);
	}
}

uint32_t World::getSeaLevel() const
{
	return seaLevel;
}

void World::setSeaLevel(uint32_t seaLevelIn)
{
	seaLevel = seaLevelIn;
}

int32_t World::getStrongPower(BlockPos& pos, EnumFacing direction)
{
	return getBlockState(pos).getStrongPower(this, pos, direction);
}

WorldType World::getWorldType()
{
	return worldInfo.getTerrainType();
}

int32_t World::getStrongPower(BlockPos& pos)
{
	auto j2 = 0;
	j2 = MathHelper::max(j2, getStrongPower(pos.down(), EnumFacing::DOWN));
	if (j2 >= 15) 
	{
		return j2;
	}
	else 
	{
		j2 = MathHelper::max(j2, getStrongPower(pos.up(), EnumFacing::UP));
		if (j2 >= 15) 
		{
			return j2;
		}
		else 
		{
			j2 = MathHelper::max(j2, getStrongPower(pos.north(), EnumFacing::NORTH));
			if (j2 >= 15) 
			{
				return j2;
			}
			else 
			{
				j2 = MathHelper::max(j2, getStrongPower(pos.south(), EnumFacing::SOUTH));
				if (j2 >= 15) 
				{
					return j2;
				}
				else 
				{
					j2 = MathHelper::max(j2, getStrongPower(pos.west(), EnumFacing::WEST));
					if (j2 >= 15) 
					{
						return j2;
					}
					else 
					{
						j2 = MathHelper::max(j2, getStrongPower(pos.east(), EnumFacing::EAST));
						return j2 >= 15 ? j2 : j2;
					}
				}
			}
		}
	}
}

bool World::isSidePowered(BlockPos& pos, EnumFacing side)
{
	return getRedstonePower(pos, side) > 0;
}

int32_t World::getRedstonePower(BlockPos& pos, EnumFacing facing)
{
	auto iblockstate1 = getBlockState(pos);
	return iblockstate1.isNormalCube() ? getStrongPower(pos) : iblockstate1.getWeakPower(this, pos, facing);
}

bool World::isBlockPowered(BlockPos& pos)
{
	if (getRedstonePower(pos.down(), EnumFacing::DOWN) > 0) {
		return true;
	}
	else if (getRedstonePower(pos.up(), EnumFacing::UP) > 0) {
		return true;
	}
	else if (getRedstonePower(pos.north(), EnumFacing::NORTH) > 0) {
		return true;
	}
	else if (getRedstonePower(pos.south(), EnumFacing::SOUTH) > 0) {
		return true;
	}
	else if (getRedstonePower(pos.west(), EnumFacing::WEST) > 0) {
		return true;
	}
	else {
		return getRedstonePower(pos.east(), EnumFacing::EAST) > 0;
	}
}

int32_t World::getRedstonePowerFromNeighbors(BlockPos& pos)
{
	auto j2 = 0;

	for(auto enumfacing : EnumFacing::values())
	{
		auto k2 = getRedstonePower(pos.offset(enumfacing), enumfacing);
		if (k2 >= 15) 
		{
			return 15;
		}

		if (k2 > j2) 
		{
			j2 = k2;
		}
	}

	return j2;
}

EntityPlayer* World::getClosestPlayerToEntity(Entity* entityIn, double distance)
{
	return getClosestPlayer(entityIn.posX, entityIn.posY, entityIn.posZ, distance, false);
}

EntityPlayer* World::getNearestPlayerNotCreative(Entity* entityIn, double distance)
{
	return getClosestPlayer(entityIn.posX, entityIn.posY, entityIn.posZ, distance, true);
}

EntityPlayer* World::getClosestPlayer(double posX, double posY, double posZ, double distance, bool spectator)
{
	const auto predicate = spectator ? EntitySelectors::CAN_AI_TARGET : EntitySelectors::NOT_SPECTATING;
	return getClosestPlayer(posX, posY, posZ, distance, predicate);
}

bool World::isAnyPlayerWithinRangeAt(double x, double y, double z, double range)
{
	for (auto entityplayer : playerEntities)
	{
		if (EntitySelectors::NOT_SPECTATING(entityplayer)) 
		{
			double d0 = entityplayer.getDistanceSq(x, y, z);
			if (range < 0.0 || d0 < range * range) 
			{
				return true;
			}
		}
	}

	return false;
}

EntityPlayer* World::getNearestAttackablePlayer(Entity* entityIn, double maxXZDistance, double maxYDistance)
{
	return getNearestAttackablePlayer(entityIn.posX, entityIn.posY, entityIn.posZ, maxXZDistance, maxYDistance, std::nullopt, std::nullopt);
}

EntityPlayer* World::getNearestAttackablePlayer(BlockPos& pos, double maxXZDistance, double maxYDistance)
{
	return getNearestAttackablePlayer((double)((float)pos.getx() + 0.5F), (double)((float)pos.gety() + 0.5F), (double)((float)pos.getz() + 0.5F), maxXZDistance, maxYDistance, std::nullopt, std::nullopt);
}

EntityPlayer* World::getPlayerEntityByName(std::string name)
{
	for (auto entityplayer : playerEntities)
	{
		if (name == (entityplayer.getName())) 
		{
			return entityplayer;
		}
	}

	return nullptr;
}

EntityPlayer* World::getPlayerEntityByUUID(xg::Guid& uuid)
{
	for (auto entityplayer : playerEntities) 
	{
		if (uuid == (entityplayer.getUniqueID())) 
		{
			return entityplayer;
		}
	}

	return nullptr;
}

void World::sendQuittingDisconnectingPacket()
{
}

void World::checkSessionLock()
{
	saveHandler.checkSessionLock();
}

void World::setTotalWorldTime(int64_t worldTime)
{
	worldInfo.setWorldTotalTime(worldTime);
}

int64_t World::getSeed()
{
	return worldInfo.getSeed();
}

int64_t World::getTotalWorldTime()
{
	return worldInfo.getWorldTotalTime();
}

int64_t World::getWorldTime()
{
	return worldInfo.getWorldTime();
}

void World::setWorldTime(int64_t time)
{
	worldInfo.setWorldTime(time);
}

BlockPos World::getSpawnPoint()
{
	BlockPos blockpos1(worldInfo.getSpawnX(), worldInfo.getSpawnY(), worldInfo.getSpawnZ());
	if (!getWorldBorder().contains(blockpos1)) 
	{
		blockpos1 = getHeight(BlockPos(getWorldBorder().getCenterX(), 0.0, getWorldBorder().getCenterZ()));
	}

	return blockpos1;
}

void World::setSpawnPoint(BlockPos& pos)
{
	worldInfo.setSpawn(pos);
}

void World::joinEntityInSurroundings(Entity* entityIn)
{
	auto j2 = MathHelper::floor(entityIn.posX / 16.0);
	auto k2 = MathHelper::floor(entityIn.posZ / 16.0);
	auto l2 = true;

	for (auto i3 = -2; i3 <= 2; ++i3)
	{
		for (auto j3 = -2; j3 <= 2; ++j3)
		{
			getChunk(j2 + i3, k2 + j3);
		}
	}

	if (!loadedEntityList.contains(entityIn)) 
	{
		loadedEntityList.emplace_back(entityIn);
	}
}

bool World::isBlockModifiable(EntityPlayer* player, BlockPos& pos)
{
	return true;
}

void World::setEntityState(Entity* entityIn, char state)
{
}

IChunkProvider World::getChunkProvider()
{
	return chunkProvider;
}

void World::addBlockEvent(BlockPos& pos, Block* blockIn, int32_t eventID, int32_t eventParam)
{
	getBlockState(pos).onBlockEventReceived(this, pos, eventID, eventParam);
}

ISaveHandler World::getSaveHandler()
{
	return saveHandler;
}

WorldInfo World::getWorldInfo()
{
	return worldInfo;
}

GameRules World::getGameRules()
{
	return worldInfo.getGameRulesInstance();
}

void World::updateAllPlayersSleepingFlag()
{
}

float World::getThunderStrength(float delta)
{
	return (prevThunderingStrength + (thunderingStrength - prevThunderingStrength) * delta) * getRainStrength(delta);
}

void World::setThunderStrength(float strength)
{
	prevThunderingStrength = strength;
	thunderingStrength = strength;
}

float World::getRainStrength(float delta)
{
	return prevRainingStrength + (rainingStrength - prevRainingStrength) * delta;
}

void World::setRainStrength(float strength)
{
	prevRainingStrength = strength;
	rainingStrength = strength;
}

bool World::isThundering()
{
	return (double)getThunderStrength(1.0F) > 0.9;
}

bool World::isRaining()
{
	return (double)getRainStrength(1.0F) > 0.2;
}

bool World::isRainingAt(BlockPos& position)
{
	if (!isRaining()) 
	{
		return false;
	}
	else if (!canSeeSky(position))
	{
		return false;
	}
	else if (getPrecipitationHeight(position).gety() > position.gety()) 
	{
		return false;
	}
	else 
	{
		auto biome = getBiome(position);
		if (biome.getEnableSnow()) 
		{
			return false;
		}
		else 
		{
			return canSnowAt(position, false) ? false : biome.canRain();
		}
	}
}

bool World::isBlockinHighHumidity(BlockPos& pos)
{
	auto biome = getBiome(pos);
	return biome.isHighHumidity();
}

MapStorage* World::getMapStorage()
{
	return mapStorage;
}

void World::setData(std::string dataID, WorldSavedData worldSavedDataIn)
{
	mapStorage.setData(dataID, worldSavedDataIn);
}

int32_t World::getUniqueDataId(std::string key)
{
	return mapStorage.getUniqueDataId(key);
}

void World::playBroadcastSound(int32_t id, BlockPos& pos, int32_t data)
{
	for (auto ev : eventListeners)
	{
		ev.broadcastSound(id, pos, data);
	}
}

void World::playEvent(int32_t type, BlockPos& pos, int32_t data)
{
	playEvent(nullptr, type, pos, data);
}

void World::playEvent(EntityPlayer* player, int32_t type, BlockPos& pos, int32_t data)
{
	try {
		for (auto ev : eventListeners)
		{
			ev.playEvent(player, type, pos, data);
		}

	}
	catch (Throwable var8) {
		CrashReport crashreport3 = CrashReport.makeCrashReport(var8, "Playing level event");
		CrashReportCategory crashreportcategory3 = crashreport3.makeCategory("Level event being played");
		crashreportcategory3.addCrashSection("Block coordinates", CrashReportCategory.getCoordinateInfo(pos));
		crashreportcategory3.addCrashSection("Event source", player);
		crashreportcategory3.addCrashSection("Event type", type);
		crashreportcategory3.addCrashSection("Event data", data);
		throw new ReportedException(crashreport3);
	}
}

int32_t World::getHeight() const
{
	return 256;
}

int32_t World::getActualHeight()
{
	return provider.isNether() ? 128 : 256;
}

pcg32& World::setRandomSeed(int32_t seedX, int32_t seedY, int32_t seedZ)
{
	uint64_t j2 = static_cast<uint64_t>(seedX) * 341873128712 + static_cast<uint64_t>(seedY) * 132897987541 + getWorldInfo().getSeed() + static_cast<uint64_t>(seedZ);
	rand.seed(j2);
	return rand;
}

double World::getHorizon()
{
	return worldInfo.getTerrainType() == WorldType::FLAT ? 0.0 : 63.0;
}

void World::sendBlockBreakProgress(int32_t breakerId, BlockPos& pos, int32_t progress)
{
	for (auto iworldeventlistener : eventListeners)
	{
		iworldeventlistener.sendBlockBreakProgress(breakerId, pos, progress);
	}
}

Calendar World::getCurrentDate()
{
	if (getTotalWorldTime() % 600L == 0L) {
		calendar.setTimeInMillis(MinecraftServer.getCurrentTimeMillis());
	}

	return calendar;
}

void World::makeFireworks(double x, double y, double z, double motionX, double motionY, double motionZ,
	NBTTagCompound* compound)
{
}

Scoreboard& World::getScoreboard()
{
	return worldScoreboard;
}

EnumDifficulty World::getDifficulty()
{
	return getWorldInfo().getDifficulty();
}

int32_t World::getSkylightSubtracted()
{
	return skylightSubtracted;
}

void World::setSkylightSubtracted(int32_t newSkylightSubtracted)
{
	skylightSubtracted = newSkylightSubtracted;
}

int32_t World::getLastLightningBolt()
{
	return lastLightningBolt;
}

void World::setLastLightningBolt(int32_t lastLightningBoltIn)
{
	lastLightningBolt = lastLightningBoltIn;
}

VillageCollection World::getVillageCollection()
{
	return villageCollection;
}

DifficultyInstance World::getDifficultyForLocation(BlockPos& pos)
{
	int64_t j2 = 0L;
	float f = 0.0F;
	if (isBlockLoaded(pos)) 
	{
		f = getCurrentMoonPhaseFactor();
		j2 = getChunk(pos).getInhabitedTime();
	}

	return DifficultyInstance(getDifficulty(), getWorldTime(), j2, f);
}

void World::updateComparatorOutputLevel(BlockPos& pos, Block* blockIn)
{
	for (auto enumfacing : Plane::HORIZONTAL)
	{
		auto blockpos1 = pos.offset(enumfacing);
		if (isBlockLoaded(blockpos1)) 
		{
			auto iblockstate1 = getBlockState(blockpos1);
			if (Blocks::UNPOWERED_COMPARATOR.isSameDiode(iblockstate1)) {
				iblockstate1.neighborChanged(this, blockpos1, blockIn, pos);
			}
			else if (iblockstate1.isNormalCube()) {
				blockpos1 = blockpos1.offset(enumfacing);
				iblockstate1 = getBlockState(blockpos1);
				if (Blocks.UNPOWERED_COMPARATOR.isSameDiode(iblockstate1)) {
					iblockstate1.neighborChanged(this, blockpos1, blockIn, pos);
				}
			}
		}
	}
}

CrashReportCategory World::addWorldInfoToCrashReport(CrashReport report)
{
	CrashReportCategory crashreportcategory3 = report.makeCategoryDepth("Affected level", 1);
	crashreportcategory3.addCrashSection("Level name", worldInfo == null ? "????" : worldInfo.getWorldName());
	crashreportcategory3.addDetail("All players", new ICrashReportDetail(){
	   public String call() {
		  return World.this.playerEntities.size() + " total; " + World.this.playerEntities;
	   }
		});
	crashreportcategory3.addDetail("Chunk stats", new ICrashReportDetail(){
	   public String call() {
		  return World.this.chunkProvider.makeString();
	   }
		});

	try {
		worldInfo.addToCrashReport(crashreportcategory3);
	}
	catch (Throwable var4) {
		crashreportcategory3.addCrashSectionThrowable("Level Data Unobtainable", var4);
	}

	return crashreportcategory3;
}

WorldBorder World::getWorldBorder()
{
	return worldBorder;
}

bool World::isSpawnChunk(int32_t x, int32_t z)
{
	BlockPos blockpos1 = getSpawnPoint();
	auto j2 = x * 16 + 8 - blockpos1.getx();
	auto k2 = z * 16 + 8 - blockpos1.getz();
	auto l2 = true;
	return j2 >= -128 && j2 <= 128 && k2 >= -128 && k2 <= 128;
}

void World::sendPacketToServer(Packet& packetIn)
{
	throw UnsupportedOperationException("Can't send packets to server unless you're on the client.");
}

LootTableManager World::getLootTableManager()
{
	return lootTable;
}

BlockPos World::findNearestStructure(std::string structureName, BlockPos& position, bool findUnexplored)
{
}

#include "WorldEntitySpawner.h"
#include "../util/math/BlockPos.h"
#include "World.h"
#include "../util/WeightedRandom.h"
int32_t WorldEntitySpawner::MOB_COUNT_DIV = MathHelper::pow(17.0, 2.0);

int32_t WorldEntitySpawner::findChunksForSpawning(WorldServer& worldServerIn, bool spawnHostileMobs,
	bool spawnPeacefulMobs, bool spawnOnSetTickRate)
{
	if (!spawnHostileMobs && !spawnPeacefulMobs) 
	{
		return 0;
	}
	else 
	{
		eligibleChunksForSpawning.clear();
		auto i = 0;
		auto var6 = worldServerIn.playerEntities.begin();


		while (true) 
		{
			EntityPlayer* entityplayer;
			do 
			{
				if (var6 == worldServerIn.playerEntities.end())
				{
					int j4 = 0;
					auto blockpos1 = worldServerIn.getSpawnPoint();
					auto var39 = EnumCreatureType.values();
					auto k = var39.size();

				label129:
					for (auto var40 = 0; var40 < k; ++var40) 
					{
						EnumCreatureType enumcreaturetype = var39[var40];
						if ((!enumcreaturetype.getPeacefulCreature() || spawnPeacefulMobs) && (enumcreaturetype.getPeacefulCreature() || spawnHostileMobs) && (!enumcreaturetype.getAnimal() || spawnOnSetTickRate)) 
						{
							j1 = worldServerIn.countEntities(enumcreaturetype.getCreatureClass());
							int l4 = enumcreaturetype.getMaxNumberOfCreature() * i / MOB_COUNT_DIV;
							if (j1 <= l4) {
								BlockPos.MutableBlockPos blockpos$mutableblockpos = new BlockPos.MutableBlockPos();
								Iterator var44 = eligibleChunksForSpawning.iterator();

							label126:
								while (true) {
									int k1;
									int l1;
									int i2;
									IBlockState iblockstate;
									do {
										if (!var44.hasNext()) {
											continue label129;
										}

										ChunkPos chunkpos1 = (ChunkPos)var44.next();
										BlockPos blockpos = getRandomChunkPosition(worldServerIn, chunkpos1.x, chunkpos1.z);
										k1 = blockpos.getX();
										l1 = blockpos.getY();
										i2 = blockpos.getZ();
										iblockstate = worldServerIn.getBlockState(blockpos);
									} while (iblockstate.isNormalCube());

									int j2 = 0;

									for (int k2 = 0; k2 < 3; ++k2) {
										int l2 = k1;
										int i3 = l1;
										int j3 = i2;
										int k3 = true;
										Biome.SpawnListEntry biome$spawnlistentry = null;
										IEntityLivingData ientitylivingdata = null;
										int l3 = MathHelper.ceil(Math.random() * 4.0D);

										for (int i4 = 0; i4 < l3; ++i4) {
											l2 += worldServerIn.rand.nextInt(6) - worldServerIn.rand.nextInt(6);
											i3 += worldServerIn.rand.nextInt(1) - worldServerIn.rand.nextInt(1);
											j3 += worldServerIn.rand.nextInt(6) - worldServerIn.rand.nextInt(6);
											blockpos$mutableblockpos.setPos(l2, i3, j3);
											float f = (float)l2 + 0.5F;
											float f1 = (float)j3 + 0.5F;
											if (!worldServerIn.isAnyPlayerWithinRangeAt((double)f, (double)i3, (double)f1, 24.0D) && blockpos1.distanceSq((double)f, (double)i3, (double)f1) >= 576.0D) {
												if (biome$spawnlistentry == null) {
													biome$spawnlistentry = worldServerIn.getSpawnListEntryForTypeAt(enumcreaturetype, blockpos$mutableblockpos);
													if (biome$spawnlistentry == null) {
														break;
													}
												}

												if (worldServerIn.canCreatureTypeSpawnHere(enumcreaturetype, biome$spawnlistentry, blockpos$mutableblockpos) && canCreatureTypeSpawnAtLocation(EntitySpawnPlacementRegistry.getPlacementForEntity(biome$spawnlistentry.entityClass), worldServerIn, blockpos$mutableblockpos)) {
													EntityLiving entityliving;
													try {
														entityliving = (EntityLiving)biome$spawnlistentry.entityClass.getConstructor(World.class).newInstance(worldServerIn);
													}
													catch (Exception var36) {
														var36.printStackTrace();
														return j4;
													}

													entityliving.setLocationAndAngles((double)f, (double)i3, (double)f1, worldServerIn.rand.nextFloat() * 360.0F, 0.0F);
													if (entityliving.getCanSpawnHere() && entityliving.isNotColliding()) {
														ientitylivingdata = entityliving.onInitialSpawn(worldServerIn.getDifficultyForLocation(new BlockPos(entityliving)), ientitylivingdata);
														if (entityliving.isNotColliding()) {
															++j2;
															worldServerIn.spawnEntity(entityliving);
														}
														else {
															entityliving.setDead();
														}

														if (j2 >= entityliving.getMaxSpawnedInChunk()) {
															continue label126;
														}
													}

													j4 += j2;
												}
											}
										}
									}
								}
							}
						}
					}

					return j4;
				}

				entityplayer = (EntityPlayer)var6.next();
			} while (entityplayer.isSpectator());

			int j = MathHelper::floor(entityplayer.posX / 16.0);
			k = MathHelper:: floor(entityplayer.posZ / 16.0);
			int l = true;

			for (int i1 = -8; i1 <= 8; ++i1) {
				for (j1 = -8; j1 <= 8; ++j1) {
					bool flag = i1 == -8 || i1 == 8 || j1 == -8 || j1 == 8;
					ChunkPos chunkpos = new ChunkPos(i1 + j, j1 + k);
					if (!eligibleChunksForSpawning.contains(chunkpos)) {
						++i;
						if (!flag && worldServerIn.getWorldBorder().contains(chunkpos)) {
							PlayerChunkMapEntry playerchunkmapentry = worldServerIn.getPlayerChunkMap().getEntry(chunkpos.x, chunkpos.z);
							if (playerchunkmapentry != nullptr && playerchunkmapentry.isSentToPlayers()) {
								eligibleChunksForSpawning.emplace(chunkpos);
							}
						}
					}
				}
			}
		}
	}
}

bool WorldEntitySpawner::isValidEmptySpawnBlock(IBlockState* state)
{
	if (state->isBlockNormalCube()) {
		return false;
	}
	else if (state->canProvidePower()) {
		return false;
	}
	else if (state->getMaterial().isLiquid()) {
		return false;
	}
	else {
		return !BlockRailBase::isRailBlock(state);
	}
}

BlockPos WorldEntitySpawner::getRandomChunkPosition(World* worldIn, int32_t x, int32_t z) const
{
	auto chunk = worldIn.getChunk(x, z);
	int i = x * 16 + worldIn->rand(16);
	int j = z * 16 + worldIn->rand(16);
	int k = MathHelper::roundUp(chunk.getHeight(BlockPos(i, 0, j)) + 1, 16);
	int l = worldIn->rand(k > 0 ? k : chunk.getTopFilledSegment() + 16 - 1);
	return BlockPos(i, l, j);
}

bool WorldEntitySpawner::canCreatureTypeSpawnAtLocation(SpawnPlacementType spawnPlacementTypeIn, World* worldIn, BlockPos pos)
{
	if (!worldIn->getWorldBorder().contains(pos)) 
	{
		return false;
	}
	else 
	{
		auto iblockstate = worldIn->getBlockState(pos);
		if (spawnPlacementTypeIn == SpawnPlacementType::IN_WATER) 
		{
			return iblockstate->getMaterial() == Material::WATER && worldIn->getBlockState(pos.down()).getMaterial() == Material::WATER && !worldIn->getBlockState(pos.up()).isNormalCube();
		}
		else {
			auto blockpos = pos.down();
			if (!worldIn->getBlockState(blockpos).isTopSolid()) 
			{
				return false;
			}
			else 
			{
				auto block = worldIn->getBlockState(blockpos).getBlock();
				auto flag = block != Blocks::BEDROCK && block != Blocks::BARRIER;
				return flag && isValidEmptySpawnBlock(iblockstate) && isValidEmptySpawnBlock(worldIn->getBlockState(pos.up()));
			}
		}
	}
}

void WorldEntitySpawner::performWorldGenSpawning(World* worldIn, Biome& biomeIn, int32_t centerX, int32_t centerZ, int32_t diameterX, int32_t diameterZ, pcg32& randomIn)
{
	auto list = biomeIn.getSpawnableList(EnumCreatureType::CREATURE);
	if (!list.empty()) 
	{
		while (randomIn.nextFloat() < biomeIn.getSpawningChance()) 
		{
			SpawnListEntry biome$spawnlistentry = (SpawnListEntry)WeightedRandom::getRandomItem(worldIn->rand, list);
			auto i = biome$spawnlistentry.minGroupCount + randomIn.nextInt(1 + biome$spawnlistentry.maxGroupCount - biome$spawnlistentry.minGroupCount);
			IEntityLivingData ientitylivingdata = null;
			auto j = centerX + randomIn(diameterX);
			auto k = centerZ + randomIn(diameterZ);
			auto l = j;
			auto i1 = k;

			for (auto j1 = 0; j1 < i; ++j1) 
			{
				auto flag = false;

				for (auto k1 = 0; !flag && k1 < 4; ++k1) 
				{
					BlockPos blockpos = worldIn->getTopSolidOrLiquidBlock(BlockPos(j, 0, k));
					if (canCreatureTypeSpawnAtLocation(SpawnPlacementType.ON_GROUND, worldIn, blockpos)) 
					{
						EntityLiving entityliving;
						try {
							entityliving = (EntityLiving)biome$spawnlistentry.entityClass.getConstructor(World.class).newInstance(worldIn);
						}
						catch (Exception var21) {
							var21.printStackTrace();
							continue;
						}

						entityliving.setLocationAndAngles((double)((float)j + 0.5F), (double)blockpos.gety(), (double)((float)k + 0.5F), randomIn.nextFloat() * 360.0F, 0.0F);
						worldIn->spawnEntity(entityliving);
						ientitylivingdata = entityliving.onInitialSpawn(worldIn->getDifficultyForLocation(new BlockPos(entityliving)), ientitylivingdata);
						flag = true;
					}

					j += randomIn(5) - randomIn(5);

					for (k += randomIn(5) - randomIn(5); j < centerX || j >= centerX + diameterX || k < centerZ || k >= centerZ + diameterX; k = i1 + randomIn(5) - randomIn(5)) 
					{
						j = l + randomIn(5) - randomIn(5);
					}
				}
			}
		}
	}
}

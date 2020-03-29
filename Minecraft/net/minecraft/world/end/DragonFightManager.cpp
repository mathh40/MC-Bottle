#include "DragonFightManager.h"
#include "../../../../../spdlog/spdlog/spdlog.h"
#include "text/TextComponentTranslation.h"
#include "NBTUtil.h"
#include "state/BlockWorldState.h"
#include "../../../../../spdlog/spdlog/fmt/bundled/format.h"
#include "NBTTagInt.h"
#include "biome/BiomeEndDecorator.h"
#include "EntitySelectors.h"
#include "DragonSpawnManager.h"

auto VALID_PLAYER = [](const Entity& lhs) { return EntitySelectors::IS_ALIVE(lhs) && EntitySelectors::withinRange(lhs,0.0, 128.0, 0.0, 192.0); };

std::shared_ptr<spdlog::logger> Chunk::LOGGER = spdlog::get("Minecraft")->clone("DragonFightManager");

DragonFightManager::DragonFightManager(WorldServer* worldIn, NBTTagCompound* compound)
	: bossInfo(std::make_unique<TextComponentTranslation>("entity->EnderDragon.name"), BossInfo::Color::PINK, BossInfo::Overlay::PROGRESS)
	  , scanForLegacyFight(true), world(worldIn)
{
	bossInfo.setPlayEndBossMusic(true).setCreateFog(true);
	if (compound->hasKey("DragonKilled", 99)) 
	{
		if (compound->hasUniqueId("DragonUUID")) 
		{
			dragonUniqueId = compound->getUniqueId("DragonUUID");
		}

		dragonKilled = compound->getBoolean("DragonKilled");
		previouslyKilled = compound->getBoolean("PreviouslyKilled");
		if (compound->getBoolean("IsRespawning")) 
		{
			respawnState = DragonSpawnManager::START;
		}

		if (compound->hasKey("ExitPortalLocation", 10)) 
		{
			exitPortalLocation = NBTUtil::getPosFromTag(compound->getCompoundTag("ExitPortalLocation"));
		}
	}
	else 
	{
		dragonKilled = true;
		previouslyKilled = true;
	}

	if (compound->hasKey("Gateways", 9)) 
	{
		auto nbttaglist = compound->getTagList("Gateways", 3);

		for (int i = 0; i < nbttaglist->tagCount(); ++i) 
		{
			gateways.emplace_back(nbttaglist->getIntAt(i));
		}
	}
	else 
	{
		std::array<int32_t, 20> temp = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
		gateways.assign(temp.begin(), temp.end());
		std::shuffle(gateways.begin(), gateways.end(), pcg32(worldIn->getSeed()));
	}

	portalPattern = FactoryBlockPattern.start().aisle("       ", "       ", "       ", "   #   ", "       ", "       ", "       ").aisle("       ", "       ", "       ", "   #   ", "       ", "       ", "       ").aisle("       ", "       ", "       ", "   #   ", "       ", "       ", "       ").aisle("  ###  ", " #   # ", "#     #", "#  #  #", "#     #", " #   # ", "  ###  ").aisle("       ", "  ###  ", " ##### ", " ##### ", " ##### ", "  ###  ", "       ").where('#', BlockWorldState.hasState(BlockMatcher.forBlock(Blocks.BEDROCK))).build();
}

std::unique_ptr<NBTTagCompound> DragonFightManager::getCompound()
{
	auto nbttagcompound = std::make_unique<NBTTagCompound>();
	if (dragonUniqueId != nullptr) 
	{
		nbttagcompound->setUniqueId("DragonUUID", dragonUniqueId);
	}

	nbttagcompound->setBoolean("DragonKilled", dragonKilled);
	nbttagcompound->setBoolean("PreviouslyKilled", previouslyKilled);
	nbttagcompound->setTag("ExitPortalLocation", NBTUtil::createPosTag(exitPortalLocation));

	auto nbttaglist = std::make_unique<NBTTagList>();

	for(auto i : gateways)
	{
		nbttaglist->appendTag(std::make_unique<NBTTagInt>(i));
	}

	nbttagcompound->setTag("Gateways", std::move(nbttaglist));
	return nbttagcompound;
}

void DragonFightManager::tick()
{
	bossInfo.setVisible(!dragonKilled);
	if (++ticksSinceLastPlayerScan >= 20) 
	{
		updatePlayers();
		ticksSinceLastPlayerScan = 0;
	}

	if (!bossInfo.getPlayers().empty()) 
	{
		if (scanForLegacyFight) 
		{
			LOGGER->info("Scanning for legacy world dragon fight...");
			loadChunks();
			scanForLegacyFight = false;
			auto flag = hasDragonBeenKilled();
			if (flag) 
			{
				LOGGER->info("Found that the dragon has been killed in this world already.");
				previouslyKilled = true;
			}
			else 
			{
				LOGGER->info("Found that the dragon has not yet been killed in this world.");
				previouslyKilled = false;
				generatePortal(false);
			}

			auto list = world->getEntities<EntityDragon>(EntitySelectors.IS_ALIVE);
			if (list.empty()) 
			{
				dragonKilled = true;
			}
			else 
			{
				auto entitydragon = (EntityDragon)list.get(0);
				dragonUniqueId = entitydragon.getUniqueID();
				LOGGER->info("Found that there's a dragon still alive ({})", entitydragon);
				dragonKilled = false;
				if (!flag) 
				{
					LOGGER->info("But we didn't have a portal, let's remove it.");
					entitydragon.setDead();
					dragonUniqueId = xg::Guid();
				}
			}

			if (!previouslyKilled && dragonKilled) 
			{
				dragonKilled = false;
			}
		}

		if (respawnState != null) 
		{
			if (crystals == null) 
			{
				respawnState = null;
				respawnDragon();
			}

			respawnState.process(world, this, crystals, respawnStateTicks++, exitPortalLocation);
		}

		if (!dragonKilled) 
		{
			if (!dragonUniqueId.isValid() || ++ticksSinceDragonSeen >= 1200) 
			{
				loadChunks();
				auto list1 = world->getEntities<EntityDragon>(EntitySelectors.IS_ALIVE);
				if (list1.empty())
				{
					LOGGER->debug("Haven't seen the dragon, respawning it");
					createNewDragon();
				}
				else 
				{
					LOGGER->debug("Haven't seen our dragon, but found another one to use.");
					dragonUniqueId = ((EntityDragon)list1.get(0)).getUniqueID();
				}

				ticksSinceDragonSeen = 0;
			}

			if (++ticksSinceCrystalsScanned >= 100) 
			{
				findAliveCrystals();
				ticksSinceCrystalsScanned = 0;
			}
		}
	}
}

void DragonFightManager::setRespawnState(DragonSpawnManager state)
{
	if (respawnState == null) 
	{
		throw std::runtime_error("Dragon respawn isn't in progress, can't skip ahead in the animation.");
	}
	else 
	{
		respawnStateTicks = 0;
		if (state == DragonSpawnManager::END) 
		{
			respawnState = fmt::internal::null;
			dragonKilled = false;
			auto entitydragon = createNewDragon();

			for(auto entityplayermp : bossInfo.getPlayers())
			{
				CriteriaTriggers.SUMMONED_ENTITY.trigger(entityplayermp, entitydragon);
			}
		}
		else 
		{
			respawnState = state;
		}
	}
}

BlockPattern.PatternHelper DragonFightManager::findExitPortal()
{
	for (auto i = -8; i <= 8; ++i) 
	{
		for (auto l = -8; l <= 8; ++l)
		{
			auto chunk = world->getChunk(i, l);

			for (auto tileentity : chunk.getTileEntityMap())
			{
				if (tileentity instanceof TileEntityEndPortal) 
				{
					BlockPattern.PatternHelper blockpattern$patternhelper = portalPattern.match(world, tileentity.getPos());
					if (blockpattern$patternhelper != null) 
					{
						auto blockpos = blockpattern$patternhelper.translateOffset(3, 3, 3).getPos();
						if (!exitPortalLocation.has_value() && blockpos.getx() == 0 && blockpos.getz() == 0) 
						{
							exitPortalLocation = blockpos;
						}

						return blockpattern$patternhelper;
					}
				}
			}
		}
	}

	auto i = world->getHeight(WorldGenEndPodium.END_PODIUM_LOCATION).getY();

	for (auto l = i; l >= 0; --l) 
	{
		BlockPattern.PatternHelper blockpattern$patternhelper1 = this.portalPattern.match(this.world, new BlockPos(WorldGenEndPodium.END_PODIUM_LOCATION.getX(), l, WorldGenEndPodium.END_PODIUM_LOCATION.getZ()));
		if (blockpattern$patternhelper1 != null) 
		{
			if (!exitPortalLocation.has_value())
			{
				exitPortalLocation = blockpattern$patternhelper1.translateOffset(3, 3, 3).getPos();
			}

			return blockpattern$patternhelper1;
		}
	}

	return nullptr;
}

void DragonFightManager::loadChunks() const
{
	for (auto i = -8; i <= 8; ++i) 
	{
		for (auto j = -8; j <= 8; ++j)
		{
			world->getChunk(i, j);
		}
	}
}

void DragonFightManager::updatePlayers()
{
	std::unordered_set<EntityPlayerMP*> set;

	for(auto entityplayermp : world->getPlayers<EntityPlayerMP>(VALID_PLAYER))
	{
		bossInfo.addPlayer(entityplayermp);
		set.emplace(entityplayermp);
	}

	auto player = bossInfo.getPlayers();
	std::unordered_set<EntityPlayerMP*> set1(player.begin(), player.end());

	Util::removeAll(set1, set);

	for (auto entityplayermp1 : set1)
	{
		bossInfo.removePlayer(entityplayermp1);
	}
}

void DragonFightManager::findAliveCrystals()
{
	ticksSinceCrystalsScanned = 0;
	aliveCrystals = 0;
	auto var1 = BiomeEndDecorator::getSpikesForWorld(world);

	for (auto endspike : var1)
	{
		aliveCrystals += world->getEntitiesWithinAABB<EntityEnderCrystal>(endspike.getTopBoundingBox()).size();
	}

	LOGGER->debug("Found {} end crystals still alive", aliveCrystals);
}

void DragonFightManager::spawnNewGateway()
{
	if (!gateways.empty()) 
	{
		auto i = gateways.erase(gateways.end() - 1);
		auto j = (96.0 * MathHelper::cos(2.0 * (-3.141592653589793 + 0.15707963267948966 * *i)));
		auto k = (96.0 * MathHelper::sin(2.0 * (-3.141592653589793 + 0.15707963267948966 * *i)));
		BlockPos pos(j, 75.0, k);
		generateGateway(pos);
	}
}

void DragonFightManager::generateGateway(BlockPos& pos)
{
	world->playEvent(3000, pos, 0);
	(new WorldGenEndGateway()).generate(world, new Random(), pos);
}

void DragonFightManager::generatePortal(bool active)
{
	WorldGenEndPodium worldgenendpodium;
	if (exitPortalLocation) 
	{
		for (exitPortalLocation = world->getTopSolidOrLiquidBlock(WorldGenEndPodium.END_PODIUM_LOCATION).down(); world->getBlockState(exitPortalLocation).getBlock() == Blocks.BEDROCK exitPortalLocation.gety() > world->getSeaLevel(); exitPortalLocation = exitPortalLocation.down())
		{
		}
	}

	pcg32 diff;
	worldgenendpodium.generate(world, diff, exitPortalLocation);
}

std::unique_ptr<EntityDragon> DragonFightManager::createNewDragon()
{
	BlockPos pos(0, 128, 0);
	world->getChunk(pos);
	auto entitydragon = std::make_unique<EntityDragon>(world);
	entitydragon->getPhaseManager().setPhase(PhaseList.HOLDING_PATTERN);
	auto rand = world->rand;
	std::uniform_real_distribution<float> dis(0.0, 1.0);
	entitydragon->setLocationAndAngles(0.0, 128.0, 0.0, dis(rand) * 360.0F, 0.0F);
	world->spawnEntity(entitydragon);
	dragonUniqueId = entitydragon.getUniqueID();
	return entitydragon;
}

void DragonFightManager::respawnDragon(std::vector<Entity*> crystalsIn)
{
	if (dragonKilled && respawnState == nullptr) 
	{
		for (BlockPattern.PatternHelper blockpattern$patternhelper = findExitPortal(); blockpattern$patternhelper != null; blockpattern$patternhelper = findExitPortal()) 
		{
			for (auto i = 0; i < portalPattern.getPalmLength(); ++i) 
			{
				for (auto j = 0; j < portalPattern.getThumbLength(); ++j)
				{
					for (auto k = 0; k < portalPattern.getFingerLength(); ++k)
					{
						auto blockworldstate = blockpattern$patternhelper.translateOffset(i, j, k);
						if (blockworldstate.getBlockState().getBlock() == Blocks.BEDROCK || blockworldstate.getBlockState().getBlock() == Blocks.END_PORTAL) 
						{
							world->setBlockState(blockworldstate.getPos(), Blocks.END_STONE.getDefaultState());
						}
					}
				}
			}
		}

		respawnState = DragonSpawnManager::START;
		respawnStateTicks = 0;
		generatePortal(false);
		crystals = crystalsIn;
	}
}

void DragonFightManager::dragonUpdate(EntityDragon* dragonIn)
{
	if (dragonIn->getUniqueID() == dragonUniqueId)
	{
		bossInfo.setPercent(dragonIn->getHealth() / dragonIn->getMaxHealth());
		ticksSinceDragonSeen = 0;
		if (dragonIn->hasCustomName()) 
		{
			bossInfo.setName(dragonIn->getDisplayName());
		}
	}
}

int32_t DragonFightManager::getNumAliveCrystals() const
{
	return aliveCrystals;
}

void DragonFightManager::onCrystalDestroyed(EntityEnderCrystal* crystal, DamageSource::DamageSource& dmgSrc)
{
	if (respawnState != nullptr && crystals.contains(crystal))
	{
		LOGGER->debug("Aborting respawn sequence");
		respawnState = nullptr;
		respawnStateTicks = 0;
		resetSpikeCrystals();
		generatePortal(true);
	}
	else 
	{
		findAliveCrystals();
		auto entity = world->getEntityFromUuid(dragonUniqueId);
		if (entity instanceof EntityDragon) 
		{
			((EntityDragon)entity).onCrystalDestroyed(crystal,BlockPos(crystal), dmgSrc);
		}
	}
}

bool DragonFightManager::hasPreviouslyKilledDragon() const
{
	return previouslyKilled;
}

void DragonFightManager::respawnDragon()
{
	if (dragonKilled && respawnState == nullptr) 
	{
		auto blockpos = exitPortalLocation;
		if (blockpos == nullptr)
		{
			LOGGER->debug("Tried to respawn, but need to find the portal first.");
			BlockPattern.PatternHelper blockpattern$patternhelper = findExitPortal();
			if (blockpattern$patternhelper == nullptr) 
			{
				LOGGER->debug("Couldn't find a portal, so we made one.");
				generatePortal(true);
			}
			else {
				LOGGER->debug("Found the exit portal & temporarily using it.");
			}

			blockpos = exitPortalLocation;
		}

		std::vector<Entity*> list1;
		auto blockpos1 = blockpos.value().up(1);

		for(auto enumfacing : EnumFacing::Plane::HORIZONTAL)
		{
			auto list = world->getEntitiesWithinAABB<EntityEnderCrystal>(AxisAlignedBB(blockpos1.offset(enumfacing, 2)));
			if (list.empty()) 
			{
				return;
			}

			list1.assign(list);
		}

		LOGGER->debug("Found all crystals, respawning dragon.");
		respawnDragon(list1);
	}
}

void DragonFightManager::resetSpikeCrystals()
{
	auto var1 = BiomeEndDecorator::getSpikesForWorld(world);
	int var2 = var1.length;

	for (int var3 = 0; var3 < var2; ++var3) 
	{
		WorldGenSpikes.EndSpike worldgenspikes$endspike = var1[var3];
		Iterator var5 = world->getEntitiesWithinAABB<EntityEnderCrystal>(worldgenspikes$endspike.getTopBoundingBox()).iterator();

		while (var5.hasNext()) 
		{
			EntityEnderCrystal entityendercrystal = (EntityEnderCrystal)var5.next();
			entityendercrystal.setEntityInvulnerable(false);
			entityendercrystal.setBeamTarget((BlockPos)null);
		}
	}
}

void DragonFightManager::processDragonDeath(EntityDragon* dragon)
{
	if (dragon->getUniqueID() == dragonUniqueId) 
	{
		bossInfo.setPercent(0.0F);
		bossInfo.setVisible(false);
		generatePortal(true);
		spawnNewGateway();
		if (!previouslyKilled) 
		{
			world->setBlockState(world->getHeight(WorldGenEndPodium::END_PODIUM_LOCATION), Blocks::DRAGON_EGG.getDefaultState());
		}

		previouslyKilled = true;
		dragonKilled = true;
	}
}

bool DragonFightManager::hasDragonBeenKilled()
{
	for (auto i = -8; i <= 8; ++i) 
	{
		for (auto j = -8; j <= 8; ++j) 
		{
			auto chunk = world->getChunk(i, j);
			Iterator var4 = chunk.getTileEntityMap().values().iterator();

			for (auto tileentity : chunk.getTileEntityMap())
			{
				if (Util::instanceof<TileEntityEndPortal>(tileentity))
				{
					return true;
				}
			}
		}
	}

	return false;
}

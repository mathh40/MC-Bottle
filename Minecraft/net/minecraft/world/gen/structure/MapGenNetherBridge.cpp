#include "MapGenNetherBridge.h"

bool MapGenNetherBridge::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
	auto i = chunkX >> 4;
	auto j = chunkZ >> 4;
	rand.seed((long)(i ^ j << 4) ^ world->getSeed());
	rand();
	if (rand(3) != 0) 
	{
		return false;
	} else if (chunkX != (i << 4) + 4 + rand(8)) 
	{
		return false;
	} else 
	{
		return chunkZ == (j << 4) + 4 + rand(8);
	}
}

StructureStart MapGenNetherBridge::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
    return MapGenNetherBridge::Start(world, rand, chunkX, chunkZ);
}

MapGenNetherBridge::MapGenNetherBridge()
{
	spawnList.emplace_back(new Biome:::SpawnListEntry<EntityBlaze>(10, 2, 3));
	spawnList.emplace_back(new Biome::SpawnListEntry<EntityPigZombie>(5, 4, 4));
	spawnList.emplace_back(new Biome::SpawnListEntry<EntityWitherSkeleton>(8, 5, 5));
	spawnList.emplace_back(new Biome::SpawnListEntry<EntitySkeleton>(2, 5, 5));
	spawnList.emplace_back(new Biome::SpawnListEntry<EntityMagmaCube>(3, 4, 4));
}

std::string MapGenNetherBridge::getStructureName()
{
	return "Fortress";
}

std::vector<SpawnListEntry> MapGenNetherBridge::getSpawnList() const
{
	return spawnList;
}

std::optional<BlockPos> MapGenNetherBridge::getNearestStructurePos(World* worldIn, const BlockPos& pos, bool findUnexplored)
{
	auto j = pos.getx() >> 4;
	auto k = pos.getz() >> 4;

	for(auto l = 0; l <= 1000; ++l) 
	{
		for(auto i1 = -l; i1 <= l; ++i1) 
		{
			auto flag = i1 == -l || i1 == l;

			for(int j1 = -l; j1 <= l; ++j1) 
			{
				auto flag1 = j1 == -l || j1 == l;
				if (flag || flag1) 
				{
					auto k1 = j + i1;
					auto l1 = k + j1;
					if (canSpawnStructureAtCoords(k1, l1) && (!findUnexplored || !worldIn->isChunkGeneratedAt(k1, l1))) 
					{
						return BlockPos((k1 << 4) + 8, 64, (l1 << 4) + 8);
					}
				}
			}
		}
	}

	return std::nullopt;
}

MapGenNetherBridge::Start::Start(World* worldIn, pcg32& random, int32_t chunkX, int32_t chunkZ)
	:StructureStart(chunkX, chunkZ)
{
	Start structurenetherbridgepieces$start(random, (chunkX << 4) + 2, (chunkZ << 4) + 2);
	components.emplace_back(structurenetherbridgepieces$start);
	structurenetherbridgepieces$start.buildComponent(structurenetherbridgepieces$start, components, random);
	auto list = structurenetherbridgepieces$start.pendingChildren;

	while(!list.isEmpty()) 
	{
		auto i = random(list.size());
		auto structurecomponent = list.remove(i);
		structurecomponent.buildComponent(structurenetherbridgepieces$start, components, random);
	}

	updateBoundingBox();
	setRandomHeight(worldIn, random, 48, 70);
}

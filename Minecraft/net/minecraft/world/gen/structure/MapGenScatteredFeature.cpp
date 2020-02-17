#include "MapGenScatteredFeature.h"
#include "ComponentScatteredFeaturePieces.h"

std::vector<Biome*> MapGenScatteredFeature::BIOMELIST = { Biomes::DESERT, Biomes::DESERT_HILLS, Biomes::JUNGLE, Biomes::JUNGLE_HILLS, Biomes::SWAMPLAND, Biomes::ICE_PLAINS, Biomes::COLD_TAIGA };

bool MapGenScatteredFeature::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
	int32_t i = chunkX;
	int32_t j = chunkZ;
	if (chunkX < 0) 
	{
		chunkX -= maxDistanceBetweenScatteredFeatures - 1;
	}

	if (chunkZ < 0) 
	{
		chunkZ -= maxDistanceBetweenScatteredFeatures - 1;
	}

	int32_t k = chunkX / maxDistanceBetweenScatteredFeatures;
	int32_t l = chunkZ / maxDistanceBetweenScatteredFeatures;
	auto random = world->setRandomSeed(k, l, 14357617);
	k *= maxDistanceBetweenScatteredFeatures;
	l *= maxDistanceBetweenScatteredFeatures;
	k += random(maxDistanceBetweenScatteredFeatures - 8);
	l += random(maxDistanceBetweenScatteredFeatures - 8);
	if (i == k && j == l) 
	{
		auto biome = world->getBiomeProvider().getBiome(BlockPos(i * 16 + 8, 0, j * 16 + 8));
		if (biome == nullptr) 
		{
			return false;
		}

		for(auto biome1 : BIOMELIST)
		{
			if (biome == biome1) 
			{
				return true;
			}
		}
	}

	return false;
}

StructureStart MapGenScatteredFeature::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
	return MapGenScatteredFeature::Start(world, rand, chunkX, chunkZ);
}

MapGenScatteredFeature::MapGenScatteredFeature()
	:maxDistanceBetweenScatteredFeatures(32), minDistanceBetweenScatteredFeatures(8)
{
	monsters.emplace_back(SpawnListEntry(EntityWitch,1,1,1));
}

MapGenScatteredFeature::MapGenScatteredFeature(const std::unordered_map<std::string, std::string>& p_i2061_1_)
	: MapGenScatteredFeature()
{
	for(auto entry : p_i2061_1_)
	{
		if (entry.first == "distance") 
		{
			maxDistanceBetweenScatteredFeatures = MathHelper::getInt(entry.second, maxDistanceBetweenScatteredFeatures, 9);
		}
	}
}

std::string MapGenScatteredFeature::getStructureName()
{
	return "Temple";
}

std::optional<BlockPos> MapGenScatteredFeature::getNearestStructurePos(World* worldIn, BlockPos& pos,
	bool findUnexplored)
{	world = worldIn;
	return findNearestStructurePosBySpacing(worldIn, *this, pos, maxDistanceBetweenScatteredFeatures, 8, 14357617, false, 100, findUnexplored);
}

bool MapGenScatteredFeature::isSwampHut(BlockPos& pos)
{
	auto structurestart = getStructureAt(pos);
	if (structurestart.has_value() && Util::instanceof<MapGenScatteredFeature::Start>(structurestart) && !structurestart.value().components.isEmpty()) {
		StructureComponent structurecomponent = (StructureComponent)structurestart.components.get(0);
		return Util::instanceof< ComponentScatteredFeaturePieces::SwampHut>(structurecomponent);
	}
	else 
	{
		return false;
	}
}

std::vector<> MapGenScatteredFeature::getMonsters()
{
	return monsters;
}

MapGenScatteredFeature::Start::Start(World* worldIn, pcg32& random, int32_t chunkX, int32_t chunkZ)
	:Start(worldIn, random, chunkX, chunkZ, worldIn->getBiome(BlockPos(chunkX * 16 + 8, 0, chunkZ * 16 + 8)))
{
}

MapGenScatteredFeature::Start::Start(World* worldIn, pcg32& random, int32_t chunkX, int32_t chunkZ, Biome* biomeIn)
	: StructureStart(chunkX, chunkZ)
{
	if (biomeIn != Biomes::JUNGLE && biomeIn != Biomes::JUNGLE_HILLS) 
	{
		if (biomeIn == Biomes::SWAMPLAND) 
		{
			ComponentScatteredFeaturePieces::SwampHut componentscatteredfeaturepieces$swamphut = ComponentScatteredFeaturePieces::SwampHut(random, chunkX * 16, chunkZ * 16);
			components.emplace_back(componentscatteredfeaturepieces$swamphut);
		}
		else if (biomeIn != Biomes::DESERT && biomeIn != Biomes::DESERT_HILLS) 
		{
			if (biomeIn == Biomes::ICE_PLAINS || biomeIn == Biomes::COLD_TAIGA) 
			{
				ComponentScatteredFeaturePieces::Igloo componentscatteredfeaturepieces$igloo = ComponentScatteredFeaturePieces::Igloo(random, chunkX * 16, chunkZ * 16);
				components.emplace_back(componentscatteredfeaturepieces$igloo);
			}
		}
		else {
			ComponentScatteredFeaturePieces::DesertPyramid componentscatteredfeaturepieces$desertpyramid = ComponentScatteredFeaturePieces::DesertPyramid(random, chunkX * 16, chunkZ * 16);
			components.emplace_back(componentscatteredfeaturepieces$desertpyramid);
		}
	}
	else {
		ComponentScatteredFeaturePieces::JunglePyramid componentscatteredfeaturepieces$junglepyramid = ComponentScatteredFeaturePieces::JunglePyramid(random, chunkX * 16, chunkZ * 16);
		components.emplace_back(componentscatteredfeaturepieces$junglepyramid);
	}

	updateBoundingBox();
}

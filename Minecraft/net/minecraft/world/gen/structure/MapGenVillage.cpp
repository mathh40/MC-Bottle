#include "MapGenVillage.h"

std::vector<Biome*> MapGenVillage::VILLAGE_SPAWN_BIOMES = {Biomes::PLAINS, Biomes::DESERT, Biomes::SAVANNA, Biomes::TAIGA};

MapGenVillage::MapGenVillage(const std::unordered_map<std::string, std::string>& map)
{
	for(auto& entry : map)
	{
		if (entry.first == "size")
		{
			size = MathHelper::getInt(entry.second, size, 0);
		} 
		else if (entry.first == "distance") 
		{
			distance = MathHelper::getInt(entry.second, distance, 9);
		}
	}
}

std::string MapGenVillage::getStructureName()
{
	return "Village";
}

std::optional<BlockPos> MapGenVillage::getNearestStructurePos(World* worldIn, BlockPos& pos, bool findUnexplored)
{
	world = worldIn;
	return findNearestStructurePosBySpacing(worldIn, *this, pos, distance, 8, 10387312, false, 100, findUnexplored);
}

MapGenVillage::Start::Start(World* worldIn, pcg32& rand, int32_t x, int32_t z, int32_t size)
	:StructureStart(x,z)
{
	auto list = StructureVillagePieces::getStructureVillageWeightedPieceList(rand, size);
         StructureVillagePieces::Start structurevillagepieces$start = StructureVillagePieces::Start(worldIn->getBiomeProvider(), 0, rand, (x << 4) + 2, (z << 4) + 2, list, size);
         components.emplace_back(structurevillagepieces$start);
         structurevillagepieces$start.buildComponent(structurevillagepieces$start, components, rand);
         auto list1 = structurevillagepieces$start.pendingRoads;
         auto list2 = structurevillagepieces$start.pendingHouses;

         int k;
         while(!list1.isEmpty() || !list2.isEmpty()) {
            StructureComponent structurecomponent;
            if (list1.isEmpty()) {
               k = rand(list2.size());
               structurecomponent = (StructureComponent)list2.remove(k);
               structurecomponent.buildComponent(structurevillagepieces$start, components, rand);
            } else {
               k = rand(list1.size());
               structurecomponent = (StructureComponent)list1.remove(k);
               structurecomponent.buildComponent(structurevillagepieces$start, components, rand);
            }
         }

         updateBoundingBox();
         k = 0;
         Iterator var13 = components.iterator();

         while(var13.hasNext()) 
		 {
            StructureComponent structurecomponent1 = (StructureComponent)var13.next();
            if (!(structurecomponent1 instanceof StructureVillagePieces.Road)) 
			{
               ++k;
            }
         }

         hasMoreThanTwoComponents = k > 2;
}

bool MapGenVillage::Start::isSizeableStructure()
{
	return hasMoreThanTwoComponents;
}

void MapGenVillage::Start::writeToNBT(NBTTagCompound* tagCompound)
{
	StructureStart::writeToNBT(tagCompound);
	tagCompound->setBoolean("Valid", hasMoreThanTwoComponents);
}

void MapGenVillage::Start::readFromNBT(NBTTagCompound* tagCompound)
{
	StructureStart::readFromNBT(tagCompound);
	hasMoreThanTwoComponents = tagCompound->getBoolean("Valid");
}

bool MapGenVillage::canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ)
{
	int32_t i = chunkX;
	int32_t j = chunkZ;
	if (chunkX < 0) 
	{
		chunkX -= distance - 1;
	}

	if (chunkZ < 0) 
	{
		chunkZ -= distance - 1;
	}

	int32_t k = chunkX / distance;
	int32_t l = chunkZ / distance;
	auto random = world->setRandomSeed(k, l, 10387312);
	k *= distance;
	l *= distance;
	k += random(distance - 8);
	l += random(distance - 8);
	if (i == k && j == l) 
	{
		bool flag = world->getBiomeProvider().areBiomesViable(i * 16 + 8, j * 16 + 8, 0, VILLAGE_SPAWN_BIOMES);
		if (flag) 
		{
			return true;
		}
	}

	return false;
}

StructureStart MapGenVillage::getStructureStart(int32_t chunkX, int32_t chunkZ)
{
	return MapGenVillage::Start(world, rand, chunkX, chunkZ, size);
}

#include "MapGenStructureData.h"
#include "NBTTagCompound.h"

MapGenStructureData::MapGenStructureData(std::string_view name)
	:WorldSavedData(name)
{
}

void MapGenStructureData::readFromNBT(NBTTagCompound* nbt)
{
	tagCompound = nbt->getCompoundTag("Features");
}

NBTTagCompound* MapGenStructureData::writeToNBT(NBTTagCompound* compound)
{
	compound->setTag("Features", tagCompound);
	return compound;
}

void MapGenStructureData::writeInstance(NBTTagCompound* tagCompoundIn, int32_t chunkX, int32_t chunkZ) const
{
	tagCompound->setTag(formatChunkCoords(chunkX, chunkZ), tagCompoundIn);
}

std::string MapGenStructureData::formatChunkCoords(int32_t chunkX, int32_t chunkZ) const
{
	return "[" + std::to_string(chunkX) + "," + std::to_string(chunkZ) + "]";
}

void MapGenStructureData::writeInstance(NBTTagCompound* tagCompoundIn, int32_t chunkX, int32_t chunkZ)
{
	tagCompound.setTag(formatChunkCoords(chunkX, chunkZ), tagCompoundIn);
}

std::string MapGenStructureData::formatChunkCoords(int32_t chunkX, int32_t chunkZ)
{
	return "[" + std::to_string(chunkX) + "," + std::to_string(chunkZ) + "]";
}

NBTTagCompound* MapGenStructureData::getTagCompound() const
{
	return tagCompound.get();
}



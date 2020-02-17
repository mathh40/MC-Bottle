#pragma once
#include "storage/WorldSavedData.h"
#include "NBTBase.h"

class MapGenStructureData :public WorldSavedData
{
private:
	std::unique_ptr<NBTTagCompound> tagCompound;
public:
	explicit MapGenStructureData(std::string_view name);
	void readFromNBT(NBTTagCompound* nbt) override;
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	void writeInstance(NBTTagCompound* tagCompoundIn, int32_t chunkX, int32_t chunkZ) const;
	std::string formatChunkCoords(int32_t chunkX, int32_t chunkZ) const;
	void writeInstance(NBTTagCompound* tagCompoundIn, int32_t chunkX, int32_t chunkZ);
	std::string formatChunkCoords(int32_t chunkX, int32_t chunkZ);
	NBTTagCompound* getTagCompound() const;
};

#pragma once
#include "WorldSavedData.h"
#include <array>
#include <unordered_map>
#include "MapDecoration.h"
#include <optional>

class World;
class BlockPos;
class EntityPlayer;
class ItemStack;

class MapData :public WorldSavedData
{
public:

	class MapInfo
	{
	public:
		EntityPlayer* player;
		int32_t step;
		MapInfo(EntityPlayer* player);
		Packet* getPacket(ItemStack stack);
		void update(int32_t x, int32_t y);
	private:
		bool isDirty = true;
		int32_t minX;
		int32_t minY;
		int32_t maxX = 127;
		int32_t maxY = 127;
		int32_t tick;
	};


	int32_t xCenter;
	int32_t zCenter;
	uint8_t dimension;
	bool trackingPosition;
	bool unlimitedTracking;
	uint8_t scale;
	std::array<uint8_t, 16384> colors;
	std::vector<MapData::MapInfo> playersArrayList;
	std::unordered_map<std::string, MapDecoration> mapDecorations;

	explicit MapData(std::string_view mapname);
	void calculateMapCenter(double x, double z, int32_t mapScale);
	void readFromNBT(NBTTagCompound* nbt) override;
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	void updateVisiblePlayers(EntityPlayer* player, ItemStack mapStack);
	void addTargetDecoration(ItemStack map, const BlockPos& target, std::string_view decorationName, MapDecoration::Type type);
	Packet* getMapPacket(ItemStack mapStack, World* worldIn, EntityPlayer* player);
	void updateMapData(int32_t x, int32_t y);
	MapData::MapInfo getMapInfo(EntityPlayer* player);
private:
	std::unordered_map<EntityPlayer*, std::optional<MapData::MapInfo>> playersHashMap;

	void updateDecorations(MapDecoration::Type type, World* worldIn, std::string_view decorationName, double worldX, double worldZ, double rotationIn);
};

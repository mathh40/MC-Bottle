#pragma once
#include <array>
#include <cstdint>
class MapColor
{
public:
	static std::array<MapColor*, 64> COLORS;
	static std::array< MapColor*, 16> BLOCK_COLORS;
	static MapColor AIR;
	static MapColor GRASS;
	static MapColor SAND;
	static MapColor CLOTH;
	static MapColor TNT;
	static MapColor ICE;
	static MapColor IRON;
	static MapColor FOLIAGE;
	static MapColor SNOW;
	static MapColor CLAY;
	static MapColor DIRT;
	static MapColor STONE;
	static MapColor WATER;
	static MapColor WOOD;
	static MapColor QUARTZ;
	static MapColor ADOBE;
	static MapColor MAGENTA;
	static MapColor LIGHT_BLUE;
	static MapColor YELLOW;
	static MapColor LIME;
	static MapColor PINK;
	static MapColor GRAY;
	static MapColor SILVER;
	static MapColor CYAN;
	static MapColor PURPLE;
	static MapColor BLUE;
	static MapColor BROWN;
	static MapColor GREEN;
	static MapColor RED;
	static MapColor BLACK;
	static MapColor GOLD;
	static MapColor DIAMOND;
	static MapColor LAPIS;
	static MapColor EMERALD;
	static MapColor OBSIDIAN;
	static MapColor NETHERRACK;
	static MapColor WHITE_STAINED_HARDENED_CLAY;
	static MapColor ORANGE_STAINED_HARDENED_CLAY;
	static MapColor MAGENTA_STAINED_HARDENED_CLAY;
	static MapColor LIGHT_BLUE_STAINED_HARDENED_CLAY;
	static MapColor YELLOW_STAINED_HARDENED_CLAY;
	static MapColor LIME_STAINED_HARDENED_CLAY;
	static MapColor PINK_STAINED_HARDENED_CLAY;
	static MapColor GRAY_STAINED_HARDENED_CLAY;
	static MapColor SILVER_STAINED_HARDENED_CLAY;
	static MapColor CYAN_STAINED_HARDENED_CLAY;
	static MapColor PURPLE_STAINED_HARDENED_CLAY;
	static MapColor BLUE_STAINED_HARDENED_CLAY;
	static MapColor BROWN_STAINED_HARDENED_CLAY;
	static MapColor GREEN_STAINED_HARDENED_CLAY;
	static MapColor RED_STAINED_HARDENED_CLAY;
	static MapColor BLACK_STAINED_HARDENED_CLAY;
	int32_t colorValue;
	int32_t colorIndex;

	int32_t getMapColor(int32_t index);
	static MapColor getBlockColor(EnumDyeColor dyeColorIn);

private:
	MapColor(int32_t index, int32_t color);
};
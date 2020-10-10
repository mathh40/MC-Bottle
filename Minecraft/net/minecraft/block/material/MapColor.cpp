#include "MapColor.h"

MapColor::BLOCK_COLORS [EnumDyeColor::WHITE.getMetadata()]
=
&
SNOW;
MapColor::BLOCK_COLORS [EnumDyeColor::ORANGE.getMetadata()]
=
&
ADOBE;
MapColor::BLOCK_COLORS [EnumDyeColor::MAGENTA.getMetadata()]
=
&
MAGENTA;
MapColor::BLOCK_COLORS [EnumDyeColor::LIGHT_BLUE.getMetadata()]
=
&
LIGHT_BLUE;
MapColor::BLOCK_COLORS [EnumDyeColor::YELLOW.getMetadata()]
=
&
YELLOW;
MapColor::BLOCK_COLORS [EnumDyeColor::LIME.getMetadata()]
=
&
LIME;
MapColor::BLOCK_COLORS [EnumDyeColor::PINK.getMetadata()]
=
&
PINK;
MapColor::BLOCK_COLORS [EnumDyeColor::GRAY.getMetadata()]
=
&
GRAY;
MapColor::BLOCK_COLORS [EnumDyeColor::SILVER.getMetadata()]
=
&
SILVER;
MapColor::BLOCK_COLORS [EnumDyeColor::CYAN.getMetadata()]
=
&
CYAN;
MapColor::BLOCK_COLORS [EnumDyeColor::PURPLE.getMetadata()]
=
&
PURPLE;
MapColor::BLOCK_COLORS [EnumDyeColor::BLUE.getMetadata()]
=
&
BLUE;
MapColor::BLOCK_COLORS [EnumDyeColor::BROWN.getMetadata()]
=
&
BROWN;
MapColor::BLOCK_COLORS [EnumDyeColor::GREEN.getMetadata()]
=
&
GREEN;
MapColor::BLOCK_COLORS [EnumDyeColor::RED.getMetadata()]
=
&
RED;
MapColor::BLOCK_COLORS [EnumDyeColor::BLACK.getMetadata()]
=
&
BLACK;

static MapColor AIR(0, 0);
static MapColor GRASS(1, 8368696);
static MapColor SAND(2, 16247203);
static MapColor CLOTH(3, 13092807);
static MapColor TNT(4, 16711680);
static MapColor ICE(5, 10526975);
static MapColor IRON(6, 10987431);
static MapColor FOLIAGE(7, 31744);
static MapColor SNOW(8, 16777215);
static MapColor CLAY(9, 10791096);
static MapColor DIRT(10, 9923917);
static MapColor STONE(11, 7368816);
static MapColor WATER(12, 4210943);
static MapColor WOOD(13, 9402184);
static MapColor QUARTZ(14, 16776437);
static MapColor ADOBE(15, 14188339);
static MapColor MAGENTA(16, 11685080);
static MapColor LIGHT_BLUE(17, 6724056);
static MapColor YELLOW(18, 15066419);
static MapColor LIME(19, 8375321);
static MapColor PINK(20, 15892389);
static MapColor GRAY(21, 5000268);
static MapColor SILVER(22, 10066329);
static MapColor CYAN(23, 5013401);
static MapColor PURPLE(24, 8339378);
static MapColor BLUE(25, 3361970);
static MapColor BROWN(26, 6704179);
static MapColor GREEN(27, 6717235);
static MapColor RED(28, 10040115);
static MapColor BLACK(29, 1644825);
static MapColor GOLD(30, 16445005);
static MapColor DIAMOND(31, 6085589);
static MapColor LAPIS(32, 4882687);
static MapColor EMERALD(33, 55610);
static MapColor OBSIDIAN(34, 8476209);
static MapColor NETHERRACK(35, 7340544);
static MapColor WHITE_STAINED_HARDENED_CLAY(36, 13742497);
static MapColor ORANGE_STAINED_HARDENED_CLAY(37, 10441252);
static MapColor MAGENTA_STAINED_HARDENED_CLAY(38, 9787244);
static MapColor LIGHT_BLUE_STAINED_HARDENED_CLAY(39, 7367818);
static MapColor YELLOW_STAINED_HARDENED_CLAY(40, 12223780);
static MapColor LIME_STAINED_HARDENED_CLAY(41, 6780213);
static MapColor PINK_STAINED_HARDENED_CLAY(42, 10505550);
static MapColor GRAY_STAINED_HARDENED_CLAY(43, 3746083);
static MapColor SILVER_STAINED_HARDENED_CLAY(44, 8874850);
static MapColor CYAN_STAINED_HARDENED_CLAY(45, 5725276);
static MapColor PURPLE_STAINED_HARDENED_CLAY(46, 8014168);
static MapColor BLUE_STAINED_HARDENED_CLAY(47, 4996700);
static MapColor BROWN_STAINED_HARDENED_CLAY(48, 4993571);
static MapColor GREEN_STAINED_HARDENED_CLAY(49, 5001770);
static MapColor RED_STAINED_HARDENED_CLAY(50, 9321518);
static MapColor BLACK_STAINED_HARDENED_CLAY(51, 2430480);


MapColor(int32_t index, int32_t color) {
    if (index >= 0 && index <= 63) {
        colorIndex = index;
        colorValue = color;
        COLORS[index] = this;
    } else {
        throw std::logic_error("Map colour ID must be between 0 and 63 (inclusive)");
    }
}

int32_t MapColor::getMapColor(int32_t index) {
    auto i = 220;
    if (index == 3) {
        i = 135;
    }

    if (index == 2) {
        i = 255;
    }

    if (index == 1) {
        i = 220;
    }

    if (index == 0) {
        i = 180;
    }

    auto j = (colorValue >> 16 & 255) * i / 255;
    auto k = (colorValue >> 8 & 255) * i / 255;
    auto l = (colorValue & 255) * i / 255;
    return -16777216 | j << 16 | k << 8 | l;
}

MapColor MapColor::getBlockColor(EnumDyeColor dyeColorIn) {
    return BLOCK_COLORS[dyeColorIn.getMetadata()];
}

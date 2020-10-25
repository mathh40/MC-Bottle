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

MapColor MapColor::AIR(0, 0);
MapColor MapColor::GRASS(1, 8368696);
MapColor MapColor::SAND(2, 16247203);
MapColor MapColor::CLOTH(3, 13092807);
MapColor MapColor::TNT(4, 16711680);
MapColor MapColor::ICE(5, 10526975);
MapColor MapColor::IRON(6, 10987431);
MapColor MapColor::FOLIAGE(7, 31744);
MapColor MapColor::SNOW(8, 16777215);
MapColor MapColor::CLAY(9, 10791096);
MapColor MapColor::DIRT(10, 9923917);
MapColor MapColor::STONE(11, 7368816);
MapColor MapColor::WATER(12, 4210943);
MapColor MapColor::WOOD(13, 9402184);
MapColor MapColor::QUARTZ(14, 16776437);
MapColor MapColor::ADOBE(15, 14188339);
MapColor MapColor::MAGENTA(16, 11685080);
MapColor MapColor::LIGHT_BLUE(17, 6724056);
MapColor MapColor::YELLOW(18, 15066419);
MapColor MapColor::LIME(19, 8375321);
MapColor MapColor::PINK(20, 15892389);
MapColor MapColor::GRAY(21, 5000268);
MapColor MapColor::SILVER(22, 10066329);
MapColor MapColor::CYAN(23, 5013401);
MapColor MapColor::PURPLE(24, 8339378);
MapColor MapColor::BLUE(25, 3361970);
MapColor MapColor::BROWN(26, 6704179);
MapColor MapColor::GREEN(27, 6717235);
MapColor MapColor::RED(28, 10040115);
MapColor MapColor::BLACK(29, 1644825);
MapColor MapColor::GOLD(30, 16445005);
MapColor MapColor::DIAMOND(31, 6085589);
MapColor MapColor::LAPIS(32, 4882687);
MapColor MapColor::EMERALD(33, 55610);
MapColor MapColor::OBSIDIAN(34, 8476209);
MapColor MapColor::NETHERRACK(35, 7340544);
MapColor MapColor::WHITE_STAINED_HARDENED_CLAY(36, 13742497);
MapColor MapColor::ORANGE_STAINED_HARDENED_CLAY(37, 10441252);
MapColor MapColor::MAGENTA_STAINED_HARDENED_CLAY(38, 9787244);
MapColor MapColor::LIGHT_BLUE_STAINED_HARDENED_CLAY(39, 7367818);
MapColor MapColor::YELLOW_STAINED_HARDENED_CLAY(40, 12223780);
MapColor MapColor::LIME_STAINED_HARDENED_CLAY(41, 6780213);
MapColor MapColor::PINK_STAINED_HARDENED_CLAY(42, 10505550);
MapColor MapColor::GRAY_STAINED_HARDENED_CLAY(43, 3746083);
MapColor MapColor::SILVER_STAINED_HARDENED_CLAY(44, 8874850);
MapColor MapColor::CYAN_STAINED_HARDENED_CLAY(45, 5725276);
MapColor MapColor::PURPLE_STAINED_HARDENED_CLAY(46, 8014168);
MapColor MapColor::BLUE_STAINED_HARDENED_CLAY(47, 4996700);
MapColor MapColor::BROWN_STAINED_HARDENED_CLAY(48, 4993571);
MapColor MapColor::GREEN_STAINED_HARDENED_CLAY(49, 5001770);
MapColor MapColor::RED_STAINED_HARDENED_CLAY(50, 9321518);
MapColor MapColor::BLACK_STAINED_HARDENED_CLAY(51, 2430480);


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

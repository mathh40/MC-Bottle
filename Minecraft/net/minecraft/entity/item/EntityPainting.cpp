#include "EntityPainting.h"

   EntityPainting::EnumArt EntityPainting::EnumArt::KEBAB = EntityPainting::EnumArt("Kebab", 16, 16, 0, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::AZTEC = EntityPainting::EnumArt("Aztec", 16, 16, 16, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::ALBAN = EntityPainting::EnumArt("Alban", 16, 16, 32, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::AZTEC_2 = EntityPainting::EnumArt("Aztec2", 16, 16, 48, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::BOMB = EntityPainting::EnumArt("Bomb", 16, 16, 64, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::PLANT = EntityPainting::EnumArt("Plant", 16, 16, 80, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::WASTELAND = EntityPainting::EnumArt("Wasteland", 16, 16, 96, 0);
   EntityPainting::EnumArt EntityPainting::EnumArt::POOL = EntityPainting::EnumArt("Pool", 32, 16, 0, 32);
   EntityPainting::EnumArt EntityPainting::EnumArt::COURBET = EntityPainting::EnumArt("Courbet", 32, 16, 32, 32);
   EntityPainting::EnumArt EntityPainting::EnumArt::SEA = EntityPainting::EnumArt("Sea", 32, 16, 64, 32);
   EntityPainting::EnumArt EntityPainting::EnumArt::SUNSET = EntityPainting::EnumArt("Sunset", 32, 16, 96, 32);
   EntityPainting::EnumArt EntityPainting::EnumArt::CREEBET = EntityPainting::EnumArt("Creebet", 32, 16, 128, 32);
   EntityPainting::EnumArt EntityPainting::EnumArt::WANDERER = EntityPainting::EnumArt("Wanderer", 16, 32, 0, 64);
   EntityPainting::EnumArt EntityPainting::EnumArt::GRAHAM = EntityPainting::EnumArt("Graham", 16, 32, 16, 64);
   EntityPainting::EnumArt EntityPainting::EnumArt::MATCH = EntityPainting::EnumArt("Match", 32, 32, 0, 128);
   EntityPainting::EnumArt EntityPainting::EnumArt::BUST = EntityPainting::EnumArt("Bust", 32, 32, 32, 128);
   EntityPainting::EnumArt EntityPainting::EnumArt::STAGE = EntityPainting::EnumArt("Stage", 32, 32, 64, 128);
   EntityPainting::EnumArt  EntityPainting::EnumArt::VOID = EntityPainting::EnumArt("Void", 32, 32, 96, 128);
   EntityPainting::EnumArt EntityPainting::EnumArt::SKULL_AND_ROSES = EntityPainting::EnumArt("SkullAndRoses", 32, 32, 128, 128);
   EntityPainting::EnumArt EntityPainting::EnumArt::WITHER = EntityPainting::EnumArt("Wither", 32, 32, 160, 128);
   EntityPainting::EnumArt EntityPainting::EnumArt:: FIGHTERS = EntityPainting::EnumArt("Fighters", 64, 32, 0, 96);
   EntityPainting::EnumArt EntityPainting::EnumArt::POINTER = EntityPainting::EnumArt("Pointer", 64, 64, 0, 192);
   EntityPainting::EnumArt EntityPainting::EnumArt::PIGSCENE = EntityPainting::EnumArt("Pigscene", 64, 64, 64, 192);
   EntityPainting::EnumArt EntityPainting::EnumArt::BURNING_SKULL = EntityPainting::EnumArt("BurningSkull", 64, 64, 128, 192);
   EntityPainting::EnumArt EntityPainting::EnumArt::SKELETON = EntityPainting::EnumArt("Skeleton", 64, 48, 192, 64);
   EntityPainting::EnumArt EntityPainting::EnumArt::DONKEY_KONG = EntityPainting::EnumArt("DonkeyKong", 64, 48, 192, 112);

EntityPainting::EnumArt::EnumArt(std::string_view titleIn, int32_t width, int32_t height, int32_t textureU,
    int32_t textureV)
        :title(titleIn),sizeX(width),sizeY(height),offsetX(textureU),offsetY(textureV){

}

EntityPainting::EntityPainting(World *worldIn)
    :EntityHanging(worldIn){

}

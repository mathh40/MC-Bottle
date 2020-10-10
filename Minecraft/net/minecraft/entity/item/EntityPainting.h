#pragma once
#include "../EntityHanging.h"

constexpr std::string_view MaxString = "SkullAndRoses";

class EntityPainting :public EntityHanging {
public:
    class EnumArt {
    public:
        static EntityPainting::EnumArt KEBAB;
        static EntityPainting::EnumArt AZTEC;
        static EntityPainting::EnumArt ALBAN;
        static EntityPainting::EnumArt AZTEC_2;
        static EntityPainting::EnumArt BOMB;
        static EntityPainting::EnumArt PLANT;
        static EntityPainting::EnumArt WASTELAND;
        static EntityPainting::EnumArt POOL;
        static EntityPainting::EnumArt COURBET;
        static EntityPainting::EnumArt SEA;
        static EntityPainting::EnumArt SUNSET;
        static EntityPainting::EnumArt CREEBET;
        static EntityPainting::EnumArt WANDERER;
        static EntityPainting::EnumArt GRAHAM;
        static EntityPainting::EnumArt MATCH;
        static EntityPainting::EnumArt BUST;
        static EntityPainting::EnumArt STAGE;
        static EntityPainting::EnumArt VOID;
        static EntityPainting::EnumArt SKULL_AND_ROSES;
        static EntityPainting::EnumArt WITHER;
        static EntityPainting::EnumArt FIGHTERS;
        static EntityPainting::EnumArt POINTER;
        static EntityPainting::EnumArt PIGSCENE;
        static EntityPainting::EnumArt BURNING_SKULL;
        static EntityPainting::EnumArt SKELETON;
        static EntityPainting::EnumArt DONKEY_KONG;

        EnumArt(std::string_view titleIn, int32_t width, int32_t height, int32_t textureU, int32_t textureV);

        static constexpr int32_t MAX_NAME_LENGTH = MaxString.size();
        std::string_view title;
        int32_t sizeX;
        int32_t sizeY;
        int32_t offsetX;
        int32_t offsetY;
    };

    EntityPainting(World* worldIn);

    EntityPainting::EnumArt art;
private:

};
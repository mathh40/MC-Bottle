#pragma once
#include "../EntityHanging.h"

constexpr std::string_view MaxString = "SkullAndRoses";

class EntityPainting : public EntityHanging
{
  public:
    class EnumArt
    {
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

        static std::vector<EnumArt *> values();

        static constexpr int32_t MAX_NAME_LENGTH = MaxString.size();
        std::string_view title;
        int32_t sizeX;
        int32_t sizeY;
        int32_t offsetX;
        int32_t offsetY;

      private:
        static std::vector<EnumArt *> valu;
    };

    EntityPainting(World *worldIn);
    EntityPainting(World *worldIn, BlockPos pos, EnumFacing facing);
    EntityPainting(World *worldIn, BlockPos pos, EnumFacing facing, std::string_view title);

    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    int32_t getWidthPixels() override;
    int32_t getHeightPixels() override;
    void onBroken(Entity *brokenEntity) override;
    void playPlaceSound() override;
    void setLocationAndAngles(double x, double y, double z, float yaw, float pitch) override;
    void setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
                                      int32_t posRotationIncrements, bool teleport) override;

    EntityPainting::EnumArt *art = nullptr;

  private:
};

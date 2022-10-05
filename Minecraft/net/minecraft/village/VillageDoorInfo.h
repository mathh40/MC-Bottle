#pragma once
#include "EnumFacing.h"
#include "math/BlockPos.h"

class VillageDoorInfo
{
  public:
    VillageDoorInfo(BlockPos pos, int32_t deltaX, int32_t deltaZ, int32_t timestamp);
    VillageDoorInfo(BlockPos pos, EnumFacing facing, int32_t timestamp);
    int32_t getDistanceSquared(int32_t x, int32_t y, int32_t z) const;
    int32_t getDistanceToDoorBlockSq(BlockPos pos) const;
    int32_t getDistanceToInsideBlockSq(BlockPos pos) const;
    bool isInsideSide(BlockPos pos);
    void resetDoorOpeningRestrictionCounter();
    void incrementDoorOpeningRestrictionCounter();
    int32_t getDoorOpeningRestrictionCounter() const;
    BlockPos getDoorBlockPos() const;
    BlockPos getInsideBlockPos() const;
    int32_t getInsideOffsetX();
    int32_t getInsideOffsetZ();
    int32_t getLastActivityTimestamp() const;
    void setLastActivityTimestamp(int32_t timestamp);
    bool getIsDetachedFromVillageFlag() const;
    void setIsDetachedFromVillageFlag(bool detached);
    EnumFacing getInsideDirection();

  private:
    static EnumFacing getFaceDirection(int32_t deltaX, int32_t deltaZ);

    BlockPos doorBlockPos;
    BlockPos insideBlock;
    EnumFacing insideDirection;
    int32_t lastActivityTimestamp;
    bool isDetachedFromVillageFlag;
    int32_t doorOpeningRestrictionCounter;
};

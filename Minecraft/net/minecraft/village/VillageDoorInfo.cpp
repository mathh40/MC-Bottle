#include "VillageDoorInfo.h"

VillageDoorInfo::VillageDoorInfo(BlockPos pos, int32_t deltaX, int32_t deltaZ, int32_t timestamp)
	:VillageDoorInfo(pos, getFaceDirection(deltaX, deltaZ), timestamp)
{
}

VillageDoorInfo::VillageDoorInfo(BlockPos pos, EnumFacing facing, int32_t timestamp)
   :doorBlockPos(pos),insideDirection(facing),insideBlock(pos.offset(facing, 2)),lastActivityTimestamp(timestamp)
{
}

int32_t VillageDoorInfo::getDistanceSquared(int32_t x, int32_t y, int32_t z) const
{
   return (int32_t)doorBlockPos.distanceSq((double)x, (double)y, (double)z);
}

int32_t VillageDoorInfo::getDistanceToDoorBlockSq(BlockPos pos) const
{
   return (int32_t)pos.distanceSq(getDoorBlockPos());
}

int32_t VillageDoorInfo::getDistanceToInsideBlockSq(BlockPos pos) const
{
   return (int32_t)insideBlock.distanceSq(pos);
}

bool VillageDoorInfo::isInsideSide(BlockPos pos)
{
   int32_t i = pos.getx() - doorBlockPos.getx();
   int32_t j = pos.getz() - doorBlockPos.gety();
   return i * insideDirection.getXOffset() + j * insideDirection.getZOffset() >= 0;
}

void VillageDoorInfo::resetDoorOpeningRestrictionCounter()
{
   doorOpeningRestrictionCounter = 0;
}

void VillageDoorInfo::incrementDoorOpeningRestrictionCounter()
{
   ++doorOpeningRestrictionCounter;
}

int32_t VillageDoorInfo::getDoorOpeningRestrictionCounter() const
{
   return doorOpeningRestrictionCounter;
}

BlockPos VillageDoorInfo::getDoorBlockPos() const
{
   return doorBlockPos;
}

BlockPos VillageDoorInfo::getInsideBlockPos() const
{
   return insideBlock;
}

int32_t VillageDoorInfo::getInsideOffsetX()
{
   return insideDirection.getXOffset() * 2;
}

int32_t VillageDoorInfo::getInsideOffsetZ()
{
   insideDirection.getZOffset() * 2;
}

int32_t VillageDoorInfo::getLastActivityTimestamp() const
{
   return lastActivityTimestamp;
}

void VillageDoorInfo::setLastActivityTimestamp(int32_t timestamp)
{
   lastActivityTimestamp = timestamp;
}

bool VillageDoorInfo::getIsDetachedFromVillageFlag() const
{
   return isDetachedFromVillageFlag;
}

void VillageDoorInfo::setIsDetachedFromVillageFlag(bool detached)
{
   isDetachedFromVillageFlag = detached;
}

EnumFacing VillageDoorInfo::getInsideDirection()
{
   return insideDirection;
}

EnumFacing VillageDoorInfo::getFaceDirection(int32_t deltaX, int32_t deltaZ)
{
   if (deltaX < 0) 
   {
      return EnumFacing::WEST;
   }
   else if (deltaX > 0) 
   {
      return EnumFacing::EAST;
   }
   else 
   {
      return deltaZ < 0 ? EnumFacing::NORTH : EnumFacing::SOUTH;
   }
}

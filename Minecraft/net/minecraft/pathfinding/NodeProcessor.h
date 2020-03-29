#pragma once
#include "../world/IBlockAccess.h"
#include "PathPoint.h"

class EntityLiving;

class NodeProcessor
{
public:
    virtual void init(IBlockAccess* sourceIn, EntityLiving* mob);
    virtual void postProcess();
  virtual PathPoint getStart() = 0;
  virtual PathPoint getPathPointToCoords(double var1, double var3, double var5) = 0;
  virtual int32_t findPathOptions(const std::vector<PathPoint>& var1, PathPoint var2, PathPoint var3, float var4) = 0;
  virtual PathNodeType getPathNodeType(IBlockAccess* var1, int32_t var2, int32_t var3, int32_t var4, EntityLiving* var5, int32_t var6, int32_t var7, int var8, bool var9, bool var10) = 0;
  virtual PathNodeType getPathNodeType(IBlockAccess* var1, int32_t var2, int32_t var3, int32_t var4) = 0;
  void setCanEnterDoors(bool canEnterDoorsIn);
  void setCanOpenDoors(bool canOpenDoorsIn);
  void setCanSwim(bool canSwimIn);
  bool getCanEnterDoors() const;
  bool getCanOpenDoors() const;
  bool getCanSwim() const;


protected:
  PathPoint openPoint(int32_t x, int32_t y, int32_t z);

  IBlockAccess* blockaccess;
  EntityLiving* entity;
  std::unordered_map<int32_t,PathPoint> pointMap;
  int32_t entitySizeX;
  int32_t entitySizeY;
  int32_t entitySizeZ;
  bool canEnterDoors;
  bool canOpenDoors;
  bool canSwim;
private:
};

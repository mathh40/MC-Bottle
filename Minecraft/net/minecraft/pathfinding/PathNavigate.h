#pragma once
#include <optional>
#include "NodeProcessor.h"
#include "Path.h"
#include "PathFinder.h"

class EntityLiving;
class World;
class IAttributeInstance;
class PathNavigate
{
public:
  PathNavigate(EntityLiving* entityIn, World* worldIn);
  void setSpeed(double speedIn);
  float getPathSearchRange() const;
  bool canUpdatePathOnTimeout() const;
  void updatePath();
  std::optional<Path> getPathToXYZ(double x, double y, double z);
  std::optional<Path> getPathToPos(BlockPos pos);
  virtual std::optional<Path> getPathToEntityLiving(Entity* entityIn);
  bool tryMoveToXYZ(double x, double y, double z, double speedIn);
  virtual bool tryMoveToEntityLiving(Entity* entityIn, double speedIn);
  bool setPath(std::optional<Path> pathentityIn, double speedIn);
  std::optional<Path> getPath() const;
  virtual void onUpdateNavigation();
  bool noPath();
  void clearPath();
  virtual bool canEntityStandOnPos(BlockPos pos);
  NodeProcessor* getNodeProcessor() const;

protected:
  EntityLiving* entity;
  World* world;
  std::optional<Path> currentPath;
  double speed;
  int32_t totalTicks;
  float maxDistanceToWaypoint;
  bool tryUpdatePath;
  NodeProcessor* nodeProcessor;
  virtual PathFinder getPathFinder() = 0;
  virtual void debugPathFinding();
  virtual void pathFollow();
  void checkForStuck(Vec3d positionVec3);
  virtual Vec3d getEntityPosition() = 0;
  virtual bool canNavigate() = 0;
  bool isInLiquid() const;
  virtual void removeSunnyPath();
  virtual bool isDirectPathBetweenPoints(const Vec3d& var1,const Vec3d& var2, int32_t var3, int32_t var4, int32_t var5) = 0;

private:
  IAttributeInstance* pathSearchRange;
  int32_t ticksAtLastPos;
  Vec3d lastPosCheck;
  Vec3d timeoutCachedNode;
  int64_t timeoutTimer;
  int64_t lastTimeoutCheck;
  double timeoutLimit;
  int64_t lastTimeUpdated;
  BlockPos targetPos;
  PathFinder pathFinder;
};

#pragma once
#include <unordered_set>

#include "NodeProcessor.h"
#include "Path.h"
#include "PathPoint.h"

class PathFinder
{
public:
  PathFinder(NodeProcessor* processor);
  std::optional<Path> findPath(IBlockAccess* worldIn, EntityLiving* entitylivingIn, Entity* targetEntity, float maxDistance);
  std::optional<Path> findPath(IBlockAccess* worldIn, EntityLiving* entitylivingIn, BlockPos& targetPos, float maxDistance);




private:

  std::optional<Path> findPath(IBlockAccess* worldIn, EntityLiving* entitylivingIn, double x, double y, double z, float maxDistance);
  std::optional<Path> findPath(PathPoint& pathFrom, PathPoint& pathTo, float maxDistance);
  Path createPath(const PathPoint& start, const PathPoint& end);


  PathHeap path;
  std::unordered_set<> closedSet;
  std::vector<PathPoint> pathOptions;
  NodeProcessor* nodeProcessor;
};

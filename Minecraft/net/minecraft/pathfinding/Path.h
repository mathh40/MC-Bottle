#pragma once
#include <optional>


#include "math/Vec3d.h"
#include "PathPoint.h"

class Entity;

class Path
{
public:
  explicit Path(const std::vector<PathPoint>& pathpoints);
  void incrementPathIndex();
  bool isFinished() const;
  std::optional<PathPoint> getFinalPathPoint();
  PathPoint getPathPointFromIndex(int32_t index);
  void setPoint(int32_t index, PathPoint point);
  int32_t getCurrentPathLength() const;
  void setCurrentPathLength(int32_t length);
  int32_t getCurrentPathIndex() const;
  void setCurrentPathIndex(int32_t currentPathIndexIn);
  Vec3d getVectorFromIndex(const Entity& entityIn, int index);
  Vec3d getPosition(const Entity& entityIn);
  Vec3d getCurrentPos();
  bool isSamePath(const Path& pathentityIn);
  std::vector<PathPoint> getOpenSet() const;
  std::vector<PathPoint> getClosedSet() const;
  PathPoint getTarget() const;
  Path read(const std::vector<PacketBuffer& buf);



private:
  std::vector<PathPoint> points;
  std::vector<PathPoint> openSet;
  std::vector<PathPoint> closedSet;
  PathPoint target;
  int32_t currentPathIndex;
  int32_t pathLength;
};

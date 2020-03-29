#pragma once
#include <array>

#include "PathPoint.h"

class PathHeap
{
public:
  PathHeap();
  void clearPath();
  PathPoint dequeue();
  void changeDistance(PathPoint& point, float distance);

  PathPoint addPoint(PathPoint point);
private:

  void sortBack(int32_t index);
  void sortForward(int32_t index);
  bool isPathEmpty() const;

  std::vector<PathPoint> pathPoints;
  int32_t count;
};

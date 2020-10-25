#pragma once
#include "ByteBuffer.h"
#include "PathNodeType.h"


#include <cstdint>
#include <string>

class PathPoint
{
public:
  PathPoint(int32_t x, int32_t y, int32_t z);
  PathPoint cloneMove(int32_t x, int32_t y, int32_t z) const;
  static int32_t makeHash(int32_t x, int32_t y, int32_t z);
  float distanceTo(PathPoint pathpointIn) const;
  float distanceToSquared(PathPoint pathpointIn) const;
  float distanceManhattan(PathPoint p_186281_1_) const;
  int32_t hashCode() const;
  bool isAssigned() const;
  std::string toString();
  static PathPoint createFromBuffer(const ByteBuffer& buf);

  friend bool operator==(const PathPoint& a,const PathPoint& b);

  int32_t x;
  int32_t y;
  int32_t z;
  int32_t index = -1;
  float totalPathDistance;
  float distanceToNext;
  float distanceToTarget;
  PathPoint* previous;
  bool visited;
  float distanceFromOrigin;
  float cost;
  float costMalus;
  PathNodeType nodeType;
private:
  int32_t hash;
};

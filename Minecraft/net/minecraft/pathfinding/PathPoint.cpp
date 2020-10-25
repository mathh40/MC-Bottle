#include "PathPoint.h"

#include <limits>

#include "math/MathHelper.h"

PathPoint::PathPoint(int32_t x, int32_t y, int32_t z)
  :nodeType(PathNodeType::BLOCKED),x(x),y(y),z(z),hash(makeHash(x, y, z))
{
    
}

PathPoint PathPoint::cloneMove(int32_t x, int32_t y, int32_t z) const
{
  PathPoint pathpoint = PathPoint(x, y, z);
  pathpoint.index = index;
  pathpoint.totalPathDistance = totalPathDistance;
  pathpoint.distanceToNext = distanceToNext;
  pathpoint.distanceToTarget = distanceToTarget;
  pathpoint.previous = previous;
  pathpoint.visited = visited;
  pathpoint.distanceFromOrigin = distanceFromOrigin;
  pathpoint.cost = cost;
  pathpoint.costMalus = costMalus;
  pathpoint.nodeType = nodeType;
  return pathpoint;
}

int32_t PathPoint::makeHash(int32_t x, int32_t y, int32_t z)
{
  auto seed = 0x8000;
  return y & 255 | (x & 32767) << 8 | (z & 32767) << 24 | (x < 0 ? std::numeric_limits<int32_t>::min() : 0) | (z < 0 ? seed : 0);
}

float PathPoint::distanceTo(PathPoint pathpointIn) const
{
  float f = (float)(pathpointIn.x - x);
  float f1 = (float)(pathpointIn.y - y);
  float f2 = (float)(pathpointIn.z - z);
  return MathHelper::sqrt(f * f + f1 * f1 + f2 * f2);
}

float PathPoint::distanceToSquared(PathPoint pathpointIn) const
{
  float f = (float)(pathpointIn.x - x);
  float f1 = (float)(pathpointIn.y - y);
  float f2 = (float)(pathpointIn.z - z);
  return f * f + f1 * f1 + f2 * f2;
}

float PathPoint::distanceManhattan(PathPoint p_186281_1_) const
{
  float f = (float)MathHelper::abs(p_186281_1_.x - x);
  float f1 = (float)MathHelper::abs(p_186281_1_.y - y);
  float f2 = (float)MathHelper::abs(p_186281_1_.z - z);
  return f + f1 + f2;
}

int32_t PathPoint::hashCode() const
{
  return hash;
}

bool PathPoint::isAssigned() const
{
  return index >= 0;
}

std::string PathPoint::toString()
{
  return fmt::format(x + ", " + y + ", " + z);
}

PathPoint PathPoint::createFromBuffer(const ByteBuffer& buf)
{
  PathPoint pathpoint = PathPoint(buf.getInt(), buf.getInt(), buf.getInt());
  pathpoint.distanceFromOrigin = buf.getFloat();
  pathpoint.cost = buf.getFloat();
  pathpoint.costMalus = buf.getFloat();
  pathpoint.visited = buf.readBoolean();
  pathpoint.nodeType = PathNodeType::values()[buf.getInt()];
  pathpoint.distanceToTarget = buf.getFloat();
  return pathpoint;
}

bool operator==(const PathPoint &a, const PathPoint &b)
{
  return a.hash == b.hash && a.x == b.x && a.y == b.y && a.z == b.z;
}

#include "Path.h"

Path::Path(const std::vector<PathPoint> &pathpoints)
  : points(pathpoints), target(0,0,0), pathLength(pathpoints.size()) { }

void Path::incrementPathIndex()
{
  ++currentPathIndex;
}

bool Path::isFinished() const
{
  return currentPathIndex >= pathLength;
}

std::optional<PathPoint> Path::getFinalPathPoint()
{
  return pathLength > 0 ? std::optional<PathPoint>(points[pathLength - 1]) : std::nullopt;
}

PathPoint Path::getPathPointFromIndex(int32_t index)
{
  return points[index];
}

void Path::setPoint(int32_t index, PathPoint point)
{
  points[index] = point;
}

int32_t Path::getCurrentPathLength() const
{
  return pathLength;
}

void Path::setCurrentPathLength(int32_t length)
{
  pathLength = length;
}

int32_t Path::getCurrentPathIndex() const
{
  return currentPathIndex;
}

void Path::setCurrentPathIndex(int32_t currentPathIndexIn)
{
  currentPathIndex = currentPathIndexIn;
}

Vec3d Path::getVectorFromIndex(const Entity &entityIn, int index)
{
  double d0 = (double)points[index].x + (double)((int)(entityIn.width + 1.0F)) * 0.5;
  double d1 = (double)points[index].y;
  double d2 = (double)points[index].z + (double)((int)(entityIn.width + 1.0F)) * 0.5;
  return Vec3d(d0, d1, d2);
}

Vec3d Path::getPosition(const Entity &entityIn)
{
  return getVectorFromIndex(entityIn, currentPathIndex);
}

Vec3d Path::getCurrentPos()
{
  PathPoint pathpoint = points[currentPathIndex];
  return Vec3d((double)pathpoint.x, (double)pathpoint.y, (double)pathpoint.z);
}

bool Path::isSamePath(const Path &pathentityIn)
{
  if (pathentityIn == nullptr) 
  {
    return false;
  }
  else if (pathentityIn.points.size() != points.size()) 
  {
    return false;
  }
  else 
  {
    for(auto i = 0; i < points.size(); ++i) 
    {
      if (points[i].x != pathentityIn.points[i].x || points[i].y != pathentityIn.points[i].y || points[i].z != pathentityIn.points[i].z) 
      {
        return false;
      }
    }

    return true;
  }
}

std::vector<PathPoint> Path::getOpenSet() const
{
  return openSet;
}

std::vector<PathPoint> Path::getClosedSet() const
{
  return closedSet;
}

PathPoint Path::getTarget() const
{
  return target;
}

Path Path::read(const PacketBuffer &buf)
{
  int32_t i = buf.readInt();
  PathPoint pathpoint = PathPoint::createFromBuffer(buf);
  std::vector<PathPoint> apathpoint(buf.readInt());

  for(int32_t j = 0; j < apathpoint.size(); ++j) 
  {
    apathpoint[j] = PathPoint::createFromBuffer(buf);
  }

  std::vector<PathPoint> apathpoint1(buf.readInt());

  for(int32_t k = 0; k < apathpoint1.size(); ++k) 
  {
    apathpoint1[k] = PathPoint::createFromBuffer(buf);
  }

  std::vector<PathPoint> apathpoint2(buf.readInt());

  for(int32_t l = 0; l < apathpoint2.size(); ++l) 
  {
    apathpoint2[l] = PathPoint::createFromBuffer(buf);
  }

  Path path = Path(apathpoint);
  path.openSet = apathpoint1;
  path.closedSet = apathpoint2;
  path.target = pathpoint;
  path.currentPathIndex = i;
  return path;
}

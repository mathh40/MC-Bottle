#include "PathHeap.h"

#include "spdlog/sinks/null_sink.h"

PathHeap::PathHeap()
{
    pathPoints.resize(128);
}

void PathHeap::clearPath()
{
    count = 0;
}

PathPoint PathHeap::dequeue()
{
    PathPoint pathpoint = pathPoints[0];
    pathPoints[0]       = pathPoints[--count];
    pathPoints[count]   = nullptr;
    if (count > 0)
    {
        sortForward(0);
    }

    pathpoint.index = -1;
    return pathpoint;
}

void PathHeap::changeDistance(PathPoint &point, float distance)
{
    float f                = point.distanceToTarget;
    point.distanceToTarget = distance;
    if (distance < f)
    {
        sortBack(point.index);
    }
    else
    {
        sortForward(point.index);
    }
}

PathPoint PathHeap::addPoint(PathPoint point)
{
    if (point.index >= 0)
    {
        throw std::runtime_error("OW KNOWS!");
    }
    else
    {
        if (count == pathPoints.size())
        {
            std::vector<PathPoint> apathpoint(count << 1);
            std::copy(pathPoints.begin(), pathPoints.begin() + count, apathpoint.begin());
            pathPoints = apathpoint;
        }

        pathPoints[count] = point;
        point.index       = count;
        sortBack(count++);
        return point;
    }
}

void PathHeap::sortBack(int32_t index)
{
    PathPoint &pathpoint = pathPoints[index];

    int32_t i;
    for (float f = pathpoint.distanceToTarget; index > 0; index = i)
    {
        i                     = index - 1 >> 1;
        PathPoint &pathpoint1 = pathPoints[i];
        if (f >= pathpoint1.distanceToTarget)
        {
            break;
        }

        pathPoints[index] = pathpoint1;
        pathpoint1.index  = index;
    }

    pathPoints[index] = pathpoint;
    pathpoint.index   = index;
}

void PathHeap::sortForward(int32_t index)
{
    PathPoint &pathpoint = pathPoints[index];
    float f              = pathpoint.distanceToTarget;

    while (true)
    {
        int i = 1 + (index << 1);
        int j = i + 1;
        if (i >= count)
        {
            break;
        }

        PathPoint &pathpoint1 = pathPoints[i];
        float f1              = pathpoint1.distanceToTarget;
        PathPoint *pathpoint2 = nullptr;
        float f2;
        if (j >= count)
        {
            pathpoint2 = nullptr;
            f2         = std::numeric_limits<float>::infinity();
        }
        else
        {
            pathpoint2 = &pathPoints[j];
            f2         = pathpoint2->distanceToTarget;
        }

        if (f1 < f2)
        {
            if (f1 >= f)
            {
                break;
            }

            pathPoints[index] = pathpoint1;
            pathpoint1.index  = index;
            index             = i;
        }
        else
        {
            if (f2 >= f)
            {
                break;
            }

            pathPoints[index] = *pathpoint2;
            pathpoint2->index = index;
            index             = j;
        }
    }

    pathPoints[index] = pathpoint;
    pathpoint.index   = index;
}

bool PathHeap::isPathEmpty() const
{
    return count == 0;
}

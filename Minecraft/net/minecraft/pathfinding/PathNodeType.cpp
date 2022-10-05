#include "PathNodeType.h"

PathNodeType PathNodeType::BLOCKED          = PathNodeType(-1.0F);
PathNodeType PathNodeType::OPEN             = PathNodeType(0.0F);
PathNodeType PathNodeType::WALKABLE         = PathNodeType(0.0F);
PathNodeType PathNodeType::TRAPDOOR         = PathNodeType(0.0F);
PathNodeType PathNodeType::FENCE            = PathNodeType(-1.0F);
PathNodeType PathNodeType::LAVA             = PathNodeType(-1.0F);
PathNodeType PathNodeType::WATER            = PathNodeType(8.0F);
PathNodeType PathNodeType::RAIL             = PathNodeType(0.0F);
PathNodeType PathNodeType::DANGER_FIRE      = PathNodeType(8.0F);
PathNodeType PathNodeType::DAMAGE_FIRE      = PathNodeType(16.0F);
PathNodeType PathNodeType::DANGER_CACTUS    = PathNodeType(8.0F);
PathNodeType PathNodeType::DAMAGE_CACTUS    = PathNodeType(-1.0F);
PathNodeType PathNodeType::DANGER_OTHER     = PathNodeType(8.0F);
PathNodeType PathNodeType::DAMAGE_OTHER     = PathNodeType(-1.0F);
PathNodeType PathNodeType::DOOR_OPEN        = PathNodeType(0.0F);
PathNodeType PathNodeType::DOOR_WOOD_CLOSED = PathNodeType(-1.0F);
PathNodeType PathNodeType::DOOR_IRON_CLOSED = PathNodeType(-1.0F);

PathNodeType::PathNodeType(float priorityIn) : priority(priorityIn)
{
}

float PathNodeType::getPriority() const
{
    return priority;
}

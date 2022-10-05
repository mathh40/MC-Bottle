#pragma once
#include "../../pathfinding/Path.h"
#include "../../village/VillageDoorInfo.h"
#include "EntityAIBase.h"

class Village;
class EntityCreature;

class EntityAIMoveThroughVillage : public EntityAIBase
{
  public:
    EntityAIMoveThroughVillage(EntityCreature *entityIn, double movementSpeedIn, bool isNocturnalIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;

  private:
    std::optional<VillageDoorInfo> findNearestDoor(const Village &villageIn);
    bool doesDoorListContain(VillageDoorInfo doorInfoIn);
    void resizeDoorList();

    EntityCreature *entity;
    double movementSpeed;
    std::optional<Path> path;
    std::optional<VillageDoorInfo> doorInfo;
    bool isNocturnal;
    std::vector<VillageDoorInfo> doorList;
};

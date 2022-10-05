#pragma once
#include "../world/World.h"
#include "VillageDoorInfo.h"

class Village
{
  public:
    Village();
    explicit Village(World *worldIn);
    void setWorld(World *worldIn);
    void tick(int32_t tickCounterIn);
    BlockPos getCenter() const;
    int32_t getVillageRadius() const;
    int32_t getNumVillageDoors();
    int32_t getTicksSinceLastDoorAdding() const;
    int32_t getNumVillagers() const;
    bool isBlockPosWithinSqVillageRadius(BlockPos pos) const;
    std::vector<> getVillageDoorInfoList();
    VillageDoorInfo getNearestDoor(BlockPos pos);
    VillageDoorInfo getDoorInfo(BlockPos pos);
    VillageDoorInfo getExistedDoor(BlockPos doorBlock);
    void addVillageDoorInfo(VillageDoorInfo doorInfo);
    bool isAnnihilated();
    void addOrRenewAgressor(EntityLivingBase *entitylivingbaseIn);
    EntityLivingBase *findNearestVillageAggressor(EntityLivingBase *entitylivingbaseIn);
    EntityPlayer *getNearestTargetPlayer(EntityLivingBase *villageDefender);
    int32_t getPlayerReputation(std::string playerName);
    int32_t modifyPlayerReputation(std::string playerName, int32_t reputation);
    bool isPlayerReputationTooLow(std::string playerName);
    void readVillageDataFromNBT(NBTTagCompound *compound);
    void writeVillageDataToNBT(NBTTagCompound *compound);
    void endMatingSeason();
    bool isMatingSeason() const;
    void setDefaultPlayerReputation(int32_t defaultReputation);

    struct VillageAggressor
    {
        EntityLivingBase *agressor;
        int32_t agressionTime;

        VillageAggressor(EntityLivingBase *agressorIn, int32_t agressionTimeIn)
        {
            agressor      = agressorIn;
            agressionTime = agressionTimeIn;
        }
    };

  private:
    std::optional<Vec3d> findRandomSpawnPos(BlockPos pos, int32_t x, int32_t y, int32_t z);
    bool isAreaClearAround(BlockPos blockSize, BlockPos blockLocation) const;
    void updateNumIronGolems();
    void updateNumVillagers();
    void removeDeadAndOldAgressors();
    void removeDeadAndOutOfRangeDoors();
    bool isWoodDoor(BlockPos pos);
    void updateVillageRadiusAndCenter();

    World *world;
    std::vector<> villageDoorInfoList;
    BlockPos centerHelper;
    BlockPos center;
    int32_t villageRadius;
    int32_t lastAddDoorTimestamp;
    int32_t tickCounter;
    int32_t numVillagers;
    int32_t noBreedTicks;
    std::unordered_map<std::string, int32_t> playerReputation;
    std::vector<Village::VillageAggressor> villageAgressors;
    int numIronGolems;
};

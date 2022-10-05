#include "../world/World.h"
#include "../world/storage/WorldSavedData.h"
#include "Village.h"

class VillageCollection : public WorldSavedData
{
  public:
    VillageCollection(std::string_view name);
    explicit VillageCollection(World *worldIn);
    void setWorldsForAll(World *worldIn);
    void addToVillagerPositionList(BlockPos pos);
    void tick();
    std::vector<Village> getVillageList();
    Village getNearestVillage(BlockPos doorBlock, int32_t radius);
    void readFromNBT(NBTTagCompound *nbt) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    static std::string fileNameForProvider(WorldProvider &provider);

  private:
    void removeAnnihilatedVillages();
    void dropOldestVillagerPosition();
    void addNewDoorsToVillageOrCreateVillage();
    void addDoorsAround(BlockPos central);
    VillageDoorInfo checkDoorExistence(BlockPos doorBlock);
    void addToNewDoorsList(BlockPos doorBlock);
    int32_t countBlocksCanSeeSky(BlockPos centerPos, EnumFacing direction, int32_t limitation);
    bool positionInList(BlockPos pos);
    bool isWoodDoor(BlockPos doorPos);

    World *world;
    std::vector<BlockPos> villagerPositionsList;
    std::vector<VillageDoorInfo> newDoors;
    std::vector<Village> villageList;
    int32_t tickCounter;
};

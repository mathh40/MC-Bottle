#include "VillageCollection.h"

#include "../world/WorldProvider.h"
#include "Village.h"

VillageCollection::VillageCollection(std::string_view name) : WorldSavedData(name)
{
}

VillageCollection::VillageCollection(World *worldIn)
    : WorldSavedData(fileNameForProvider(worldIn->provider)), world(worldIn)
{
    markDirty();
}

void VillageCollection::setWorldsForAll(World *worldIn)
{
    world = worldIn;
    for (auto village : villageList)
    {
        village.setWorld(worldIn);
    }
}

void VillageCollection::addToVillagerPositionList(BlockPos pos)
{
    if (villagerPositionsList.size() <= 64 && !positionInList(pos))
    {
        villagerPositionsList.add(pos);
    }
}

void VillageCollection::tick()
{
    ++tickCounter;
    Iterator var1 = villageList.iterator();

    while (var1.hasNext())
    {
        Village village = (Village)var1.next();
        village.tick(tickCounter);
    }

    removeAnnihilatedVillages();
    dropOldestVillagerPosition();
    addNewDoorsToVillageOrCreateVillage();
    if (tickCounter % 400 == 0)
    {
        markDirty();
    }
}

std::vector<> VillageCollection::getVillageList()
{
    return villageList;
}

Village VillageCollection::getNearestVillage(BlockPos doorBlock, int32_t radius)
{
    Village village;
    double d0 = 3.4028234663852886E38;

    for (auto village1 : villageList)
    {
        double d1 = village1.getCenter().distanceSq(doorBlock);
        if (d1 < d0)
        {
            float f = (float)(radius + village1.getVillageRadius());
            if (d1 <= (double)(f * f))
            {
                village = village1;
                d0      = d1;
            }
        }
    }

    return village;
}

void VillageCollection::readFromNBT(NBTTagCompound *nbt)
{
    tickCounter            = nbt->getInteger("Tick");
    NBTTagList *nbttaglist = nbt->getTagList("Villages", 10);

    for (int i = 0; i < nbttaglist->tagCount(); ++i)
    {
        NBTTagCompound *nbttagcompound = nbttaglist->getCompoundTagAt(i);
        Village village;
        village.readVillageDataFromNBT(nbttagcompound);
        villageList.emplace_back(village);
    }
}

NBTTagCompound *VillageCollection::writeToNBT(NBTTagCompound *compound)
{
    compound->setInteger("Tick", tickCounter);
    NBTTagList *nbttaglist = new NBTTagList();
    Iterator var3          = villageList.iterator();

    while (var3.hasNext())
    {
        Village village                = (Village)var3.next();
        NBTTagCompound *nbttagcompound = new NBTTagCompound();
        village.writeVillageDataToNBT(nbttagcompound);
        nbttaglist->appendTag(nbttagcompound);
    }

    compound->setTag("Villages", nbttaglist);
    return compound;
}

std::string VillageCollection::fileNameForProvider(WorldProvider &provider)
{
    return "villages" + provider.getDimensionType().getSuffix();
}

void VillageCollection::removeAnnihilatedVillages()
{
    Iterator iterator = villageList.iterator();

    while (iterator.hasNext())
    {
        Village village = (Village)iterator.next();
        if (village.isAnnihilated())
        {
            iterator.remove();
            markDirty();
        }
    }
}

void VillageCollection::dropOldestVillagerPosition()
{
    if (!villagerPositionsList.empty())
    {
        addDoorsAround(villagerPositionsList.remove(0));
    }
}

void VillageCollection::addNewDoorsToVillageOrCreateVillage()
{
    for (int32_t i = 0; i < newDoors.size(); ++i)
    {
        VillageDoorInfo villagedoorinfo = (newDoors[i]);
        Village village                 = getNearestVillage(villagedoorinfo.getDoorBlockPos(), 32);
        if (village == nullptr)
        {
            village = Village(world);
            villageList.add(village);
            markDirty();
        }

        village.addVillageDoorInfo(villagedoorinfo);
    }

    newDoors.clear();
}

void VillageCollection::addDoorsAround(BlockPos central)
{
    for (int32_t l = -16; l < 16; ++l)
    {
        for (int32_t i1 = -4; i1 < 4; ++i1)
        {
            for (int32_t j1 = -16; j1 < 16; ++j1)
            {
                BlockPos blockpos = central.add(l, i1, j1);
                if (isWoodDoor(blockpos))
                {
                    VillageDoorInfo villagedoorinfo = checkDoorExistence(blockpos);
                    if (villagedoorinfo == nullptr)
                    {
                        addToNewDoorsList(blockpos);
                    }
                    else
                    {
                        villagedoorinfo.setLastActivityTimestamp(tickCounter);
                    }
                }
            }
        }
    }
}

VillageDoorInfo VillageCollection::checkDoorExistence(BlockPos doorBlock)
{
    Iterator var2 = newDoors.iterator();

    VillageDoorInfo villagedoorinfo;
    do
    {
        if (!var2.hasNext())
        {
            var2 = villageList.iterator();

            VillageDoorInfo villagedoorinfo1;
            do
            {
                if (!var2.hasNext())
                {
                    return fmt::internal::null;
                }

                Village village  = (Village)var2.next();
                villagedoorinfo1 = village.getExistedDoor(doorBlock);
            } while (villagedoorinfo1 == fmt::internal::null);

            return villagedoorinfo1;
        }

        villagedoorinfo = (VillageDoorInfo)var2.next();
    } while (villagedoorinfo.getDoorBlockPos().getX() != doorBlock.getx() ||
             villagedoorinfo.getDoorBlockPos().getZ() != doorBlock.getz() ||
             MathHelper::abs(villagedoorinfo.getDoorBlockPos().getY() - doorBlock.gety()) > 1);

    return villagedoorinfo;
}

void VillageCollection::addToNewDoorsList(BlockPos doorBlock)
{
    EnumFacing enumfacing  = BlockDoor.getFacing(world, doorBlock);
    EnumFacing enumfacing1 = enumfacing.getOpposite();
    auto i                 = countBlocksCanSeeSky(doorBlock, enumfacing, 5);
    auto j                 = countBlocksCanSeeSky(doorBlock, enumfacing1, i + 1);
    if (i != j)
    {
        newDoors.push_pack(VillageDoorInfo(doorBlock, i < j ? enumfacing : enumfacing1, tickCounter));
    }
}

bool VillageCollection::positionInList(BlockPos pos)
{
    Iterator var2 = villagerPositionsList.iterator();

    BlockPos blockpos;
    do
    {
        if (!var2.hasNext())
        {
            return false;
        }

        blockpos = (BlockPos)var2.next();
    } while (!blockpos == pos);

    return true;
}

bool VillageCollection::isWoodDoor(BlockPos doorPos)
{
    IBlockState *iblockstate = world->getBlockState(doorPos);
    Block block              = iblockstate->getBlock();
    if (Util:: instanceof <BlockDoor>(block))
    {
        return iblockstate->getMaterial() == Material::WOOD;
    }
    else
    {
        return false;
    }
}

int32_t VillageCollection::countBlocksCanSeeSky(BlockPos centerPos, EnumFacing direction, int32_t limitation)
{
    auto i = 0;

    for (auto j = 1; j <= 5; ++j)
    {
        if (world->canSeeSky(centerPos.offset(direction, j)))
        {
            ++i;
            if (i >= limitation)
            {
                return i;
            }
        }
    }

    return i;
}

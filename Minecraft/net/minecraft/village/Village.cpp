#include "Village.h"

Village::Village()
	:centerHelper(BlockPos::ORIGIN)
	 ,center(BlockPos::ORIGIN)
{	
}

Village::Village(World* worldIn)
:centerHelper(BlockPos::ORIGIN)
	 ,center(BlockPos::ORIGIN)
	,world(worldIn)
{
}

void Village::setWorld(World* worldIn)
{
	world = worldIn;
}

void Village::tick(int32_t tickCounterIn)
{
	tickCounter = tickCounterIn;
      removeDeadAndOutOfRangeDoors();
      removeDeadAndOldAgressors();
      if (tickCounterIn % 20 == 0) {
         updateNumVillagers();
      }

      if (tickCounterIn % 30 == 0) {
         updateNumIronGolems();
      }

      auto i = numVillagers / 10;
      if (numIronGolems < i && villageDoorInfoList.size() > 20 && world->rand(7000) == 0) {
         Vec3d vec3d = findRandomSpawnPos(center, 2, 4, 2);
         if (vec3d != fmt::internal::null) {
            EntityIronGolem* entityirongolem = new EntityIronGolem(world);
            entityirongolem->setPosition(vec3d.getx(), vec3d.gety(), vec3d.getz());
            world->spawnEntity(entityirongolem);
            ++numIronGolems;
         }
      }
}

bool Village::isAreaClearAround(BlockPos blockSize, BlockPos blockLocation) const
{
   if (!world->getBlockState(blockLocation.down()).isTopSolid()) 
   {
      return false;
   }
   else 
   {
      auto i = blockLocation.getx() - blockSize.getx() / 2;
      auto j = blockLocation.getz() - blockSize.getz() / 2;

      for(auto k = i; k < i + blockSize.getx(); ++k) {
         for(auto l = blockLocation.gety(); l < blockLocation.gety() + blockSize.gety(); ++l) {
            for(auto i1 = j; i1 < j + blockSize.getz(); ++i1) {
               if (world->getBlockState(BlockPos(k, l, i1)).isNormalCube()) {
                  return false;
               }
            }
         }
      }

      return true;
   }
}

void Village::updateNumIronGolems()
{
   auto list = world->getEntitiesWithinAABB(EntityIronGolem.class, AxisAlignedBB((double)(center.getx() - villageRadius), (double)(center.gety() - 4), (double)(center.getz() - villageRadius), (double)(center.getx() + villageRadius), (double)(center.gety() + 4), (double)(center.getz() + villageRadius)));
   numIronGolems = list.size();
}

void Village::updateNumVillagers()
{
   auto list = world->getEntitiesWithinAABB(EntityVillager.class, AxisAlignedBB((double)(center.getx() - villageRadius), (double)(center.gety() - 4), (double)(center.getz() - villageRadius), (double)(center.getx() + villageRadius), (double)(center.gety() + 4), (double)(center.getz() + villageRadius)));
   numVillagers = list.size();
   if (numVillagers == 0) 
   {
      playerReputation.clear();
   }
}

void Village::removeDeadAndOldAgressors()
{
   auto iterator = villageAgressors.begin();

   while(true) 
   {
      Village::VillageAggressor village$villageaggressor;
      do 
      {
         if (iterator == villageDoorInfoList.end()) 
         {
            return;
         }

         village$villageaggressor = *iterator++;
      }
      while(village$villageaggressor.agressor.isEntityAlive() && MathHelper::abs(tickCounter - village$villageaggressor.agressionTime) <= 300);

      iterator.remove();
   }
}

void Village::removeDeadAndOutOfRangeDoors()
{
   bool flag = false;
   bool flag1 = world.rand(50) == 0;
   Iterator iterator = this.villageDoorInfoList.iterator();

   while(true) {
      VillageDoorInfo villagedoorinfo;
      do {
         if (!iterator.hasNext()) {
            if (flag) {
               updateVillageRadiusAndCenter();
            }

            return;
         }

         villagedoorinfo = (VillageDoorInfo)iterator.next();
         if (flag1) {
            villagedoorinfo.resetDoorOpeningRestrictionCounter();
         }
      } while(isWoodDoor(villagedoorinfo.getDoorBlockPos()) && MathHelper::abs(tickCounter - villagedoorinfo.getLastActivityTimestamp()) <= 1200);

      centerHelper = centerHelper.subtract(villagedoorinfo.getDoorBlockPos());
      flag = true;
      villagedoorinfo.setIsDetachedFromVillageFlag(true);
      iterator.remove();
   }
}

bool Village::isWoodDoor(BlockPos pos)
{
   IBlockState* iblockstate = world->getBlockState(pos);
   Block block = iblockstate->getBlock();
   if (Util::instanceof<BlockDoor>(block)) {
      return iblockstate->getMaterial() == Material::WOOD;
   } else {
      return false;
   }
}

void Village::updateVillageRadiusAndCenter()
{
   auto i = villageDoorInfoList.size();
      if (i == 0) {
         center = BlockPos::ORIGIN;
         villageRadius = 0;
      } else {
         center = BlockPos(centerHelper.getx() / i, centerHelper.gety() / i, centerHelper.getz() / i);
         auto j = 0;

         VillageDoorInfo villagedoorinfo;
         for(Iterator var3 = villageDoorInfoList.iterator(); var3.hasNext(); j = MathHelper::max(villagedoorinfo.getDistanceToDoorBlockSq(center), j)) {
            villagedoorinfo = (VillageDoorInfo)var3.next();
         }

         villageRadius = MathHelper::max(32, MathHelper::sqrt(j) + 1);
      }
}



BlockPos Village::getCenter() const
{
   return center;
}

int32_t Village::getVillageRadius() const
{
   return villageRadius;
}

int32_t Village::getNumVillageDoors()
{
   return villageDoorInfoList.size();
}

int32_t Village::getTicksSinceLastDoorAdding() const
{
   return tickCounter - lastAddDoorTimestamp;
}

int32_t Village::getNumVillagers() const
{
   return numVillagers;
}

bool Village::isBlockPosWithinSqVillageRadius(BlockPos pos) const
{
   return center.distanceSq(pos) < (double)(villageRadius * villageRadius);
}

std::vector<> Village::getVillageDoorInfoList()
{
   return villageDoorInfoList;
}

VillageDoorInfo Village::getNearestDoor(BlockPos pos)
{
   VillageDoorInfo villagedoorinfo;
   auto i = std::numeric_limits<int32_t>::max();

   for(auto villagedoorinfo1 : villageDoorInfoList)
   {
      auto j = villagedoorinfo1.getDistanceToDoorBlockSq(pos);
      if (j < i) 
      {
         villagedoorinfo = villagedoorinfo1;
         i = j;
      }
   }

   return villagedoorinfo;
}

VillageDoorInfo Village::getDoorInfo(BlockPos pos)
{
   VillageDoorInfo villagedoorinfo;
   auto i = std::numeric_limits<int32_t>::max();

   for(auto villagedoorinfo1 : villageDoorInfoList)
   {
      auto j = villagedoorinfo1.getDistanceToDoorBlockSq(pos);
      if (j > 256) 
      {
         j *= 1000;
      }
      else 
      {
         j = villagedoorinfo1.getDoorOpeningRestrictionCounter();
      }

      if (j < i) 
      {
         BlockPos blockpos = villagedoorinfo1.getDoorBlockPos();
         EnumFacing enumfacing = villagedoorinfo1.getInsideDirection();
         if (world->getBlockState(blockpos.offset(enumfacing, 1)).getBlock().isPassable(world, blockpos.offset(enumfacing, 1)) && world->getBlockState(blockpos.offset(enumfacing, -1)).getBlock().isPassable(world, blockpos.offset(enumfacing, -1)) && world->getBlockState(blockpos.up().offset(enumfacing, 1)).getBlock().isPassable(world, blockpos.up().offset(enumfacing, 1)) && world->getBlockState(blockpos.up().offset(enumfacing, -1)).getBlock().isPassable(world, blockpos.up().offset(enumfacing, -1))) {
            villagedoorinfo = villagedoorinfo1;
            i = j;
         }
      }
   }

   return villagedoorinfo;
}

std::optional<VillageDoorInfo> Village::getExistedDoor(BlockPos doorBlock)
{
   if (center.distanceSq(doorBlock) > (double)(villageRadius * villageRadius)) {
      return std::nullopt;
      } else {

         auto var2 = villageDoorInfoList.begin();

         VillageDoorInfo villagedoorinfo;
         do 
         {
            if (var2 == villageDoorInfoList.end()) 
            {
               return std::nullopt;
            }

            villagedoorinfo = *var2++;
         }
         while(villagedoorinfo.getDoorBlockPos().getX() != doorBlock.getx() || villagedoorinfo.getDoorBlockPos().getZ() != doorBlock.getz() || MathHelper::abs(villagedoorinfo.getDoorBlockPos().getY() - doorBlock.gety()) > 1);

         return villagedoorinfo;
      }
}
void Village::addVillageDoorInfo(VillageDoorInfo doorInfo)
{
   villageDoorInfoList.add(doorInfo);
   centerHelper = centerHelper.add(doorInfo.getDoorBlockPos());
   updateVillageRadiusAndCenter();
   lastAddDoorTimestamp = doorInfo.getLastActivityTimestamp();
}

bool Village::isAnnihilated()
{
   return villageDoorInfoList.isEmpty();
}

void Village::addOrRenewAgressor(EntityLivingBase* entitylivingbaseIn)
{
   auto var2 = villageAgressors.begin();

      Village::VillageAggressor village$villageaggressor;
      do 
      {
         if (var2 == villageAgressors.end()) 
         {
            villageAgressors.add(Village::VillageAggressor(entitylivingbaseIn, tickCounter));
            return;
         }

         village$villageaggressor = *var2++;
      } while(village$villageaggressor.agressor != entitylivingbaseIn);

      village$villageaggressor.agressionTime = tickCounter;
}

EntityLivingBase* Village::findNearestVillageAggressor(EntityLivingBase* entitylivingbaseIn)
{
   double d0 = std::numeric_limits<double>::max();
   Village::VillageAggressor village$villageaggressor;

   for(auto i = 0; i < villageAgressors.size(); ++i) 
   {
      Village::VillageAggressor village$villageaggressor1 = villageAgressors[i];
      double d1 = village$villageaggressor1.agressor.getDistanceSq(entitylivingbaseIn);
      if (d1 <= d0) {
         village$villageaggressor = village$villageaggressor1;
         d0 = d1;
      }
   }

   return village$villageaggressor == fmt::internal::null ? fmt::internal::null : village$villageaggressor.agressor;
}

EntityPlayer* Village::getNearestTargetPlayer(EntityLivingBase* villageDefender)
{
   double d0 = std::numeric_limits<double>::max();
   EntityPlayer* entityplayer = nullptr;

   for(auto s :playerReputation)
   {
      if (isPlayerReputationTooLow(s)) 
      {
         EntityPlayer* entityplayer1 = world.getPlayerEntityByName(s);
         if (entityplayer1 != nullptr) 
         {
            double d1 = entityplayer1->getDistanceSq(villageDefender);
            if (d1 <= d0) {
               entityplayer = entityplayer1;
               d0 = d1;
            }
         }
      }
   }

   return entityplayer;
}

int32_t Village::getPlayerReputation(std::string playerName)
{
   int32_t integer = playerReputation.get(playerName);
   return integer integer;
}

int32_t Village::modifyPlayerReputation(std::string playerName, int32_t reputation)
{
   int32_t i = getPlayerReputation(playerName);
   int32_t j = MathHelper::clamp(i + reputation, -30, 10);
   playerReputation.emplace(playerName, j);
   return j;
}

bool Village::isPlayerReputationTooLow(std::string playerName)
{
   return getPlayerReputation(playerName) <= -15;
}

void Village::readVillageDataFromNBT(NBTTagCompound* compound)
{
   numVillagers = compound->getInteger("PopSize");
      villageRadius = compound->getInteger("Radius");
      numIronGolems = compound->getInteger("Golems");
      lastAddDoorTimestamp = compound->getInteger("Stable");
      tickCounter = compound->getInteger("Tick");
      noBreedTicks = compound->getInteger("MTick");
      center = BlockPos(compound->getInteger("CX"), compound->getInteger("CY"), compound->getInteger("CZ"));
      centerHelper = BlockPos(compound->getInteger("ACX"), compound->getInteger("ACY"), compound->getInteger("ACZ"));
      NBTTagList* nbttaglist = compound->getTagList("Doors", 10);

      for(auto i = 0; i < nbttaglist->tagCount(); ++i) {
         NBTTagCompound* nbttagcompound = nbttaglist.getCompoundTagAt(i);
         VillageDoorInfo villagedoorinfo = new VillageDoorInfo(new BlockPos(nbttagcompound.getInteger("X"), nbttagcompound.getInteger("Y"), nbttagcompound.getInteger("Z")), nbttagcompound.getInteger("IDX"), nbttagcompound.getInteger("IDZ"), nbttagcompound.getInteger("TS"));
         villageDoorInfoList.add(villagedoorinfo);
      }

      NBTTagList* nbttaglist1 = compound->getTagList("Players", 10);

      for(auto j = 0; j < nbttaglist1->tagCount(); ++j) {
         NBTTagCompound* nbttagcompound1 = nbttaglist1->getCompoundTagAt(j);
         if (nbttagcompound1->hasKey("UUID") && world != nullptr && world->getMinecraftServer() != nullptr) 
         {
            PlayerProfileCache playerprofilecache = world->getMinecraftServer().getPlayerProfileCache();
            xg::Guid uuid(nbttagcompound1->getString("UUID"));
            GameProfile gameprofile = playerprofilecache.getProfileByUUID(uuid);
            if (gameprofile != nullptr) {
               playerReputation.insert(gameprofile.getName(), nbttagcompound1->getInteger("S"));
            }
         } else {
            playerReputation.insert(nbttagcompound1->getString("Name"), nbttagcompound1->getInteger("S"));
         }
      }
}

void Village::writeVillageDataToNBT(NBTTagCompound* compound)
{
   compound->setInteger("PopSize", numVillagers);
   compound->setInteger("Radius", villageRadius);
   compound->setInteger("Golems", numIronGolems);
   compound->setInteger("Stable", lastAddDoorTimestamp);
   compound->setInteger("Tick", tickCounter);
   compound->setInteger("MTick", noBreedTicks);
   compound->setInteger("CX", center.getx());
   compound->setInteger("CY", center.gety());
   compound->setInteger("CZ", center.getz());
   compound->setInteger("ACX", centerHelper.getx());
   compound->setInteger("ACY", centerHelper.gety());
   compound->setInteger("ACZ", centerHelper.getz());
   NBTTagList nbttaglist = new NBTTagList();
   Iterator var3 = villageDoorInfoList.iterator();

   while(var3.hasNext()) {
      VillageDoorInfo villagedoorinfo = (VillageDoorInfo)var3.next();
      NBTTagCompound nbttagcompound = new NBTTagCompound();
      nbttagcompound.setInteger("X", villagedoorinfo.getDoorBlockPos().getX());
      nbttagcompound.setInteger("Y", villagedoorinfo.getDoorBlockPos().getY());
      nbttagcompound.setInteger("Z", villagedoorinfo.getDoorBlockPos().getZ());
      nbttagcompound.setInteger("IDX", villagedoorinfo.getInsideOffsetX());
      nbttagcompound.setInteger("IDZ", villagedoorinfo.getInsideOffsetZ());
      nbttagcompound.setInteger("TS", villagedoorinfo.getLastActivityTimestamp());
      nbttaglist.appendTag(nbttagcompound);
   }

   compound->setTag("Doors", nbttaglist);
   NBTTagList nbttaglist1 = new NBTTagList();
   Iterator var11 = this.playerReputation.keySet().iterator();

   while(var11.hasNext()) {
      String s = (String)var11.next();
      NBTTagCompound nbttagcompound1 = new NBTTagCompound();
      PlayerProfileCache playerprofilecache = world->getMinecraftServer().getPlayerProfileCache();

      try {
         GameProfile gameprofile = playerprofilecache.getGameProfileForUsername(s);
         if (gameprofile != fmt::internal::null) {
            nbttagcompound1.setString("UUID", gameprofile.getId().toString());
            nbttagcompound1.setInteger("S", (Integer)playerReputation.get(s));
            nbttaglist1.appendTag(nbttagcompound1);
         }
      } catch (RuntimeException var9) {
      }
   }

   compound->setTag("Players", nbttaglist1);
}

void Village::endMatingSeason()
{
   noBreedTicks = tickCounter;
}

bool Village::isMatingSeason() const
{
   return noBreedTicks == 0 || tickCounter - noBreedTicks >= 3600;
}

void Village::setDefaultPlayerReputation(int32_t defaultReputation)
{
   for(auto s : playerReputation)
      {
         modifyPlayerReputation(s.first, defaultReputation);
      }
}

std::optional<Vec3d> Village::findRandomSpawnPos(BlockPos pos, int32_t x, int32_t y, int32_t z)
{
	for(auto i = 0; i < 10; ++i) {
         BlockPos blockpos = pos.add(world->rand(16) - 8, world->rand(6) - 3, world->rand(16) - 8);
         if (isBlockPosWithinSqVillageRadius(blockpos) && isAreaClearAround(BlockPos(x, y, z), blockpos)) {
            return Vec3d((double)blockpos.getx(), (double)blockpos.gety(), (double)blockpos.getz());
         }
      }

      return std::nullopt;
}

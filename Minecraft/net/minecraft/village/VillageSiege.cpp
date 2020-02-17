#include "VillageSiege.h"
#include "../world/WorldEntitySpawner.h"

VillageSiege::VillageSiege(World* worldIn)
	:world(worldIn)
{
}

void VillageSiege::tick()
{
   if (world->isDaytime()) 
   {
      siegeState = 0;
   }
   else if (siegeState != 2) 
   {
      if (siegeState == 0) 
      {
         float f = world->getCelestialAngle(0.0f);
         if ((double)f < 0.5 || (double)f > 0.501) 
         {
            return;
         }

         siegeState = world->rand(10) == 0 ? 1 : 2;
         hasSetupSiege = false;
         if (siegeState == 2) 
         {
            return;
         }
      }

      if (siegeState != -1) 
      {
         if (!hasSetupSiege) 
         {
            if (!trySetupSiege()) 
            {
               return;
            }

            hasSetupSiege = true;
         }

         if (nextSpawnTime > 0) 
         {
            --nextSpawnTime;
         }
         else 
         {
            nextSpawnTime = 2;
            if (siegeCount > 0) 
            {
               spawnZombie();
               --siegeCount;
            }
            else 
            {
               siegeState = 2;
            }
         }
      }
   }
}

bool VillageSiege::trySetupSiege()
{
   auto list = world->playerEntities;
   auto iterator = list.begin();
   std::optional<Vec3d> vec3d;

   do 
   {
      do 
      {
         do 
         {
            do 
            {
               do 
               {
                  EntityPlayer* entityplayer;
                  do 
                  {
                     if (iterator == list.end()) 
                     {
                        return false;
                     }

                     entityplayer = *iterator;
                     ++iterator;
                  }
                  while(entityplayer->isSpectator());

                  village = world->getVillageCollection().getNearestVillage(BlockPos(entityplayer), 1);
               }
               while(village == nullptr);
            }
            while(village.getNumVillageDoors() < 10);
         }
         while(village.getTicksSinceLastDoorAdding() < 20);
      }
      while(village.getNumVillagers() < 20);

      BlockPos blockpos = village.getCenter();
      float f = (float)village.getVillageRadius();
      bool flag = false;

      for(int i = 0; i < 10; ++i) {
         float f1 = MathHelper::nextFloat(world->rand) * 6.2831855F;
         spawnX = blockpos.getx() + (int)((double)(MathHelper::cos(f1) * f) * 0.9);
         spawnY = blockpos.gety();
         spawnZ = blockpos.getz() + (int)((double)(MathHelper::sin(f1) * f) * 0.9);
         flag = false;
         auto var9 = world->getVillageCollection().getVillageList();

         for(auto village1 : var9)
         {
            if (village1 != village && village1.isBlockPosWithinSqVillageRadius(BlockPos(spawnX, spawnY, spawnZ))) 
            {
               flag = true;
               break;
            }
         }

         if (!flag) 
         {
            break;
         }
      }

      if (flag) 
      {
         return false;
      }

      vec3d = findRandomSpawnPos(BlockPos(spawnX, spawnY, spawnZ));
   }
   while(!vec3d.has_value());

   nextSpawnTime = 0;
   siegeCount = 20;
   return true;
}

bool VillageSiege::spawnZombie()
{
   Vec3d vec3d = findRandomSpawnPos(BlockPos(spawnX, spawnY, spawnZ));
   if (vec3d == nullptr) 
   {
      return false;
   }
   else 
   {
      EntityZombie* entityzombie;
      try 
      {
         entityzombie = new EntityZombie(world);
         entityzombie->onInitialSpawn(world->getDifficultyForLocation(BlockPos(entityzombie)), (IEntityLivingData)nullptr);
      }
      catch (Exception var4) 
      {
         var4.printStackTrace();
         return false;
      }

      entityzombie->setLocationAndAngles(vec3d.getx(), vec3d.gety(), vec3d.getz(), MathHelper::nextFloat(world->rand) * 360.0F, 0.0F);
      world->spawnEntity(entityzombie);
      BlockPos blockpos = village.getCenter();
      entityzombie->setHomePosAndDistance(blockpos, village.getVillageRadius());
      return true;
   }
}

std::optional<Vec3d> VillageSiege::findRandomSpawnPos(BlockPos pos)
{
   for(int i = 0; i < 10; ++i) 
   {
      BlockPos blockpos = pos.add((int32_t)world->rand(16) - 8, world->rand(6) - 3, world->rand(16) - 8);
      if (village.isBlockPosWithinSqVillageRadius(blockpos) && WorldEntitySpawner::canCreatureTypeSpawnAtLocation(EntityLiving::SpawnPlacementType.ON_GROUND, world, blockpos)) 
      {
         return Vec3d((double)blockpos.getx(), (double)blockpos.gety(), (double)blockpos.getz());
      }
   }

   return std::nullopt;
}

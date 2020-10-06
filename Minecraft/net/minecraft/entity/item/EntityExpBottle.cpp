#include "EntityExpBottle.h"


#include "../../potion/PotionUtils.h"
#include "datafix/DataFixer.h"

EntityExpBottle::EntityExpBottle(World *worldIn)
    :EntityThrowable(worldIn){
}

EntityExpBottle::EntityExpBottle(World *worldIn, EntityLivingBase *throwerIn)
    :EntityThrowable(worldIn, throwerIn){
}

EntityExpBottle::EntityExpBottle(World *worldIn, double x, double y, double z)
    :EntityThrowable(worldIn, x, y, z){
}

void EntityExpBottle::registerFixesExpBottle(DataFixer fixer) {
    EntityThrowable::registerFixesThrowable(fixer, "ThrowableExpBottle");
}

float EntityExpBottle::getGravityVelocity() {
    return 0.07F;
}

void EntityExpBottle::onImpact(RayTraceResult result) {
    if (!world->isRemote) {
         world->playEvent(2002, BlockPos(this), PotionUtils::getPotionColor(PotionTypes::WATER));
         auto i = 3 + world->rand(5) + world->rand(5);

         while(i > 0) {
            auto j = EntityXPOrb::getXPSplit(i);
            i -= j;
            world->spawnEntity(new EntityXPOrb(world, posX, posY, posZ, j));
         }

         setDead();
      }
}

#include "EntityMinecartMobSpawner.h"

#include "../../tileentity/TileEntityMobSpawner.h"
#include "../EntityList.h"
#include "datafix/DataFixer.h"
#include "datafix/IDataWalker.h"

class EntityMinecartMobSpawnerWalker : public IDataWalker
{
  public:
    NBTTagCompound *process(IDataFixer *fixer, NBTTagCompound *compound, int32_t versionIn) override
    {
        std::string s = compound->getString("id");
        if (EntityList::getKey(EntityMinecartMobSpawner.class).equals(new ResourceLocation(s)))
        {
            compound->setString("id", TileEntity::getKey(TileEntityMobSpawner.class).toString());
            fixer.process(FixTypes::BLOCK_ENTITY, compound, versionIn);
            compound->setString("id", s);
        }

        return compound;
    }
} walker;

EntityMinecartMobSpawner::EntityMinecartMobSpawner(World *worldIn) : EntityMinecart(worldIn), mobSpawnerLogic(this)
{
}

EntityMinecartMobSpawner::EntityMinecartMobSpawner(World *worldIn, double x, double y, double z)
    : EntityMinecart(worldIn, x, y, z), mobSpawnerLogic(this)
{
}

void EntityMinecartMobSpawner::registerFixesMinecartMobSpawner(DataFixer fixer)
{
    registerFixesMinecart(fixer, EntityMinecartMobSpawner.class);
    fixer.registerWalker(FixTypes::ENTITY, &walker);
}

EntityMinecart::Type EntityMinecartMobSpawner::getType()
{
    return EntityMinecart::Type::SPAWNER;
}

IBlockState *EntityMinecartMobSpawner::getDefaultDisplayTile()
{
    return Blocks::MOB_SPAWNER->getDefaultState();
}

void EntityMinecartMobSpawner::handleStatusUpdate(std::byte id)
{
    mobSpawnerLogic.setDelayToMin(id);
}

void EntityMinecartMobSpawner::onUpdate()
{
    EntityMinecart::onUpdate();
    mobSpawnerLogic.updateSpawner();
}

void EntityMinecartMobSpawner::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityMinecart::readEntityFromNBT(compound);
    mobSpawnerLogic.readFromNBT(compound);
}

void EntityMinecartMobSpawner::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityMinecart::writeEntityToNBT(compound);
    mobSpawnerLogic.writeToNBT(compound);
}

EntityMinecartMobSpawner::MobSpawnerMinecartLogic::MobSpawnerMinecartLogic(EntityMinecartMobSpawner *entityIn)
    : entity(entityIn)
{
}

void EntityMinecartMobSpawner::MobSpawnerMinecartLogic::broadcastEvent(std::byte id)
{
    entity->world->setEntityState(entity, id);
}

World *EntityMinecartMobSpawner::MobSpawnerMinecartLogic::getSpawnerWorld() const
{
    return entity->world;
}

BlockPos EntityMinecartMobSpawner::MobSpawnerMinecartLogic::getSpawnerPosition() const
{
    return BlockPos(entity);
}

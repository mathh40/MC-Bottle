#include "TileEntityMobSpawner.h"

#include "datafix/FixTypes.h"
#include "datafix/IDataWalker.h"

class IDataFixer;

TileEntityMobSpawner::TileEntityMobSpawner()
{
}

void TileEntityMobSpawner::registerFixesMobSpawner(DataFixer fixer)
{
    fixer.registerWalker(FixTypes.BLOCK_ENTITY, new IDataWalker() {
		public NBTTagCompound process(IDataFixer fixer, NBTTagCompound compound, int versionIn) {
			if (TileEntity.getKey(TileEntityMobSpawner.class).equals(new ResourceLocation(compound.getString("id")))) {
				if (compound.hasKey("SpawnPotentials", 9)) {
					NBTTagList nbttaglist = compound.getTagList("SpawnPotentials", 10);

					for(int i = 0; i < nbttaglist.tagCount(); ++i) {
        NBTTagCompound nbttagcompound = nbttaglist.getCompoundTagAt(i);
        nbttagcompound.setTag("Entity",
                              fixer.process(FixTypes::ENTITY, nbttagcompound.getCompoundTag("Entity"), versionIn));
					}
}

compound.setTag("SpawnData", fixer.process(FixTypes::ENTITY, compound.getCompoundTag("SpawnData"), versionIn));
}

return compound;
}
});
}

void TileEntityMobSpawner::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    spawnerLogic->readFromNBT(compound);
}

NBTTagCompound *TileEntityMobSpawner::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    spawnerLogic->writeToNBT(compound);
    return compound;
}

void TileEntityMobSpawner::update()
{
    spawnerLogic->updateSpawner();
}

std::optional<SPacketUpdateTileEntity> TileEntityMobSpawner::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 1, getUpdateTag());
}

NBTTagCompound *TileEntityMobSpawner::getUpdateTag()
{
    NBTTagCompound *nbttagcompound = writeToNBT(new NBTTagCompound());
    nbttagcompound->removeTag("SpawnPotentials");
    return nbttagcompound;
}

bool TileEntityMobSpawner::receiveClientEvent(int32_t id, int32_t type)
{
    return spawnerLogic->setDelayToMin(id) ? true : TileEntity::receiveClientEvent(id, type);
}

bool TileEntityMobSpawner::onlyOpsCanSetNbt()
{
    return true;
}

MobSpawnerBaseLogic *TileEntityMobSpawner::getSpawnerBaseLogic() const
{
    return spawnerLogic.get();
}

TileEntityMobSpawner::MobSpawnerLogic::MobSpawnerLogic(TileEntityMobSpawner *entity) : entity(entity)
{
}

void TileEntityMobSpawner::MobSpawnerLogic::broadcastEvent(int32_t id)
{
    entity->world->addBlockEvent(entity->pos, Blocks::MOB_SPAWNER, id, 0);
}

World *TileEntityMobSpawner::MobSpawnerLogic::getSpawnerWorld()
{
    return entity->world;
}

BlockPos TileEntityMobSpawner::MobSpawnerLogic::getSpawnerPosition()
{
    return entity->pos;
}

void TileEntityMobSpawner::MobSpawnerLogic::setNextSpawnData(WeightedSpawnerEntity p_184993_1_)
{
    MobSpawnerBaseLogic::setNextSpawnData(p_184993_1_);
    if (getSpawnerWorld() != nullptr)
    {
        IBlockState *iblockstate = getSpawnerWorld()->getBlockState(getSpawnerPosition());
        getSpawnerWorld()->notifyBlockUpdate(entity->pos, iblockstate, iblockstate, 4);
    }
}
}

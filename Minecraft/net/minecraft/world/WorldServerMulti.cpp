#include "WorldServerMulti.h"
WorldServerMulti::WorldServerMulti(MinecraftServer* server, ISaveHandler saveHandlerIn, int32_t dimensionId, WorldServer* delegate, Profiler profilerIn)
	:WorldServer(server, saveHandlerIn, DerivedWorldInfo(delegate.getWorldInfo()), dimensionId, profilerIn)
{
	delegate = delegate;
	delegate.getWorldBorder().addListener(new IBorderListener(){
		public void onSizeChanged(WorldBorder border, double newSize) {
		   WorldServerMulti.this.getWorldBorder().setTransition(newSize);
		}

		public void onTransitionStarted(WorldBorder border, double oldSize, double newSize, long time) {
		   WorldServerMulti.this.getWorldBorder().setTransition(oldSize, newSize, time);
		}

		public void onCenterChanged(WorldBorder border, double x, double z) {
		   WorldServerMulti.this.getWorldBorder().setCenter(x, z);
		}

		public void onWarningTimeChanged(WorldBorder border, int newTime) {
		   WorldServerMulti.this.getWorldBorder().setWarningTime(newTime);
		}

		public void onWarningDistanceChanged(WorldBorder border, int newDistance) {
		   WorldServerMulti.this.getWorldBorder().setWarningDistance(newDistance);
		}

		public void onDamageAmountChanged(WorldBorder border, double newAmount) {
		   WorldServerMulti.this.getWorldBorder().setDamageAmount(newAmount);
		}

		public void onDamageBufferChanged(WorldBorder border, double newSize) {
		   WorldServerMulti.this.getWorldBorder().setDamageBuffer(newSize);
		}
		});
}

World* WorldServerMulti::init()
{
	mapStorage = delegate.getMapStorage();
	worldScoreboard = delegate.getScoreboard();
	lootTable = delegate.getLootTableManager();
	advancementManager = delegate.getAdvancementManager();
	auto s = VillageCollection.fileNameForProvider(provider);
	auto villagecollection = (VillageCollection)this.mapStorage.getOrLoadData(VillageCollection.class, s);
	if (villagecollection == nullptr) 
	{
		villageCollection = VillageCollection(this);
		mapStorage.setData(s, villageCollection);
	}
	else 
{
		villageCollection = villagecollection;
		villageCollection.setWorldsForAll(this);
	}

	return this;
}

void WorldServerMulti::saveAdditionalData()
{
	provider.onWorldSave();
}

void WorldServerMulti::saveLevel()
{
}

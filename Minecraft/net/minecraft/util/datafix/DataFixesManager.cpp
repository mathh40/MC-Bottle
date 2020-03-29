#include "DataFixesManager.h"
#include "DataFixer.h"
#include "NBTTagCompound.h"
#include "FixTypes.h"

namespace DataFixesManager
{
	void registerFixes(DataFixer &fixer) {
		fixer.registerFix(FixTypes::ENTITY, new EntityArmorAndHeld());
		fixer.registerFix(FixTypes::BLOCK_ENTITY, new SignStrictJSON());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new ItemIntIDToString());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new PotionItems());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new SpawnEggNames());
		fixer.registerFix(FixTypes::ENTITY, new MinecartEntityTypes());
		fixer.registerFix(FixTypes::BLOCK_ENTITY, new SpawnerEntityTypes());
		fixer.registerFix(FixTypes::ENTITY, new StringToUUID());
		fixer.registerFix(FixTypes::ENTITY, new EntityHealth());
		fixer.registerFix(FixTypes::ENTITY, new HorseSaddle());
		fixer.registerFix(FixTypes::ENTITY, new PaintingDirection());
		fixer.registerFix(FixTypes::ENTITY, new RedundantChanceTags());
		fixer.registerFix(FixTypes::ENTITY, new RidingToPassengers());
		fixer.registerFix(FixTypes::ENTITY, new ArmorStandSilent());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new BookPagesStrictJSON());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new CookedFishIDTypo());
		fixer.registerFix(FixTypes::ENTITY, new ZombieProfToType());
		fixer.registerFix(FixTypes::OPTIONS, new ForceVBOOn());
		fixer.registerFix(FixTypes::ENTITY, new ElderGuardianSplit());
		fixer.registerFix(FixTypes::ENTITY, new SkeletonSplit());
		fixer.registerFix(FixTypes::ENTITY, new ZombieSplit());
		fixer.registerFix(FixTypes::ENTITY, new HorseSplit());
		fixer.registerFix(FixTypes::BLOCK_ENTITY, new TileEntityId());
		fixer.registerFix(FixTypes::ENTITY, new EntityId());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new BannerItemColor());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new PotionWater());
		fixer.registerFix(FixTypes::ENTITY, new ShulkerBoxEntityColor());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new ShulkerBoxItemColor());
		fixer.registerFix(FixTypes::BLOCK_ENTITY, new ShulkerBoxTileColor());
		fixer.registerFix(FixTypes::OPTIONS, new OptionsLowerCaseLanguage());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new TotemItemRename());
		fixer.registerFix(FixTypes::CHUNK, new AddBedTileEntity());
		fixer.registerFix(FixTypes::ITEM_INSTANCE, new BedItemColor());
	}

	DataFixer createFixer()
	{
		DataFixer datafixer = DataFixer(1343);
		WorldInfo.registerFixes(datafixer);
		EntityPlayerMP.registerFixesPlayerMP(datafixer);
		EntityPlayer.registerFixesPlayer(datafixer);
		AnvilChunkLoader.registerFixes(datafixer);
		ItemStack.registerFixes(datafixer);
		Template.registerFixes(datafixer);
		entity->registerFixes(datafixer);
		EntityArmorStand.registerFixesArmorStand(datafixer);
		EntityArrow.registerFixesArrow(datafixer);
		EntityBat.registerFixesBat(datafixer);
		EntityBlaze.registerFixesBlaze(datafixer);
		EntityCaveSpider.registerFixesCaveSpider(datafixer);
		EntityChicken.registerFixesChicken(datafixer);
		EntityCow.registerFixesCow(datafixer);
		EntityCreeper.registerFixesCreeper(datafixer);
		EntityDonkey.registerFixesDonkey(datafixer);
		EntityDragonFireball.registerFixesDragonFireball(datafixer);
		EntityElderGuardian.registerFixesElderGuardian(datafixer);
		EntityDragon.registerFixesDragon(datafixer);
		EntityEnderman.registerFixesEnderman(datafixer);
		EntityEndermite.registerFixesEndermite(datafixer);
		EntityEvoker.registerFixesEvoker(datafixer);
		EntityFallingBlock.registerFixesFallingBlock(datafixer);
		EntityFireworkRocket.registerFixesFireworkRocket(datafixer);
		EntityGhast.registerFixesGhast(datafixer);
		EntityGiantZombie.registerFixesGiantZombie(datafixer);
		EntityGuardian.registerFixesGuardian(datafixer);
		EntityHorse.registerFixesHorse(datafixer);
		EntityHusk.registerFixesHusk(datafixer);
		EntityItem.registerFixesItem(datafixer);
		EntityItemFrame.registerFixesItemFrame(datafixer);
		EntityLargeFireball.registerFixesLargeFireball(datafixer);
		EntityMagmaCube.registerFixesMagmaCube(datafixer);
		EntityMinecartChest.registerFixesMinecartChest(datafixer);
		EntityMinecartCommandBlock.registerFixesMinecartCommand(datafixer);
		EntityMinecartFurnace.registerFixesMinecartFurnace(datafixer);
		EntityMinecartHopper.registerFixesMinecartHopper(datafixer);
		EntityMinecartEmpty.registerFixesMinecartEmpty(datafixer);
		EntityMinecartMobSpawner.registerFixesMinecartMobSpawner(datafixer);
		EntityMinecartTNT.registerFixesMinecartTNT(datafixer);
		EntityMule.registerFixesMule(datafixer);
		EntityMooshroom.registerFixesMooshroom(datafixer);
		EntityOcelot.registerFixesOcelot(datafixer);
		EntityPig.registerFixesPig(datafixer);
		EntityPigZombie.registerFixesPigZombie(datafixer);
		EntityRabbit.registerFixesRabbit(datafixer);
		EntitySheep.registerFixesSheep(datafixer);
		EntityShulker.registerFixesShulker(datafixer);
		EntitySilverfish.registerFixesSilverfish(datafixer);
		EntitySkeleton.registerFixesSkeleton(datafixer);
		EntitySkeletonHorse.registerFixesSkeletonHorse(datafixer);
		EntitySlime.registerFixesSlime(datafixer);
		EntitySmallFireball.registerFixesSmallFireball(datafixer);
		EntitySnowman.registerFixesSnowman(datafixer);
		EntitySnowball.registerFixesSnowball(datafixer);
		EntitySpectralArrow.registerFixesSpectralArrow(datafixer);
		EntitySpider.registerFixesSpider(datafixer);
		EntitySquid.registerFixesSquid(datafixer);
		EntityStray.registerFixesStray(datafixer);
		EntityEgg.registerFixesEgg(datafixer);
		EntityEnderPearl.registerFixesEnderPearl(datafixer);
		EntityExpBottle.registerFixesExpBottle(datafixer);
		EntityPotion.registerFixesPotion(datafixer);
		EntityTippedArrow.registerFixesTippedArrow(datafixer);
		EntityVex.registerFixesVex(datafixer);
		EntityVillager.registerFixesVillager(datafixer);
		EntityIronGolem.registerFixesIronGolem(datafixer);
		EntityVindicator.registerFixesVindicator(datafixer);
		EntityWitch.registerFixesWitch(datafixer);
		EntityWither.registerFixesWither(datafixer);
		EntityWitherSkeleton.registerFixesWitherSkeleton(datafixer);
		EntityWitherSkull.registerFixesWitherSkull(datafixer);
		EntityWolf.registerFixesWolf(datafixer);
		EntityZombie.registerFixesZombie(datafixer);
		EntityZombieHorse.registerFixesZombieHorse(datafixer);
		EntityZombieVillager.registerFixesZombieVillager(datafixer);
		TileEntityPiston.registerFixesPiston(datafixer);
		TileEntityFlowerPot.registerFixesFlowerPot(datafixer);
		TileEntityFurnace.registerFixesFurnace(datafixer);
		TileEntityChest.registerFixesChest(datafixer);
		TileEntityDispenser.registerFixes(datafixer);
		TileEntityDropper.registerFixesDropper(datafixer);
		TileEntityBrewingStand.registerFixesBrewingStand(datafixer);
		TileEntityHopper.registerFixesHopper(datafixer);
		BlockJukebox.registerFixesJukebox(datafixer);
		TileEntityMobSpawner.registerFixesMobSpawner(datafixer);
		TileEntityShulkerBox.registerFixesShulkerBox(datafixer);
		registerFixes(datafixer);
		return datafixer;
	}

	std::shared_ptr<NBTTagCompound> processItemStack(IDataFixer* fixer, std::shared_ptr<NBTTagCompound> compound, int32_t version, std::string key)
	{
		if (compound->hasKey(key, 10)) {
			compound->setTag(key, fixer->process(FixTypes::ITEM_INSTANCE, compound->getCompoundTag(key), version));
		}

		return compound;
	}

	std::shared_ptr<NBTTagCompound> processInventory(IDataFixer* fixer, std::shared_ptr<NBTTagCompound> compound, int32_t version, std::string key)
	{
		if (compound->hasKey(key, 9)) {
			auto nbttaglist = compound->getTagList(key, 10);

			for (int i = 0; i < nbttaglist->tagCount(); ++i) {
				nbttaglist->set(i, fixer->process(FixTypes::ITEM_INSTANCE, nbttaglist->getCompoundTagAt(i), version));
			}
		}

		return compound;
	}
}

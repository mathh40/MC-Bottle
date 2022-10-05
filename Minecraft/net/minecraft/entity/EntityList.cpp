#include "EntityList.h"

#include "../item/ItemArmorStand.h"
#include "Entity.h"
#include "EntityAreaEffectCloud.h"
#include "EntityLeashKnot.h"
#include "ai/EntityAIBeg.h"
#include "ai/EntityAICreeperSwell.h"
#include "ai/EntityAIFollowGolem.h"
#include "ai/EntityAILlamaFollowCaravan.h"
#include "ai/EntityAIOcelotSit.h"
#include "ai/EntityAISkeletonRiders.h"
#include "ai/EntityAIZombieAttack.h"

#include <typeindex>
std::shared_ptr<spdlog::logger> EntityList::LOGGER = spdlog::get("Minecraft")->clone("EntityList");

ResourceLocation EntityList::getKey(Entity *entityIn)
{
    return getKey(entityIn->getClass());
}

ResourceLocation EntityList::getKey(std::type_index entityIn)
{
    REGISTRY.getNameForObject(entityIn);
}

std::string EntityList::getEntityString(Entity *entityIn)
{
    auto i = REGISTRY.getIDForObject(entityIn->getClass());
    return i == -1 ? "" : OLD_NAMES[i];
}

std::string EntityList::getTranslationName(std::optional<ResourceLocation> entityType)
{
    auto i = REGISTRY.getIDForObject(REGISTRY.getObject(entityType).value());
    return i == -1 ? "" : OLD_NAMES[i];
}

std::optional<std::type_index> EntityList::getClassFromID(int32_t entityID)
{
    return REGISTRY.getObjectById(entityID);
}

std::optional<std::type_index> EntityList::getClassFromName(std::string_view p_192839_0_)
{
    return REGISTRY.getObject(ResourceLocation(p_192839_0_));
}

Entity *EntityList::newEntity(std::optional<std::type_index> clazz, World *worldIn)
{
    if (!clazz)
    {
        return nullptr;
    }
    else
    {
        try
        {
            return conname[clazz.value().name()](worldIn);
        }
        catch (std::exception &var3)
        {
            return nullptr;
        }
    }
}

Entity *EntityList::createEntityByID(int32_t entityID, World *worldIn)
{
    return newEntity(getClassFromID(entityID), worldIn);
}

Entity *EntityList::createEntityByIDFromName(const ResourceLocation &name, World *worldIn)
{
    return newEntity(REGISTRY.getObject(name), worldIn);
}

Entity *EntityList::createEntityFromNBT(NBTTagCompound *nbt, World *worldIn)
{
    const ResourceLocation resourcelocation = ResourceLocation(nbt->getString("id"));
    Entity *entity                          = createEntityByIDFromName(resourcelocation, worldIn);
    if (entity == nullptr)
    {
        LOGGER->warn("Skipping Entity with id {}", resourcelocation);
    }
    else
    {
        entity->readFromNBT(nbt);
    }

    return entity;
}

std::unordered_set<ResourceLocation> EntityList::getEntityNameList()
{
    return KNOWN_TYPES;
}

bool EntityList::isMatchingName(Entity *entityIn, const ResourceLocation &entityName)
{
    ResourceLocation resourcelocation = getKey(entityIn->getClass());
    if (resourcelocation != nullptr)
    {
        return resourcelocation == entityName;
    }
    else if (Util:: instanceof <EntityPlayer>(entityIn))
    {
        return PLAYER == entityName;
    }
    else
    {
        return Util:: instanceof <EntityLightningBolt>(entityIn) ? LIGHTNING_BOLT == entityName : false;
    }
}

bool EntityList::isRegistered(const ResourceLocation &entityName)
{
    return PLAYER == entityName || getEntityNameList().contains(entityName);
}

std::string EntityList::getValidTypeNames()
{
    std::string stringbuilder;

    for (auto resourcelocation : getEntityNameList())
    {

        stringbuilder.append(resourcelocation.to_string()).append(", ");
    }

    stringbuilder.append(PLAYER.to_string());
    return stringbuilder;
}

void EntityList::init()
{
    registers<EntityItem>(1, "item", "Item");
    registers(2, "xp_orb", typeid(EntityXPOrb), "XPOrb");
    registers(3, "area_effect_cloud", typeid(EntityAreaEffectCloud), "AreaEffectCloud");
    registers(4, "elder_guardian", typeid(EntityElderGuardian), "ElderGuardian");
    registers(5, "wither_skeleton", typeid(EntityWitherSkeleton), "WitherSkeleton");
    registers(6, "stray", typeid(EntityStray), "Stray");
    registers(7, "egg", typeid(EntityEgg), "ThrownEgg");
    registers(8, "leash_knot", typeid(EntityLeashKnot), "LeashKnot");
    registers(9, "painting", typeid(EntityPainting), "Painting");
    registers(10, "arrow", typeid(EntityTippedArrow), "Arrow");
    registers(11, "snowball", typeid(EntitySnowball), "Snowball");
    registers(12, "fireball", typeid(EntityLargeFireball), "Fireball");
    registers(13, "small_fireball", typeid(EntitySmallFireball), "SmallFireball");
    registers(14, "ender_pearl", typeid(EntityEnderPearl), "ThrownEnderpearl");
    registers(15, "eye_of_ender_signal", typeid(EntityEnderEye), "EyeOfEnderSignal");
    registers(16, "potion", typeid(EntityPotion), "ThrownPotion");
    registers(17, "xp_bottle", typeid(EntityExpBottle), "ThrownExpBottle");
    registers(18, "item_frame", typeid(EntityItemFrame), "ItemFrame");
    registers(19, "wither_skull", typeid(EntityWitherSkull), "WitherSkull");
    registers(20, "tnt", typeid(EntityTNTPrimed), "PrimedTnt");
    registers(21, "falling_block", typeid(EntityFallingBlock), "FallingSand");
    registers(22, "fireworks_rocket", typeid(EntityFireworkRocket), "FireworksRocketEntity");
    registers(23, "husk", typeid(EntityHusk), "Husk");
    registers(24, "spectral_arrow", typeid(EntitySpectralArrow), "SpectralArrow");
    registers(25, "shulker_bullet", typeid(EntityShulkerBullet), "ShulkerBullet");
    registers(26, "dragon_fireball", typeid(EntityDragonFireball), "DragonFireball");
    registers(27, "zombie_villager", typeid(EntityZombieVillager), "ZombieVillager");
    registers(28, "skeleton_horse", typeid(EntitySkeletonHorse), "SkeletonHorse");
    registers(29, "zombie_horse", typeid(EntityZombieHorse), "ZombieHorse");
    registers(30, "armor_stand", typeid(EntityArmorStand), "ArmorStand");
    registers(31, "donkey", typeid(EntityDonkey), "Donkey");
    registers(32, "mule", typeid(EntityMule), "Mule");
    registers(33, "evocation_fangs", typeid(EntityEvokerFangs), "EvocationFangs");
    registers(34, "evocation_illager", typeid(EntityEvoker), "EvocationIllager");
    registers(35, "vex", typeid(EntityVex), "Vex");
    registers(36, "vindication_illager", typeid(EntityVindicator), "VindicationIllager");
    registers(37, "illusion_illager", typeid(EntityIllusionIllager), "IllusionIllager");
    registers(40, "commandblock_minecart", typeid(EntityMinecartCommandBlock),
              EntityMinecart.Type.COMMAND_BLOCK.getName());
    registers(41, "boat", typeid(EntityBoat), "Boat");
    registers(42, "minecart", typeid(EntityMinecartEmpty), EntityMinecart.Type.RIDEABLE.getName());
    registers(43, "chest_minecart", typeid(EntityMinecartChest), EntityMinecart.Type.CHEST.getName());
    registers(44, "furnace_minecart", typeid(EntityMinecartFurnace), EntityMinecart.Type.FURNACE.getName());
    registers(45, "tnt_minecart", typeid(EntityMinecartTNT), EntityMinecart.Type.TNT.getName());
    registers(46, "hopper_minecart", typeid(EntityMinecartHopper), EntityMinecart.Type.HOPPER.getName());
    registers(47, "spawner_minecart", typeid(EntityMinecartMobSpawner), EntityMinecart.Type.SPAWNER.getName());
    registers(50, "creeper", typeid(EntityCreeper), "Creeper");
    registers(51, "skeleton", typeid(EntitySkeleton), "Skeleton");
    registers(52, "spider", typeid(EntitySpider), "Spider");
    registers(53, "giant", typeid(EntityGiantZombie), "Giant");
    registers(54, "zombie", typeid(EntityZombie), "Zombie");
    registers(55, "slime", typeid(EntitySlime), "Slime");
    registers(56, "ghast", typeid(EntityGhast), "Ghast");
    registers(57, "zombie_pigman", typeid(EntityPigZombie), "PigZombie");
    registers(58, "enderman", typeid(EntityEnderman), "Enderman");
    registers(59, "cave_spider", typeid(EntityCaveSpider), "CaveSpider");
    registers(60, "silverfish", typeid(EntitySilverfish), "Silverfish");
    registers(61, "blaze", typeid(EntityBlaze), "Blaze");
    registers(62, "magma_cube", typeid(EntityMagmaCube), "LavaSlime");
    registers(63, "ender_dragon", typeid(EntityDragon), "EnderDragon");
    registers(64, "wither", typeid(EntityWither), "WitherBoss");
    registers(65, "bat", typeid(EntityBat), "Bat");
    registers(66, "witch", typeid(EntityWitch), "Witch");
    registers(67, "endermite", typeid(EntityEndermite), "Endermite");
    registers(68, "guardian", typeid(EntityGuardian), "Guardian");
    registers(69, "shulker", typeid(EntityShulker), "Shulker");
    registers(90, "pig", typeid(EntityPig), "Pig");
    registers(91, "sheep", typeid(EntitySheep), "Sheep");
    registers(92, "cow", typeid(EntityCow), "Cow");
    registers(93, "chicken", typeid(EntityChicken), "Chicken");
    registers(94, "squid", typeid(EntitySquid), "Squid");
    registers(95, "wolf", typeid(EntityWolf), "Wolf");
    registers(96, "mooshroom", typeid(EntityMooshroom), "MushroomCow");
    registers(97, "snowman", typeid(EntitySnowman), "SnowMan");
    registers(98, "ocelot", typeid(EntityOcelot), "Ozelot");
    registers(99, "villager_golem", typeid(EntityIronGolem), "VillagerGolem");
    registers(100, "horse", typeid(EntityHorse), "Horse");
    registers(101, "rabbit", typeid(EntityRabbit), "Rabbit");
    registers(102, "polar_bear", typeid(EntityPolarBear), "PolarBear");
    registers(103, "llama", typeid(EntityLlama), "Llama");
    registers(104, "llama_spit", typeid(EntityLlamaSpit), "LlamaSpit");
    registers(105, "parrot", typeid(EntityParrot), "Parrot");
    registers(120, "villager", typeid(EntityVillager), "Villager");
    registers(200, "ender_crystal", typeid(EntityEnderCrystal), "EnderCrystal");
    addSpawnInfo("bat", 4996656, 986895);
    addSpawnInfo("blaze", 16167425, 16775294);
    addSpawnInfo("cave_spider", 803406, 11013646);
    addSpawnInfo("chicken", 10592673, 16711680);
    addSpawnInfo("cow", 4470310, 10592673);
    addSpawnInfo("creeper", 894731, 0);
    addSpawnInfo("donkey", 5457209, 8811878);
    addSpawnInfo("elder_guardian", 13552826, 7632531);
    addSpawnInfo("enderman", 1447446, 0);
    addSpawnInfo("endermite", 1447446, 7237230);
    addSpawnInfo("evocation_illager", 9804699, 1973274);
    addSpawnInfo("ghast", 16382457, 12369084);
    addSpawnInfo("guardian", 5931634, 15826224);
    addSpawnInfo("horse", 12623485, 15656192);
    addSpawnInfo("husk", 7958625, 15125652);
    addSpawnInfo("llama", 12623485, 10051392);
    addSpawnInfo("magma_cube", 3407872, 16579584);
    addSpawnInfo("mooshroom", 10489616, 12040119);
    addSpawnInfo("mule", 1769984, 5321501);
    addSpawnInfo("ocelot", 15720061, 5653556);
    addSpawnInfo("parrot", 894731, 16711680);
    addSpawnInfo("pig", 15771042, 14377823);
    addSpawnInfo("polar_bear", 15921906, 9803152);
    addSpawnInfo("rabbit", 10051392, 7555121);
    addSpawnInfo("sheep", 15198183, 16758197);
    addSpawnInfo("shulker", 9725844, 5060690);
    addSpawnInfo("silverfish", 7237230, 3158064);
    addSpawnInfo("skeleton", 12698049, 4802889);
    addSpawnInfo("skeleton_horse", 6842447, 15066584);
    addSpawnInfo("slime", 5349438, 8306542);
    addSpawnInfo("spider", 3419431, 11013646);
    addSpawnInfo("squid", 2243405, 7375001);
    addSpawnInfo("stray", 6387319, 14543594);
    addSpawnInfo("vex", 8032420, 15265265);
    addSpawnInfo("villager", 5651507, 12422002);
    addSpawnInfo("vindication_illager", 9804699, 2580065);
    addSpawnInfo("witch", 3407872, 5349438);
    addSpawnInfo("wither_skeleton", 1315860, 4672845);
    addSpawnInfo("wolf", 14144467, 13545366);
    addSpawnInfo("zombie", 44975, 7969893);
    addSpawnInfo("zombie_horse", 3232308, 9945732);
    addSpawnInfo("zombie_pigman", 15373203, 5009705);
    addSpawnInfo("zombie_villager", 5651507, 7969893);
    KNOWN_TYPES.emplace(LIGHTNING_BOLT);
}

EntityList::EntityEggInfo EntityList::addSpawnInfo(std::string_view id, int32_t primaryColor, int32_t secondaryColor)
{
    ResourceLocation resourcelocation(id);
    auto info = EntityEggInfo(resourcelocation, primaryColor, secondaryColor);
    ENTITY_EGGS.emplace(resourcelocation, info);
    return info;
}

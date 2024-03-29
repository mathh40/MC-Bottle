#include "CriteriaTriggers.h"

ImpossibleTrigger CriteriaTriggers::IMPOSSIBLE = (ImpossibleTrigger)regist(new ImpossibleTrigger());
KilledTrigger CriteriaTriggers::PLAYER_KILLED_ENTITY =
    (KilledTrigger)regist(new KilledTrigger(new ResourceLocation("player_killed_entity")));
KilledTrigger CriteriaTriggers::ENTITY_KILLED_PLAYER =
    (KilledTrigger)regist(new KilledTrigger(new ResourceLocation("entity_killed_player")));
EnterBlockTrigger CriteriaTriggers::ENTER_BLOCK = (EnterBlockTrigger)regist(new EnterBlockTrigger());
InventoryChangeTrigger CriteriaTriggers::INVENTORY_CHANGED =
    (InventoryChangeTrigger)regist(new InventoryChangeTrigger());
RecipeUnlockedTrigger CriteriaTriggers::RECIPE_UNLOCKED = (RecipeUnlockedTrigger)regist(new RecipeUnlockedTrigger());
PlayerHurtEntityTrigger CriteriaTriggers::PLAYER_HURT_ENTITY =
    (PlayerHurtEntityTrigger)regist(new PlayerHurtEntityTrigger());
EntityHurtPlayerTrigger CriteriaTriggers::            = (EntityHurtPlayerTrigger)regist(new EntityHurtPlayerTrigger());
EnchantedItemTrigger CriteriaTriggers::ENCHANTED_ITEM = (EnchantedItemTrigger)regist(new EnchantedItemTrigger());
BrewedPotionTrigger CriteriaTriggers::BREWED_POTION   = (BrewedPotionTrigger)regist(new BrewedPotionTrigger());
ConstructBeaconTrigger CriteriaTriggers::CONSTRUCT_BEACON =
    (ConstructBeaconTrigger)regist(new ConstructBeaconTrigger());
UsedEnderEyeTrigger CriteriaTriggers::USED_ENDER_EYE    = (UsedEnderEyeTrigger)regist(new UsedEnderEyeTrigger());
SummonedEntityTrigger CriteriaTriggers::SUMMONED_ENTITY = (SummonedEntityTrigger)regist(new SummonedEntityTrigger());
BredAnimalsTrigger CriteriaTriggers::BRED_ANIMALS       = (BredAnimalsTrigger)regist(new BredAnimalsTrigger());
PositionTrigger CriteriaTriggers::LOCATION =
    (PositionTrigger)regist(new PositionTrigger(new ResourceLocation("location")));
PositionTrigger CriteriaTriggers::SLEPT_IN_BED =
    (PositionTrigger)regist(new PositionTrigger(new ResourceLocation("slept_in_bed")));
CuredZombieVillagerTrigger CriteriaTriggers::CURED_ZOMBIE_VILLAGER =
    (CuredZombieVillagerTrigger)regist(new CuredZombieVillagerTrigger());
VillagerTradeTrigger CriteriaTriggers::VILLAGER_TRADE = (VillagerTradeTrigger)regist(new VillagerTradeTrigger());
ItemDurabilityTrigger CriteriaTriggers::ITEM_DURABILITY_CHANGED =
    (ItemDurabilityTrigger)regist(new ItemDurabilityTrigger());
LevitationTrigger CriteriaTriggers::LEVITATION = (LevitationTrigger)regist(new LevitationTrigger());
ChangeDimensionTrigger CriteriaTriggers::CHANGED_DIMENSION =
    (ChangeDimensionTrigger)regist(new ChangeDimensionTrigger());
TickTrigger CriteriaTriggers::TICK                      = (TickTrigger)regist(new TickTrigger());
TameAnimalTrigger CriteriaTriggers::TAME_ANIMAL         = (TameAnimalTrigger)regist(new TameAnimalTrigger());
PlacedBlockTrigger CriteriaTriggers::PLACED_BLOCK       = (PlacedBlockTrigger)regist(new PlacedBlockTrigger());
ConsumeItemTrigger CriteriaTriggers::CONSUME_ITEM       = (ConsumeItemTrigger)regist(new ConsumeItemTrigger());
EffectsChangedTrigger CriteriaTriggers::EFFECTS_CHANGED = (EffectsChangedTrigger)regist(new EffectsChangedTrigger());
UsedTotemTrigger CriteriaTriggers::USED_TOTEM           = (UsedTotemTrigger)regist(new UsedTotemTrigger());
NetherTravelTrigger CriteriaTriggers::NETHER_TRAVEL     = (NetherTravelTrigger)regist(new NetherTravelTrigger());

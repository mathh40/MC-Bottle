#pragma once
#include "../util/ResourceLocation.h"

#include <unordered_map>

class ICriterionTrigger;

class CriteriaTriggers
{
  public:
    static ImpossibleTrigger IMPOSSIBLE;
    static KilledTrigger PLAYER_KILLED_ENTITY;
    static KilledTrigger ENTITY_KILLED_PLAYER;
    static EnterBlockTrigger ENTER_BLOCK;
    static InventoryChangeTrigger INVENTORY_CHANGED;
    static RecipeUnlockedTrigger RECIPE_UNLOCKED;
    static PlayerHurtEntityTrigger PLAYER_HURT_ENTITY;
    static EntityHurtPlayerTrigger ENTITY_HURT_PLAYER;
    static EnchantedItemTrigger ENCHANTED_ITEM;
    static BrewedPotionTrigger BREWED_POTION;
    static ConstructBeaconTrigger CONSTRUCT_BEACON;
    static UsedEnderEyeTrigger USED_ENDER_EYE;
    static SummonedEntityTrigger SUMMONED_ENTITY;
    static BredAnimalsTrigger BRED_ANIMALS;
    static PositionTrigger LOCATION;
    static PositionTrigger SLEPT_IN_BED;
    static CuredZombieVillagerTrigger CURED_ZOMBIE_VILLAGER;
    static VillagerTradeTrigger VILLAGER_TRADE;
    static ItemDurabilityTrigger ITEM_DURABILITY_CHANGED;
    static LevitationTrigger LEVITATION;
    static ChangeDimensionTrigger CHANGED_DIMENSION;
    static TickTrigger TICK;
    static TameAnimalTrigger TAME_ANIMAL;
    static PlacedBlockTrigger PLACED_BLOCK;
    static ConsumeItemTrigger CONSUME_ITEM;
    static EffectsChangedTrigger EFFECTS_CHANGED;
    static UsedTotemTrigger USED_TOTEM;
    static NetherTravelTrigger NETHER_TRAVEL;

    static ICriterionTrigger *get(const ResourceLocation &id);
    static Iterable getAll();

  private:
    static ICriterionTrigger *regist(ICriterionTrigger *criterion);

    static std::unordered_map<uint64_t, CriteriaTriggers> REGISTRY;
};

#pragma once
#include <unordered_map>
#include "StatBase.h"
#include "StatBasic.h"
#include "text/TextComponentTranslation.h"

class Item;
class Block;

class StatList
{
public:
	static std::vector<StatBase*> ALL_STATS;
	static std::vector<StatBase*> BASIC_STATS;
	static std::vector<StatBase*> USE_ITEM_STATS;
	static std::vector<StatBase*> MINE_BLOCK_STATS;
	static StatBase LEAVE_GAME;
	static StatBase PLAY_ONE_MINUTE;
	static StatBase TIME_SINCE_DEATH;
	static StatBase SNEAK_TIME;
	static StatBase WALK_ONE_CM;
	static StatBase CROUCH_ONE_CM;
	static StatBase SPRINT_ONE_CM;
	static StatBase SWIM_ONE_CM;
	static StatBase FALL_ONE_CM;
	static StatBase CLIMB_ONE_CM;
	static StatBase FLY_ONE_CM;
	static StatBase DIVE_ONE_CM;
	static StatBase MINECART_ONE_CM;
	static StatBase BOAT_ONE_CM;
	static StatBase PIG_ONE_CM;
	static StatBase HORSE_ONE_CM;
	static StatBase AVIATE_ONE_CM;
	static StatBase JUMP;
	static StatBase DROP;
	static StatBase DAMAGE_DEALT;
	static StatBase DAMAGE_TAKEN;
	static StatBase DEATHS;
	static StatBase MOB_KILLS;
	static StatBase ANIMALS_BRED;
	static StatBase PLAYER_KILLS;
	static StatBase FISH_CAUGHT;
	static StatBase TALKED_TO_VILLAGER;
	static StatBase TRADED_WITH_VILLAGER;
	static StatBase CAKE_SLICES_EATEN;
	static StatBase CAULDRON_FILLED;
	static StatBase CAULDRON_USED;
	static StatBase ARMOR_CLEANED;
	static StatBase BANNER_CLEANED;
	static StatBase BREWINGSTAND_INTERACTION;
	static StatBase BEACON_INTERACTION;
	static StatBase DROPPER_INSPECTED;
	static StatBase HOPPER_INSPECTED;
	static StatBase NOTEBLOCK_PLAYED;
	static StatBase NOTEBLOCK_TUNED;
	static StatBase FLOWER_POTTED;
	static StatBase TRAPPED_CHEST_TRIGGERED;
	static StatBase ENDERCHEST_OPENED;
	static StatBase ITEM_ENCHANTED;
	static StatBase RECORD_PLAYED;
	static StatBase FURNACE_INTERACTION;
	static StatBase CRAFTING_TABLE_INTERACTION;
	static StatBase CHEST_OPENED;
	static StatBase SLEEP_IN_BED;
	static StatBase OPEN_SHULKER_BOX;

	StatBase* getBlockStats(Block* blockIn);
	StatBase* getCraftStats(Item* itemIn);
	StatBase* getObjectUseStats(Item* itemIn);
	StatBase* getObjectBreakStats(Item* itemIn);
	StatBase* getObjectsPickedUpStats(Item* itemIn);
	StatBase* getDroppedObjectStats(Item* itemIn);
	void init();
protected:
	static std::unordered_map<> ID_TO_STAT_MAP;
private:	
	static std::vector<StatBase*> BLOCKS_STATS;
	static std::vector<StatBase*> CRAFTS_STATS;
	static std::vector<StatBase*> OBJECT_USE_STATS;
	static std::vector<StatBase*> OBJECT_BREAK_STATS;
	static std::vector<StatBase*> OBJECTS_PICKED_UP_STATS;
	static std::vector<StatBase*> OBJECTS_DROPPED_STATS;
};

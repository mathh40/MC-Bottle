#include "StatList.h"

StatBase StatList::LEAVE_GAME = StatBasic("stat.leaveGame", TextComponentTranslation("stat.leaveGame", new Object[0]))).initIndependentStat().registerStat();
StatBase StatList::PLAY_ONE_MINUTE;
StatBase StatList::TIME_SINCE_DEATH;
StatBase StatList::SNEAK_TIME;
StatBase StatList::WALK_ONE_CM;
StatBase StatList::CROUCH_ONE_CM;
StatBase StatList::SPRINT_ONE_CM;
StatBase StatList::SWIM_ONE_CM;
StatBase StatList::FALL_ONE_CM;
StatBase StatList::CLIMB_ONE_CM;
StatBase StatList::FLY_ONE_CM;
StatBase StatList::DIVE_ONE_CM;
StatBase StatList::MINECART_ONE_CM;
StatBase StatList::BOAT_ONE_CM;
StatBase StatList::PIG_ONE_CM;
StatBase StatList::HORSE_ONE_CM;
StatBase StatList::AVIATE_ONE_CM;
StatBase StatList::JUMP;
StatBase StatList::DROP;
StatBase StatList::DAMAGE_DEALT;
StatBase StatList::DAMAGE_TAKEN;
StatBase StatList::DEATHS;
StatBase StatList::MOB_KILLS;
StatBase StatList::ANIMALS_BRED;
StatBase StatList::PLAYER_KILLS;
StatBase StatList::FISH_CAUGHT;
StatBase StatList::TALKED_TO_VILLAGER;
StatBase StatList::TRADED_WITH_VILLAGER;
StatBase StatList::CAKE_SLICES_EATEN;
StatBase StatList::CAULDRON_FILLED;
StatBase StatList::CAULDRON_USED;
StatBase StatList::ARMOR_CLEANED;
StatBase StatList::BANNER_CLEANED;
StatBase StatList::BREWINGSTAND_INTERACTION;
StatBase StatList::BEACON_INTERACTION;
StatBase StatList::DROPPER_INSPECTED;
StatBase StatList::HOPPER_INSPECTED;
StatBase StatList::NOTEBLOCK_PLAYED;
StatBase StatList::NOTEBLOCK_TUNED;
StatBase StatList::FLOWER_POTTED;
StatBase StatList::TRAPPED_CHEST_TRIGGERED;
StatBase StatList::ENDERCHEST_OPENED;
StatBase StatList::ITEM_ENCHANTED;
StatBase StatList::RECORD_PLAYED;
StatBase StatList::FURNACE_INTERACTION;
StatBase StatList::CRAFTING_TABLE_INTERACTION;
StatBase StatList::CHEST_OPENED;
StatBase StatList::SLEEP_IN_BED;
StatBase StatList::OPEN_SHULKER_BOX;

StatBase* StatList::getBlockStats(Block* blockIn)
{
	return BLOCKS_STATS[Block::getIdFromBlock(blockIn)];
}

StatBase* StatList::getCraftStats(Item* itemIn)
{
	return CRAFTS_STATS[Item::getIdFromItem(itemIn)];
}

StatBase* StatList::getObjectUseStats(Item* itemIn)
{
	return OBJECT_USE_STATS[Item::getIdFromItem(itemIn)];
}

StatBase* StatList::getObjectBreakStats(Item* itemIn)
{
	return OBJECT_BREAK_STATS[Item::getIdFromItem(itemIn)];
}

StatBase* StatList::getObjectsPickedUpStats(Item* itemIn)
{
	return OBJECTS_PICKED_UP_STATS[Item::getIdFromItem(itemIn)];
}

StatBase* StatList::getDroppedObjectStats(Item* itemIn)
{
	return OBJECTS_DROPPED_STATS[Item::getIdFromItem(itemIn)];
}

void StatList::init()
{
	initMiningStats();
	initStats();
	initItemDepleteStats();
	initCraftableStats();
	initPickedUpAndDroppedStats();
}

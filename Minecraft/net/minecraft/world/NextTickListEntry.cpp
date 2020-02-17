#include "NextTickListEntry.h"

NextTickListEntry::NextTickListEntry(BlockPos& positionIn, Block* blockIn)
:tickEntryID(nextTickEntryID++), position(positionIn), block(blockIn)
{
}

NextTickListEntry& NextTickListEntry::setScheduledTime(int64_t scheduledTimeIn)
{
	scheduledTime = scheduledTimeIn;
	return *this;
}

void NextTickListEntry::setPriority(int32_t priorityIn)
{
	priority = priorityIn;
}

std::string NextTickListEntry::toString()
{
	return Block::getIdFromBlock(block) + ": " + position + ", " + scheduledTime + ", " + priority + ", " + tickEntryID;
}

Block* NextTickListEntry::getBlock() const
{
	return block;
}

bool operator==(const NextTickListEntry& lhs, const NextTickListEntry& rhs)
{
	return lhs.position == rhs.position && Block::isEqualTo(lhs.block, rhs.block);
}

bool operator>(const NextTickListEntry& lhs, const NextTickListEntry& rhs)
{
	if(lhs.scheduledTime > rhs.scheduledTime) 
	{
		return true;
	}
	else if (lhs.priority != rhs.priority)
	{
		return (lhs.priority - rhs.priority) > 0;
	}
	else if (lhs.tickEntryID < rhs.tickEntryID)
	{
		return false;
	}
	else 
	{
		return lhs.tickEntryID > rhs.tickEntryID;
	}
}

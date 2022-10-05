#pragma once
#include "../block/Block.h"

class NextTickListEntry
{
  public:
    BlockPos position;
    int64_t scheduledTime{};
    int32_t priority{};
    NextTickListEntry(BlockPos &positionIn, Block *blockIn);
    friend bool operator==(const NextTickListEntry &lhs, const NextTickListEntry &rhs);
    friend bool operator>(const NextTickListEntry &lhs, const NextTickListEntry &rhs);
    NextTickListEntry &setScheduledTime(int64_t scheduledTimeIn);
    void setPriority(int32_t priorityIn);
    std::string toString();
    Block *getBlock() const;

  private:
    static int64_t nextTickEntryID;
    Block *block;
    int64_t tickEntryID;
};

#pragma once
#include "NBTTagCompound.h"

#include <string>
#include <vector>
class EntityPlayer;

class IPlayerFileData
{
  public:
    virtual void writePlayerData(EntityPlayer *var1)                           = 0;
    virtual std::unique_ptr<NBTTagCompound> readPlayerData(EntityPlayer *var1) = 0;
    virtual std::vector<std::string> getAvailablePlayerDat()                   = 0;
};

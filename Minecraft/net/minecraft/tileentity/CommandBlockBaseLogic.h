#pragma once

#include "../command/CommandResultStats.h"
#include "../command/ICommandSender.h"

#include <NBTTagCompound.h>
#include <string>
#include <text\ITextComponent.h>

class ByteBuffer;
class EntityPlayer;
class World;

class CommandBlockBaseLogic : public ICommandSender
{
  public:
    int32_t getSuccessCount();
    void setSuccessCount(int32_t successCountIn);
    ITextComponent *getLastOutput();
    NBTTagCompound *writeToNBT(NBTTagCompound *p_189510_1_);
    void readDataFromNBT(NBTTagCompound *nbt);
    bool canUseCommand(int32_t permLevel, std::string_view commandName);
    virtual void setCommand(std::string_view command);
    std::string getCommand() const;
    bool trigger(World *worldIn);
    std::string getName() const;
    void setName(std::string_view name);
    void sendMessage(ITextComponent *component);
    bool sendCommandFeedback();
    void setCommandStat(CommandResultStats::Type type, int32_t amount);
    void setLastOutput(ITextComponent *lastOutputMessage);
    void setTrackOutput(bool shouldTrackOutput);
    bool shouldTrackOutput() const;
    bool tryOpenEditCommandBlock(EntityPlayer *playerIn);
    CommandResultStats getCommandResultStats();

    virtual void updateCommand()              = 0;
    virtual int32_t getCommandBlockType()     = 0;
    virtual void fillInInfo(ByteBuffer &var1) = 0;

  private:
    static const SimpleDateFormat TIMESTAMP_FORMAT = new SimpleDateFormat("HH:mm:ss");
    int64_t lastExecution                          = -1L;
    bool updateLastExecution                       = true;
    int32_t successCount;
    bool trackOutput = true;
    std::unique_ptr<ITextComponent> lastOutput;
    std::string commandStored;
    std::string customName = "@";
    const CommandResultStats resultStats;
};

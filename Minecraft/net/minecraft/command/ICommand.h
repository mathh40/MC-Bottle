#pragma once
#include <span>
#include <string>

class BlockPos;
class MinecraftServer;
class ICommandSender;

class ICommand
{
    virtual std::string getName()                                                                  = 0;
    virtual std::string getUsage(ICommandSender *var1)                                             = 0;
    virtual List getAliases()                                                                      = 0;
    virtual void execute(MinecraftServer *var1, ICommandSender *var2, std::span<std::string> var3) = 0;
    virtual bool checkPermission(MinecraftServer *var1, ICommandSender *var2)                      = 0;
    virtual List getTabCompletions(MinecraftServer *var1, ICommandSender *var2, std::span<std::string> var3,
                                   const BlockPos &var4)                                           = 0;
    virtual bool isUsernameIndex(std::span<std::string> var1, int32_t var2)                        = 0;
};

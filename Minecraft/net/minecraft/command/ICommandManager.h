#pragma once
#include <string_view>
class BlockPos;
class ICommandSender;

class ICommandManager {

    virtual int32_t executeCommand(ICommandSender* var1, std::string_view var2) = 0 ;
   virtual List getTabCompletions(ICommandSender* var1, std::string_view var2, const BlockPos& var3)= 0;
   virtual List getPossibleCommands(ICommandSender* var1)= 0;
   virtual Map getCommands()= 0;
};

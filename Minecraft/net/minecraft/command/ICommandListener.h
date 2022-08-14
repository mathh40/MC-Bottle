#pragma once
#include <string_view>
#include <vector>
class ICommand;
class ICommandSender;

class ICommandListener {
   virtual void notifyListener(ICommandSender* var1, ICommand* var2, int32_t var3, std::string_view var4, std::vector<> var5) = 0;
};

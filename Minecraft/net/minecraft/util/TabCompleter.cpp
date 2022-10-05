#include "TabCompleter.h"

#include "../client/Minecraft.h"
#include "StringUtils.h"

TabCompleter::TabCompleter(GuiTextField textFieldIn, bool hasTargetBlockIn)
    : textField(textFieldIn), hasTargetBlock(hasTargetBlockIn)
{
}

void TabCompleter::complete()
{
    if (didComplete)
    {
        textField.deleteFromCursor(0);
        textField.deleteFromCursor(textField.getNthWordFromPosWS(-1, textField.getCursorPosition(), false) -
                                   textField.getCursorPosition());
        if (completionIdx >= completions.size())
        {
            completionIdx = 0;
        }
    }
    else
    {
        int i = textField.getNthWordFromPosWS(-1, textField.getCursorPosition(), false);
        completions.clear();
        completionIdx = 0;
        std::string s = textField.getText().substring(0, textField.getCursorPosition());
        requestCompletions(s);
        if (completions.empty())
        {
            return;
        }

        didComplete = true;
        textField.deleteFromCursor(i - textField.getCursorPosition());
    }

    textField.writeText(completions[completionIdx++]);
}

void TabCompleter::resetDidComplete()
{
    didComplete = false;
}

void TabCompleter::resetRequested()
{
    requestedCompletions = false;
}

void TabCompleter::setCompletions(std::initializer_list<std::string> newCompl)
{
    if (requestedCompletions)
    {
        didComplete = false;
        completions.clear();
        std::vector<std::string> var2 = newCompl;
        int var3                      = newCompl.size();

        for (int var4 = 0; var4 < var3; ++var4)
        {
            auto s = var2[var4];
            if (!s.empty())
            {
                completions.emplace_back(s);
            }
        }

        auto s1 =
            textField.getText().substring(textField.getNthWordFromPosWS(-1, textField.getCursorPosition(), false));
        auto s2 = StringUtils::getCommonPrefix(newCompl);
        if (!s2.empty() && !s1.equalsIgnoreCase(s2))
        {
            textField.deleteFromCursor(0);
            textField.deleteFromCursor(textField.getNthWordFromPosWS(-1, textField.getCursorPosition(), false) -
                                       textField.getCursorPosition());
            textField.writeText(s2);
        }
        else if (!completions.empty())
        {
            didComplete = true;
            complete();
        }
    }
}

void TabCompleter::requestCompletions(std::string prefix)
{
    if (prefix.length() >= 1)
    {
        Minecraft.getMinecraft().player.connection.sendPacket(
            new CPacketTabComplete(prefix, getTargetBlockPos(), hasTargetBlock));
        requestedCompletions = true;
    }
}

#include "HoverEvent.h"

Action Action::SHOW_TEXT("show_text", true);
Action Action::SHOW_ITEM("show_item", true);
Action Action::SHOW_ENTITY("show_entity", true);

bool Action::shouldAllowInChat() const
{
    return allowedInChat;
}

std::string Action::getCanonicalName() const
{
    return canonicalName;
}

Action *Action::getValueByCanonicalName(std::string canonicalNameIn)
{
    return NAME_MAPPING[canonicalNameIn];
}

Action::Action(std::string canonicalNameIn, bool allowedInChatIn)
    : canonicalName(canonicalNameIn), allowedInChat(allowedInChatIn)
{
    NAME_MAPPING.emplace(canonicalNameIn, this);
}

HoverEvent::HoverEvent(Action theAction, ITextComponent *theValue) : action(theAction), value(theValue)
{
}

const Action &HoverEvent::getAction() const
{
    return action;
}

ITextComponent *HoverEvent::getValue() const
{
    return value;
}

std::string HoverEvent::toString()
{
    return "HoverEvent{action=" + action + ", value='" + value + '\'' + '}';
}

bool operator==(const HoverEvent &lhs, const HoverEvent &rhs)
{
    if (lhs.getAction() != rhs.getAction())
    {
        return false;
    }
    if (!(lhs.getValue() == rhs.getValue()))
    {
        return false;
    }
    return true;
}

bool operator!=(const HoverEvent &lhs, const HoverEvent &rhs)
{
    return !(lhs == rhs);
}

bool operator==(const Action &lhs, const Action &rhs)
{
    return lhs.getCanonicalName() == rhs.getCanonicalName();
}

auto operator!=(const Action &lhs, const Action &rhs) -> bool
{
    return !(lhs == rhs);
}

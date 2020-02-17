#include "ClickEvent.h"

Action Action::OPEN_URL("open_url", true);
Action Action::OPEN_FILE("open_file", false);
Action Action::RUN_COMMAND("run_command", true);
Action Action::SUGGEST_COMMAND("suggest_command", true);
Action Action::CHANGE_PAGE("change_page", true);

bool Action::shouldAllowInChat() const
{
	return allowedInChat;
}

std::string Action::getCanonicalName() const
{
	return canonicalName;
}

Action* Action::getValueByCanonicalName(std::string canonicalNameIn)
{
	return NAME_MAPPING[canonicalNameIn];
}

Action::Action(std::string canonicalNameIn, bool allowedInChatIn)
	:canonicalName(canonicalNameIn), allowedInChat(allowedInChatIn)
{
	NAME_MAPPING.emplace(canonicalNameIn, this);
}

ClickEvent::ClickEvent(Action theAction, std::string theValue)
	: action(theAction), value(theValue)
{

}

const Action& ClickEvent::getAction() const
{
	return action;
}

std::string ClickEvent::getValue() const
{
	return value;
}

std::string ClickEvent::toString()
{
	return "ClickEvent{action=" + action + ", value='" + value + '\'' + '}';
}


bool operator==(const ClickEvent& lhs, const ClickEvent& rhs)
{
	if (lhs.getAction() != rhs.getAction()) {
		return false;
	}
	if (!(lhs.getValue() == rhs.getValue())) {
		return false;
	}
	return true;
}

bool operator!=(const ClickEvent& lhs, const ClickEvent& rhs)
{
	return!(lhs == rhs);
}

bool operator==(const Action& lhs, const Action& rhs)
{
	return lhs.getCanonicalName() == rhs.getCanonicalName();
}

auto operator!=(const Action& lhs, const Action& rhs) -> bool
{
	return !(lhs == rhs);
}

#pragma once
#include <string>
#include <unordered_map>

class Action
{
public:
	bool shouldAllowInChat() const;
	std::string getCanonicalName() const;
	static Action* getValueByCanonicalName(std::string canonicalNameIn);
private:
	static Action OPEN_URL;
	static Action OPEN_FILE;
	static Action RUN_COMMAND;
	static Action SUGGEST_COMMAND;
	static Action CHANGE_PAGE;

	Action(std::string canonicalNameIn, bool allowedInChatIn);

	bool allowedInChat;
	std::string canonicalName;
	static std::unordered_map<std::string, Action*> NAME_MAPPING;
};

class ClickEvent
{
public:
	ClickEvent(Action theAction, std::string theValue);
	const Action& getAction()const;
	std::string getValue()const;
	std::string toString();
private:
	std::string value;
	Action action;
};

bool operator==(const ClickEvent& lhs, const ClickEvent& rhs);
bool operator!=(const ClickEvent& lhs, const ClickEvent& rhs);
bool operator==(const Action& lhs, const Action& rhs);
auto operator!=(const Action& lhs, const Action& rhs) -> bool;

namespace std {

	template <> struct hash<Action>
	{
		size_t operator()(const Action& x) const noexcept
		{
			return std::hash<std::string>()(x.getCanonicalName());
		}
	};

	template <> struct hash<ClickEvent>
	{
		size_t operator()(const ClickEvent& x) const noexcept
		{
			auto i = std::hash<Action>()(x.getAction());
			i = 31 * i + (!x.getValue().empty() ? std::hash<std::string>()(x.getValue()) : 0);
			return i;
		}
	};
}
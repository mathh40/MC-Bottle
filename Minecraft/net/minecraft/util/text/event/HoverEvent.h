#pragma once
#include "text/ITextComponent.h"

#include <string>
#include <unordered_map>

class Action
{
  public:
    bool shouldAllowInChat() const;
    std::string getCanonicalName() const;
    static Action *getValueByCanonicalName(std::string canonicalNameIn);

  private:
    static Action SHOW_TEXT;
    static Action SHOW_ITEM;
    static Action SHOW_ENTITY;

    Action(std::string canonicalNameIn, bool allowedInChatIn);

    bool allowedInChat;
    std::string canonicalName;
    static std::unordered_map<std::string, Action *> NAME_MAPPING;
};

class HoverEvent
{
  public:
    HoverEvent(Action theAction, ITextComponent *theValue);
    const Action &getAction() const;
    ITextComponent *getValue() const;
    std::string toString();

  private:
    ITextComponent *value;
    Action action;
};

bool operator==(const HoverEvent &lhs, const HoverEvent &rhs);
bool operator!=(const HoverEvent &lhs, const HoverEvent &rhs);
bool operator==(const Action &lhs, const Action &rhs);
auto operator!=(const Action &lhs, const Action &rhs) -> bool;

namespace std
{

template <> struct hash<Action>
{
    size_t operator()(const Action &x) const noexcept
    {
        return std::hash<std::string>()(x.getCanonicalName());
    }
};

template <> struct hash<HoverEvent>
{
    size_t operator()(const HoverEvent &x) const noexcept
    {
        auto i = std::hash<Action>()(x.getAction());
        i      = 31 * i + ((x.getValue() != nullptr) ? std::hash<ITextComponent *>()(x.getValue()) : 0);
        return i;
    }
};
} // namespace std

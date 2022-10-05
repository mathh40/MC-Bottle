#pragma once
#include "../../../../../nlohmann_json/single_include/nlohmann/json.hpp"
#include "ITextComponent.h"
#include "TextFormatting.h"
#include "event/ClickEvent.h"
#include "event/HoverEvent.h"

#include <optional>
#include <string>

class TextFormatting;

class Style
{
  public:
    virtual ~Style() = default;
    virtual std::optional<TextFormatting> getColor() const;
    virtual std::optional<bool> getBold() const;
    virtual std::optional<bool> getItalic() const;
    virtual std::optional<bool> getStrikethrough() const;
    virtual std::optional<bool> getUnderlined() const;
    virtual std::optional<bool> getObfuscated() const;
    bool isEmpty() const;
    virtual std::optional<ClickEvent> getClickEvent() const;
    virtual std::optional<HoverEvent> getHoverEvent() const;
    virtual std::optional<std::string> getInsertion() const;

    virtual Style &setColor(TextFormatting colorIn);
    virtual Style &setBold(bool boldIn);
    virtual Style &setItalic(bool italicIn);
    virtual Style &setStrikethrough(bool strikethroughIn);
    virtual Style &setUnderlined(bool underlinedIn);
    virtual Style &setObfuscated(bool obfuscatedIn);
    virtual Style &setClickEvent(ClickEvent eventIn);
    virtual Style &setHoverEvent(HoverEvent eventIn);
    virtual Style &setInsertion(std::string insertionIn);
    virtual Style &setParentStyle(const Style &parent);

    virtual std::string to_string() const;
    virtual std::string getFormattingCode() const;

    friend bool operator==(const Style &a, const Style &b);

  private:
    std::optional<Style> parentStyle;
    std::optional<TextFormatting> color;
    std::optional<bool> bold;
    std::optional<bool> italic;
    std::optional<bool> underlined;
    std::optional<bool> strikethrough;
    std::optional<bool> obfuscated;
    std::optional<ClickEvent> clickEvent;
    std::optional<HoverEvent> hoverEvent;
    std::optional<std::string> insertion;

    Style getParent() const;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<Style>
{
    static void to_json(json &j, const Style &p)
    {
        json jsonobject;
        if (p.getBold())
        {
            j["bold"] = p.getBold().value();
        }

        if (p.getItalic())
        {
            j["italic"] = p.getItalic().value();
        }

        if (p.getUnderlined())
        {
            j["underlined"] = p.getUnderlined().value();
        }

        if (p.getStrikethrough())
        {
            j["strikethrough"] = p.getStrikethrough().value();
        }

        if (p.getObfuscated())
        {
            j["obfuscated"] = p.getObfuscated().value();
        }

        if (p.getColor())
        {
            j["color"] = p.getColor().value();
        }

        if (p.getInsertion())
        {
            j["insertion"] = p.getInsertion().value();
        }

        if (p.getClickEvent())
        {
            json jsonobject2;
            jsonobject2["action"], p.getClickEvent().value().getAction().getCanonicalName();
            jsonobject2["value"], p.getClickEvent().value().getValue();
            j["clickEvent"] = jsonobject2;
        }

        if (p.getHoverEvent())
        {
            json jsonobject2;
            jsonobject2["action"] = p.getHoverEvent().value().getAction().getCanonicalName();
            jsonobject2["value"]  = p.getHoverEvent().value().getValue();
            j["hoverEvent"]       = jsonobject2;
        }
    }

    static void from_json(const json &j, Style &p)
    {
        if (j.is_object())
        {
            Style style;
            auto jsonobject = j;

            if (jsonobject.find("bold") != jsonobject.end())
            {
                style.setBold(jsonobject.at("bold").get<bool>());
            }

            if (jsonobject.find("italic") != jsonobject.end())
            {
                style.setItalic(jsonobject.at("italic").get<bool>());
            }

            if (jsonobject.find("underlined") != jsonobject.end())
            {
                style.setUnderlined(jsonobject.at("underlined").get<bool>());
            }

            if (jsonobject.find("strikethrough") != jsonobject.end())
            {
                style.setStrikethrough(jsonobject.at("strikethrough").get<bool>());
            }

            if (jsonobject.find("obfuscated") != jsonobject.end())
            {
                style.setObfuscated(jsonobject.at("obfuscated").get<bool>());
            }

            if (jsonobject.find("color") != jsonobject.end())
            {
                style.setColor(jsonobject.at("color").get<TextFormatting>());
            }

            if (jsonobject.find("insertion") != jsonobject.end())
            {
                style.setInsertion(jsonobject.at("insertion").get<std::string>());
            }

            if (jsonobject.find("clickEvent") != jsonobject.end())
            {
                auto jsonobject2    = jsonobject.at("clickEvent");
                auto jsonprimitive2 = jsonobject2.at("action");
                Action clickevent$action =
                    jsonprimitive2 == nullptr
                        ? nullptr
                        : HoverEvent::Action::getValueByCanonicalName(jsonprimitive2.getAsString());
                auto jsonprimitive1 = jsonobject2.at("value");
                auto s              = jsonprimitive1.get<std::string>();
                if (clickevent$action != nullptr && s != nullptr && clickevent$action.shouldAllowInChat())
                {
                    style.setClickEvent(ClickEvent(clickevent$action, s));
                }
            }

            if (jsonobject.find("hoverEvent") != jsonobject.end())
            {
                auto jsonobject2    = jsonobject.at("hoverEvent");
                auto jsonprimitive2 = jsonobject2.at("action");
                HoverEvent::Action hoverevent$action =
                    jsonprimitive2 == nullptr
                        ? nullptr
                        : HoverEvent::Action::getValueByCanonicalName(jsonprimitive2.getAsString());
                auto itextcomponent = jsonobject2.at("value").get<ITextComponent>();
                if (hoverevent$action != nullptr && itextcomponent != nullptr && hoverevent$action.shouldAllowInChat())
                {
                    style.setHoverEvent(HoverEvent(hoverevent$action, itextcomponent));
                }
            }

            p = style;
        }
    }
};
NLOHMANN_JSON_NAMESPACE_END

namespace std
{
template <> struct hash<Style>
{
    size_t operator()(const Style &x) const noexcept
    {
        size_t i = std::hash<TextFormatting>{}(x.getColor().value());
        i        = 31 * i + std::hash<bool>{}(x.getBold().value());
        i        = 31 * i + std::hash<bool>{}(x.getItalic().value());
        i        = 31 * i + std::hash<bool>{}(x.getUnderlined().value());
        i        = 31 * i + std::hash<bool>{}(x.getStrikethrough().value());
        i        = 31 * i + std::hash<bool>{}(x.getObfuscated().value());
        i        = 31 * i + std::hash<ClickEvent>{}(x.getClickEvent().value());
        i        = 31 * i + std::hash<HoverEvent>{}(x.getHoverEvent().value());
        i        = 31 * i + std::hash<std::string>{}(x.getInsertion().value());
        return i;
    }
};
} // namespace std

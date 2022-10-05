#pragma once
#include "Style.h"

class RootStyle : public Style
{
  public:
    std::optional<TextFormatting> getColor() const override;
    std::optional<bool> getBold() const override;
    std::optional<bool> getItalic() const override;
    std::optional<bool> getStrikethrough() const override;
    std::optional<bool> getUnderlined() const override;
    std::optional<bool> getObfuscated() const override;
    std::optional<ClickEvent> getClickEvent() const override;
    std::optional<HoverEvent> getHoverEvent() const override;
    std::optional<std::string> getInsertion() const override;
    Style &setBold(bool boldIn) override;
    Style &setItalic(bool italicIn) override;
    Style &setStrikethrough(bool strikethroughIn) override;
    Style &setUnderlined(bool underlinedIn) override;
    Style &setObfuscated(bool obfuscatedIn) override;
    Style &setInsertion(std::string insertionIn) override;
    Style &setParentStyle(const Style &parent) override;
    std::string to_string() const override;
    std::string getFormattingCode() const override;
};

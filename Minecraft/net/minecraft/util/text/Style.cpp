#include "Style.h"
#include <spdlog/fmt/bundled/format.h>
#include <sstream>
#include "RootStyle.h"

RootStyle ROOT;

std::optional<TextFormatting> Style::getColor() const
{
	return color.has_value() ? color : getParent().getColor();
}

std::optional<bool> Style::getBold() const
{
	return bold.has_value() ? bold : getParent().getBold();
}

std::optional<bool> Style::getItalic() const
{
	return italic.has_value() ? italic : getParent().getItalic();
}

std::optional<bool> Style::getStrikethrough() const
{
	return strikethrough.has_value() ? strikethrough : getParent().getStrikethrough();
}

std::optional<bool> Style::getUnderlined() const
{
	return underlined.has_value() ? underlined : getParent().getUnderlined();
}

std::optional<bool> Style::getObfuscated() const
{
	return obfuscated.has_value() ? obfuscated : getParent().getObfuscated();
}

bool Style::isEmpty() const
{
	return bold && italic && strikethrough && underlined && obfuscated && color  && clickEvent  && hoverEvent  && insertion;
}

std::optional<ClickEvent> Style::getClickEvent() const
{
	return clickEvent.has_value() ? clickEvent : getParent().getClickEvent();
}

std::optional<HoverEvent> Style::getHoverEvent() const
{
	return hoverEvent.has_value() ? hoverEvent : getParent().getHoverEvent();
}

std::optional<std::string> Style::getInsertion() const
{
	return insertion.has_value() ? insertion : getParent().getInsertion();
}

Style& Style::setColor(TextFormatting colorIn)
{
	color = colorIn;
	return *this;
}

Style& Style::setBold(bool boldIn)
{
	bold = boldIn;
	return *this;
}

Style& Style::setItalic(bool italicIn)
{
	italic = italicIn;
	return *this;
}

Style& Style::setStrikethrough(bool strikethroughIn)
{
	strikethrough = strikethroughIn;
	return *this;
}

Style& Style::setUnderlined(bool underlinedIn)
{
	underlined = underlinedIn;
	return *this;
}

Style& Style::setObfuscated(bool obfuscatedIn)
{
	obfuscated = obfuscatedIn;
	return *this;
}

Style& Style::setClickEvent(ClickEvent eventIn)
{
	clickEvent = eventIn;
	return *this;
}

Style& Style::setHoverEvent(HoverEvent eventIn)
{
	hoverEvent = eventIn;
	return *this;
}

Style& Style::setInsertion(std::string insertionIn)
{
	insertion = insertionIn;
	return *this;
}

Style& Style::setParentStyle(const Style& parent)
{
	parentStyle = parent;
	return *this;
}

std::string Style::to_string() const
{
    return fmt::format("Style{hasParent={}, color={}, bold={}, italic={}, underlined={}, obfuscated={}, clickEvent={}, hoverEvent={}, insertion={}}",parentStyle.value(),color.value(),bold.value(),italic.value(), getClickEvent().value(),underlined.value(),obfuscated.value(),getHoverEvent().value(),getInsertion().value());
}

std::string Style::getFormattingCode() const
{

	if (isEmpty()) {
		return parentStyle ? parentStyle->getFormattingCode() : "";
	}
	else {

		std::stringstream stringbuilder("");
		if (getColor()) {
                stringbuilder << getColor().value();
		}

		if (getBold()) {
			stringbuilder << TextFormatting::BOLD;
		}

		if (getItalic()) {
			stringbuilder << (TextFormatting::ITALIC);
		}

		if (getUnderlined()) {
			stringbuilder << (TextFormatting::UNDERLINE);
		}

		if (getObfuscated()) {
			stringbuilder << (TextFormatting::OBFUSCATED);
		}

		if (getStrikethrough()) {
			stringbuilder << (TextFormatting::STRIKETHROUGH);
		}
        return stringbuilder.str();
	}
}

Style Style::getParent() const
{
	return parentStyle.has_value() ? parentStyle.value() : ROOT;
}

bool operator==(const Style& a, const Style& b)
{
	if (a.getBold() == b.getBold() && a.getColor() == b.getColor() && a.getItalic() == b.getItalic() && a.getObfuscated() == b.getObfuscated() && a.getStrikethrough() == b.getStrikethrough() && a.getUnderlined() == b.getUnderlined())
	{
		if (a.getClickEvent().has_value()) {
			if (!(a.getClickEvent() == (b.getClickEvent()))) {
				return false;
			}
		}
		else if (b.getClickEvent().has_value()) {
			return false;
		}

		if (a.getHoverEvent().has_value()) {
			if (!(a.getHoverEvent() == (b.getHoverEvent()))) {
				return false;
			}
		}
		else if (b.getHoverEvent().has_value()) {
			return false;
		}

		if (a.getInsertion().has_value()) {
			if (!(a.getInsertion() == (b.getInsertion()))) {
				return false;
			}
		}
		else if (b.getInsertion().has_value()) {
			return false;
		}
        else 
        {
			return true;
		}
	}
	return false;
}

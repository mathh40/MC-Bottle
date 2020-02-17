#include "Style.h"
#include <spdlog/fmt/bundled/format.h>
#include <sstream>
#include "RootStyle.h"

RootStyle ROOT;

std::optional<TextFormatting> Style::getColor() const
{
	return color.has_value() ? color : getParent()->getColor();
}

const std::optional<bool> Style::getBold() const
{
	return bold.has_value() ? bold : getParent()->getBold();
}

const std::optional<bool> Style::getItalic() const
{
	return italic.has_value() ? italic : getParent()->getItalic();
}

const std::optional<bool> Style::getStrikethrough() const
{
	return strikethrough.has_value() ? strikethrough : getParent()->getStrikethrough();
}

const std::optional<bool> Style::getUnderlined() const
{
	return underlined.has_value() ? underlined : getParent()->getUnderlined();
}

const std::optional<bool> Style::getObfuscated() const
{
	return obfuscated.has_value() ? obfuscated : getParent()->getObfuscated();
}

bool Style::isEmpty() const
{
	return bold && italic && strikethrough && underlined && obfuscated && color  && clickEvent  && hoverEvent  && insertion;
}

std::optional<ClickEvent> Style::getClickEvent() const
{
	return clickEvent.has_value() ? clickEvent : getParent()->getClickEvent();
}

std::optional<HoverEvent> Style::getHoverEvent() const
{
	return hoverEvent.has_value() ? hoverEvent : getParent()->getHoverEvent();
}

std::optional<std::string> Style::getInsertion() const
{
	return insertion.has_value() ? insertion : getParent()->getInsertion();
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

Style& Style::setParentStyle(Style parent)
{
	parentStyle = parent;
	return *this;
}

std::string Style::to_string() const
{
	return "Style{hasParent=" + (parentStyle) + ", color=" + color + ", bold=" + bold + ", italic=" + italic + ", underlined=" + underlined + ", obfuscated=" + obfuscated + ", clickEvent=" + getClickEvent() + ", hoverEvent=" + getHoverEvent() + ", insertion=" + getInsertion() + '}';
}

std::string Style::getFormattingCode() const
{

	if (isEmpty()) {
		return parentStyle ? parentStyle->getFormattingCode() : "";
	}
	else {
		std::stringstream stringbuilder;
		if (getColor()) {
			stringbuilder << getColor();
		}

		if (getBold()) {
			stringbuilder << TextFormatting.BOLD;
		}

		if (getItalic()) {
			stringbuilder << (TextFormatting.ITALIC);
		}

		if (getUnderlined()) {
			stringbuilder << (TextFormatting.UNDERLINE);
		}

		if (getObfuscated()) {
			stringbuilder << (TextFormatting.OBFUSCATED);
		}

		if (getStrikethrough()) {
			stringbuilder << (TextFormatting.STRIKETHROUGH);
		}

		return stringbuilder.str();
	}
}

const Style* Style::getParent() const
{
	return parentStyle ? &parentStyle.value() : &ROOT;
}

bool operator==(const Style& a, const Style& b)
{
	bool flag;
	if (a.getBold() == b.getBold() && a.getColor() == b.getColor() && a.getItalic() == b.getItalic() && a.getObfuscated() == b.getObfuscated() && a.getStrikethrough() == b.getStrikethrough() && a.getUnderlined() == b.getUnderlined()) {
	label71: {
		if (a.getClickEvent().has_value()) {
			if (!a.getClickEvent() == (b.getClickEvent())) {
				goto label71;
			}
		}
		else if (b.getClickEvent().has_value()) {
			goto label71;
		}

		if (a.getHoverEvent().has_value()) {
			if (!a.getHoverEvent() == (b.getHoverEvent())) {
				goto label71;
			}
		}
		else if (b.getHoverEvent().has_value()) {
			goto label71;
		}

		if (a.getInsertion().has_value()) {
			if (!a.getInsertion() == (b.getInsertion())) {
				goto label71;
			}
		}
		else if (b.getInsertion().has_value()) {
			goto label71;
		}

		flag = true;
		return flag;
		}
	}

	flag = false;
	return flag;
}

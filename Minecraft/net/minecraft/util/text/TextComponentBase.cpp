#include "ITextComponent.h"
#include "TextComponentBase.h"
#include "TextComponentString.h"
#include "TextFormatting.h"
#include <sstream>

ITextComponent *TextComponentBase::appendSibling(std::shared_ptr<ITextComponent> &component) {
	component->getStyle().setParentStyle(getStyle());
	siblings.push_back(component);
	return this;
}

TextComponentBase::TextComponentList& TextComponentBase::getSiblings()
{
	return siblings;
}

ITextComponent* TextComponentBase::appendText(std::string_view text)
{
    auto appText = std::make_shared<TextComponentString>(text);
    return appendSibling(appText);
}

ITextComponent* TextComponentBase::setStyle(const Style& style)
{
    textstyle = style;

	for(auto itextcomponent : siblings)
	{
		itextcomponent->getStyle().setParentStyle(getStyle());
	}
	return this;
}

Style& TextComponentBase::getStyle()
{ return textstyle; }

std::string TextComponentBase::getUnformattedText() const
{
	std::stringstream stringbuilder;

	for(auto itextcomponent : siblings)
	{
		stringbuilder << (itextcomponent->getUnformattedComponentText());
	}

	return stringbuilder.str();
}

std::string TextComponentBase::getFormattedText() const
{
	std::stringstream stringbuilder;

	for (auto itextcomponent : siblings) {
		auto s = itextcomponent->getUnformattedComponentText();
		if (!s.empty()) {
			stringbuilder << (itextcomponent->getStyle().getFormattingCode());
			stringbuilder << (s);
			stringbuilder << (TextFormatting::RESET);
		}
	}

	return stringbuilder.str();
}

std::string TextComponentBase::toString() const
{
    std::stringstream stringbuilder("BaseComponent{style=" + textstyle.to_string() + ", siblings=");
    for (auto sib : siblings) {
        if (!sib) {

            stringbuilder << sib->getUnformattedText();
            stringbuilder << ',';
        }
	}
    stringbuilder << '}';
    return stringbuilder.str();
}

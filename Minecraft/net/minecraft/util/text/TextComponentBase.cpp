#include "ITextComponent.h"
#include "TextComponentBase.h"
#include "TextComponentString.h"
#include "TextFormatting.h"
#include <sstream>

ITextComponent* TextComponentBase::appendSibling(std::unique_ptr<ITextComponent> component)
{
	component->getStyle().setParentStyle(getStyle());
	siblings.push_back(component);
	return this;
}

TextComponentBase::TextComponentList& TextComponentBase::getSiblings() const
{
	return siblings;
}

ITextComponent* TextComponentBase::appendText(std::string text)
{
	return appendSibling(std::make_unique<TextComponentString>(text));
}

ITextComponent* TextComponentBase::setStyle(Style style)
{
	style = style;

	for(auto itextcomponent : siblings)
	{
		itextcomponent->getStyle().setParentStyle(getStyle());
	}
	return this;
}

Style& TextComponentBase::getStyle()
{
	return style;
}

std::string TextComponentBase::getUnformattedText() const
{
	std::stringstream stringbuilder;

	for(auto itextcomponent : siblings)
	{
		stringbuilder << (itextcomponent.getUnformattedComponentText());
	}

	return stringbuilder.str();
}

std::string TextComponentBase::getFormattedText() const
{
	std::stringstream stringbuilder;

	for (auto itextcomponent : siblings) {
		auto s = itextcomponent.getUnformattedComponentText();
		if (!s.isEmpty()) {
			stringbuilder << (itextcomponent.getStyle().getFormattingCode());
			stringbuilder << (s);
			stringbuilder << (TextFormatting::RESET);
		}
	}

	return stringbuilder.str();
}

std::string TextComponentBase::toString() const
{
	return "BaseComponent{style=" + style + ", siblings=" + siblings + '}';
}

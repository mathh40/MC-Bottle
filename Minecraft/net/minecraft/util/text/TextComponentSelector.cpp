#include "TextComponentSelector.h"

TextComponentSelector::TextComponentSelector(std::string selectorIn)
	:selector(selectorIn)
{

}

std::string TextComponentSelector::getSelector()
{
	return selector;
}

std::string TextComponentSelector::getUnformattedText() const
{
	return selector;
}

std::string TextComponentSelector::toString() const
{
	return "SelectorComponent{pattern='" + selector + '\'' + ", siblings=" + siblings + ", style=" + getStyle() + '}';
}

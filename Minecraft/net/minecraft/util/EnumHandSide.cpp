#include "EnumHandSide.h"

EnumHandSide EnumHandSide::LEFT = EnumHandSide(true,TextComponentTranslation("options.mainHand.left", new Object[0]));
EnumHandSide EnumHandSide::RIGHT = EnumHandSide(false,TextComponentTranslation("options.mainHand.right", new Object[0]));


EnumHandSide::EnumHandSide(bool left,ITextComponent nameIn)
	:left(left),handName(nameIn)
{

}

EnumHandSide EnumHandSide::opposite()
{
	return left ? RIGHT : LEFT;
}

std::string EnumHandSide::toString()
{
	return handName.getUnformattedText();
}

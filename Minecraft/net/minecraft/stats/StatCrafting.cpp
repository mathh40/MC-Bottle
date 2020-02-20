#include "StatCrafting.h"

StatCrafting::StatCrafting(std::string_view p_i45910_1_, std::string_view p_i45910_2_, ITextComponent* statNameIn,
	Item p_i45910_4_)
	:StatBase(p_i45910_1_ + p_i45910_2_, statNameIn),item(p_i45910_4_)
{
	
}

Item StatCrafting::getItem() const
{
	return item;
}

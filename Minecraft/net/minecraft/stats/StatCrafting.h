#pragma once
#include "StatBase.h"
class Item;
class StatCrafting : public StatBase
{
private:
	Item item;
public:
	StatCrafting(std::string_view p_i45910_1_, std::string_view p_i45910_2_, ITextComponent* statNameIn, Item p_i45910_4_);
	Item getItem() const;
};

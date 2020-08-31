#include "EnumDifficulty.h"

EnumDifficulty EnumDifficulty::PEACEFUL(0, "options.difficulty.peaceful");
EnumDifficulty EnumDifficulty::EASY(1, "options.difficulty.easy");
EnumDifficulty EnumDifficulty::NORMAL(2, "options.difficulty.normal");
EnumDifficulty EnumDifficulty::HARD(3, "options.difficulty.hard");

uint16_t EnumDifficulty::getId() const
{
	return id;
}

EnumDifficulty& EnumDifficulty::byId(uint16_t id)
{
	return *ID_MAPPING[id % ID_MAPPING.size()];
}

std::string EnumDifficulty::getTranslationKey() const {
	return translationKey;
}

EnumDifficulty::EnumDifficulty(uint16_t difficultyIdIn, std::string difficultyResourceKeyIn)
	: id(difficultyIdIn),translationKey(difficultyResourceKeyIn)
{
	ID_MAPPING.emplace_back(*this);
}

#include "EnchantmentNameParts.h"

std::shared_ptr<EnchantmentNameParts> EnchantmentNameParts::getInstance()
{
	if(INSTANCE == nullptr)
	{
		INSTANCE = std::make_shared<EnchantmentNameParts>();
	}
	return INSTANCE;
}

std::string EnchantmentNameParts::generateNewRandomName(FontRenderer fontRendererIn, int length)
{
	std::uniform_int_distribution<int> uniform_for(0, 2);
	int i = uniform_for(rand) + 3;
	std::string s = "";

	for (int j = 0; j < i; ++j) {
		if (j > 0) {
			s = s + " ";
		}

		std::uniform_int_distribution<int> uniform_text(0, namePartsArray.size());
		s = s + namePartsArray[uniform_text(rand)];
	}

	auto list = fontRendererIn.listFormattedStringToWidth(s, length);
	return org.apache.commons.lang3.StringUtils.join(list.size() >= 2 ? list.subList(0, 2) : list, " ");
}

void EnchantmentNameParts::reseedRandomGenerator(long seed)
{
	seed_source = pcg_extras::seed_seq_from <long>(seed);
}

EnchantmentNameParts::EnchantmentNameParts()
	:rand(seed_source)
{
	
}


#include "FlatGeneratorInfo.h"
#include <sstream>
#include "spdlog/fmt/bundled/posix.h"
#include "Util.h"
#include "Block.h"
#include "math/MathHelper.h"
#include "biome/Biome.h"

int32_t FlatGeneratorInfo::getBiome() const
{
	return biomeToUse;
}

void FlatGeneratorInfo::setBiome(int32_t biome)
{
	biomeToUse = biome;
}

std::unordered_map<>& FlatGeneratorInfo::getWorldFeatures()
{
	return worldFeatures;
}

std::vector<FlatLayerInfo>& FlatGeneratorInfo::getFlatLayers()
{
	return flatLayers;
}

void FlatGeneratorInfo::updateLayers()
{
	auto i = 0;
	for (auto flatlayerinfo = flatLayers.begin(); flatlayerinfo != flatLayers.end(); i += flatlayerinfo->getLayerCount())
	{
		flatlayerinfo->setMinY(i);
	}
}

std::string FlatGeneratorInfo::toString()
{
	std::stringstream stringbuilder;
	stringbuilder << 3;
	stringbuilder << ";";

	for (auto k = 0; k < flatLayers.size(); ++k) 
	{
		if (k > 0) 
		{
			stringbuilder << ",";
		}

		stringbuilder << flatLayers[k];
	}

	stringbuilder << ";";
	stringbuilder << biomeToUse;
	if (worldFeatures.empty()) 
	{
		stringbuilder << ";";
		return stringbuilder.str();
	}
	else 
	{
		stringbuilder << ";";
		auto k = 0;
		auto var3 = worldFeatures.begin();

		while (true) 
		{
			std::unordered_map<std::string, std::string> map;
			do 
			{
				if (var3 == worldFeatures.end())
				{
					return stringbuilder.str();
				}

				if (k++ > 0) 
				{
					stringbuilder << ",";
				}

				std::locale loc;
				stringbuilder << Util::toLowerCase(var3->first, loc);
				map = var3->second;
			} 
			while (map.empty());

			stringbuilder << "(";
			auto j = 0;

			for (const auto& entry1 : map)
			{
				if (j++ > 0) 
				{
					stringbuilder << " ";
				}

				stringbuilder << entry1.first;
				stringbuilder << "=";
				stringbuilder << entry1.second;
			}

			stringbuilder << ")";
		}
	}
}

FlatGeneratorInfo FlatGeneratorInfo::createFlatGeneratorFromString(std::string_view flatGeneratorSettings)
{
	if (flatGeneratorSettings.empty()) 
	{
		return getDefaultFlatGenerator();
	}
	else 
	{
		auto astring = Util::split(flatGeneratorSettings,";");
		auto i = astring.size() == 1 ? 0 : MathHelper::getInt(astring[0], 0);
		if (i >= 0 && i <= 3) 
		{
			FlatGeneratorInfo flatgeneratorinfo;
			auto j = astring.size() == 1 ? 0 : 1;
			auto liste = getLayersFromString(i, astring[j++]);
			if (!liste.empty()) 
			{
				flatgeneratorinfo.getFlatLayers().insert(liste.begin(), liste.end());
				flatgeneratorinfo.updateLayers();
				auto k = Biome::getIdForBiome(Biomes::PLAINS);
				if (i > 0 && astring.size() > j) 
				{
					k = MathHelper::getInt(astring[j++], k);
				}

				flatgeneratorinfo.setBiome(k);
				if (i > 0 && astring.size() > j)
				{
					std::locale loc;			
					auto astring1 = Util::split(Util::toLowerCase(astring[j++], loc),"'");

					for(auto& s : astring1)
					{
						auto astring2 = Util::split(s, "\\(");
						std::unordered_map<std::string, std::string> map;
						if (!astring2[0].empty()) 
						{							
							if (astring2.size() > 1 && Util::endsWith(astring2[1], ")") && astring2[1].length() > 1)
							{
								auto astring3 = Util::split(astring2[1].substr(0, astring2[1].length() - 1), " ");

								for (auto& s1 : astring1)
								{
									auto astring4 = Util::split(s1, "=");
									if (astring4.size() == 2) 
									{
										map.emplace(astring4[0], astring4[1]);
									}
								}
							}
							flatgeneratorinfo.getWorldFeatures().emplace(astring2[0], map);
						}
					}
				}
				else 
				{
					flatgeneratorinfo.getWorldFeatures().emplace("village", std::unordered_map<std::string, std::string>());
				}

				return flatgeneratorinfo;
			}
			else 
			{
				return getDefaultFlatGenerator();
			}
		}
		else 
		{
			return getDefaultFlatGenerator();
		}
	}
}

FlatGeneratorInfo FlatGeneratorInfo::getDefaultFlatGenerator()
{
	FlatGeneratorInfo flatgeneratorinfo;
	flatgeneratorinfo.setBiome(Biome::getIdForBiome(Biomes::PLAINS));
	flatgeneratorinfo.getFlatLayers().emplace_back(FlatLayerInfo(1, Blocks::BEDROCK));
	flatgeneratorinfo.getFlatLayers().emplace_back(FlatLayerInfo(2, Blocks::DIRT));
	flatgeneratorinfo.getFlatLayers().emplace_back(FlatLayerInfo(1, Blocks::GRASS));
	flatgeneratorinfo.updateLayers();
	flatgeneratorinfo.getWorldFeatures().emplace("village", std::unordered_map<std::string, std::string>());
	return flatgeneratorinfo;
}

FlatLayerInfo FlatGeneratorInfo::getLayerFromString(int32_t p_180715_0_, std::string_view p_180715_1_, int32_t p_180715_2_)
{
	auto astring = p_180715_0_ >= 3 ? Util::split(p_180715_1_,"\\*") : Util::split(p_180715_1_,"x");
	auto i = 1;
	auto j = 0;
	if (astring.size() == 2) 
	{
		i = std::stoi(astring[0]);
		if (p_180715_2_ + i >= 256) 
		{
			i = 256 - p_180715_2_;
		}

		if (i < 0) 
		{
			i = 0;
		}
	}

	Block* block = nullptr;
	auto s = astring[astring.size() - 1];
	if (p_180715_0_ < 3) 
	{
		astring = Util::split(s, ":");
		if (astring.size() > 1) 
		{
			j = std::stoi(astring[1]);
		}

		block = Block::getBlockById(std::stoi(astring[0]));
	}
	else 
	{
		astring = Util::split(s, ":");
		block = astring.size() > 1 ? Block::getBlockFromName(astring[0] + ":" + astring[1]) : nullptr;
		if (block != nullptr) 
		{
			j = astring.size() > 2 ? std::stoi(astring[2]) : 0;
		}
		else {
			block = Block::getBlockFromName(astring[0]);
			if (block != nullptr) 
			{
				j = astring.size() > 1 ? std::stoi(astring[1]) : 0;
			}
		}

		if (block == nullptr) 
		{
			return nullptr;
		}
	}

	if (block == Blocks::AIR) 
	{
		j = 0;
	}

	if (j < 0 || j > 15) 
	{
		j = 0;
	}

	FlatLayerInfo flatlayerinfo(p_180715_0_, i, block, j);
	flatlayerinfo.setMinY(p_180715_2_);
	return flatlayerinfo;
}

std::vector<FlatLayerInfo> FlatGeneratorInfo::getLayersFromString(int32_t p_180716_0_, std::string_view p_180716_1_)
{
	if (p_180716_1_ != nullptr && p_180716_1_.length() >= 1) 
	{
		std::vector<FlatLayerInfo>() liste;

		auto astring = Util::split(p_180716_1_,",");
		auto i = 0;

		for(auto& s : astring)
		{
			FlatLayerInfo flatlayerinfo = getLayerFromString(p_180716_0_, s, i);
			if (flatlayerinfo == nullptr) 
			{
				return std::vector<FlatLayerInfo>();
			}

			liste.emplace_back(flatlayerinfo);
			i += flatlayerinfo.getLayerCount();
		}

		return liste;
	}
	else {
		return std::vector<FlatLayerInfo>();
	}
}

#include "SaveHandler.h"
#include <fstream>
#include "MinecraftException.h"
#include "WorldProvider.h"
#include "SaveFormatOld.h"
#include "CompressedStreamTools.h"
#include "datafix/FixTypes.h"

std::shared_ptr<spdlog::logger> BlockAnvil::LOGGER = spdlog::get("Minecraft")->clone("SaveHandler");

SaveHandler::SaveHandler(std::filesystem::path p_i46648_1_, std::string saveDirectoryNameIn, bool p_i46648_3_,
	DataFixer dataFixerIn)
	:dataFixer(dataFixerIn)
{
	worldDirectory = p_i46648_1_.append(saveDirectoryNameIn);
	create_directory(worldDirectory);
	playersDirectory = worldDirectory;
	playersDirectory.append("playerdata");
	mapDataDir = worldDirectory;
	mapDataDir.append("data");
	create_directory(mapDataDir);
	saveDirectoryName = saveDirectoryNameIn;
	if (p_i46648_3_) 
	{
		create_directory(playersDirectory);
		auto temp = worldDirectory;
		temp.append("structures");
		structureTemplateManager = TemplateManager(temp.string(), dataFixerIn);
	}
	else 
	{
		structureTemplateManager = std::nullopt;
	}

	setSessionLock();
}

std::filesystem::path SaveHandler::getWorldDirectory()
{
	return worldDirectory;
}

void SaveHandler::checkSessionLock()
{
	try 
	{
		auto file1 = worldDirectory;
		file1.append("session.lock");
		std::ofstream datainputstream(file1);

		if (datainputstream.readLong() != this.initializationTime) {
			throw new MinecraftException("The save is being accessed from another location, aborting");
		}
	}
	catch (std::exception& var7)
	{
		throw MinecraftException("Failed to check session lock, aborting");
	}
}

void SaveHandler::setSessionLock() const
{
	try 
	{
		auto file1 = worldDirectory;
		file1.append("session.lock");
		std::ofstream dataoutputstream(file1);
		dataoutputstream.write((const char *)&initializationTime,sizeof(initializationTime));
	}
	catch (std::exception& var7) 
	{
		var7.printStackTrace();
		throw std::runtime_error("Failed to check session lock, aborting");
	}
}

IChunkLoader* SaveHandler::getChunkLoader(WorldProvider* provider)
{
	throw std::runtime_error("Old Chunk Storage is no longer supported.");
}

std::optional<WorldInfo> SaveHandler::loadWorldInfo()
{
	auto file1 = worldDirectory;
	file1.append("level.dat");
	if (exists(file1)) 
	{
		auto worldinfo = SaveFormatOld::getWorldData(file1, dataFixer);
		if (worldinfo != std::nullopt) 
		{
			return worldinfo;
		}
	}

	file1 = worldDirectory;
	file1.append("level.dat_old");
	return exists(file1) ? SaveFormatOld::getWorldData(file1, dataFixer) : std::nullopt;
}

void SaveHandler::saveWorldInfoWithPlayer(WorldInfo worldInformation, NBTTagCompound* tagCompound)
{
	auto nbttagcompound = worldInformation.cloneNBTCompound(tagCompound);
	auto nbttagcompound1 = std::make_unique<NBTTagCompound>();
	nbttagcompound1->setTag("Data", nbttagcompound);

	try 
	{
		auto file1 = worldDirectory;
		file1.append("level.dat_new");
		auto file2 = worldDirectory;
		file2.append("level.dat_old");
		auto file3 = worldDirectory;
		file3.append("level.dat");
		std::ofstream outputfile(file1);
		CompressedStreamTools::writeCompressed(nbttagcompound1, outputfile);
		if (exists(file2)) 
		{
			remove(file2);
		}
		rename(file3, file2);
		if (exists(file3))
		{
			remove(file3);
		}

		rename(file1, file3);
		if (exists(file1)) 
		{
			remove(file1);
		}
	}
	catch (std::exception& var8) 
	{
		var8.printStackTrace();
	}
}

void SaveHandler::saveWorldInfo(WorldInfo worldInformation)
{
	saveWorldInfoWithPlayer(worldInformation, nullptr);
}

void SaveHandler::writePlayerData(EntityPlayer* player)
{
	try 
	{
		auto nbttagcompound = player->writeToNBT(std::make_unique<NBTTagCompound>());
		auto file1 = playersDirectory;
		file1.append(player->getCachedUniqueIdString() + ".dat.tmp");
		auto file2 = playersDirectory;
		file1.append(player->getCachedUniqueIdString() + ".dat");
		std::ofstream outout(file1);
		CompressedStreamTools::writeCompressed(nbttagcompound, outout);
		if (exists(file2)) 
		{
			remove(file2);
		}

		rename(file1, file2);
	}
	catch (std::exception& var5) 
	{
		LOGGER->warn("Failed to save player data for {}", player->getName());
	}
}

std::unique_ptr<NBTTagCompound> SaveHandler::readPlayerData(EntityPlayer* player)
{
	std::unique_ptr<NBTTagCompound> nbttagcompound = nullptr;

	try 
	{
		auto file1 = playersDirectory;
		file1.append(player->getCachedUniqueIdString() + ".dat");
		if (exists(file1) && is_regular_file(file1)) 
		{
			std::ifstream outout(file1);
			nbttagcompound = CompressedStreamTools::readCompressed(outout);
		}
	}
	catch (std::exception& var4) 
	{
		LOGGER->warn("Failed to load player data for {}", player->getName());
	}

	if (nbttagcompound != nullptr) 
	{
		player->readFromNBT(dataFixer.process(FixTypes::PLAYER, nbttagcompound));
	}

	return nbttagcompound;
}

IPlayerFileData* SaveHandler::getPlayerNBTManager()
{
	return this;
}

std::vector<std::string> SaveHandler::getAvailablePlayerDat()
{
	std::vector<std::string> PlayerDat;
	for (auto& astring : std::filesystem::directory_iterator("sandbox"))
	{
		auto str = astring.path().string();
		if (Util::endsWith(str,".dat"))
		{
			PlayerDat.emplace_back(str.substr(0, str.size() - 4));
		}
	}

	return PlayerDat;
}

void SaveHandler::flush()
{
}

std::filesystem::path SaveHandler::getMapFileFromName(std::string_view mapName)
{
	std::filesystem::path file = mapDataDir;
	file.append(std::string(mapName) + ".dat");
	return file;
}

TemplateManager SaveHandler::getStructureTemplateManager()
{
	return structureTemplateManager;
}
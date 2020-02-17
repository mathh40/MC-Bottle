#include "SaveFormatOld.h"
#include "World.h"
#include "datafix/FixTypes.h"
#include "CompressedStreamTools.h"
#include <fstream>

std::shared_ptr<spdlog::logger> BlockAnvil::LOGGER = spdlog::get("Minecraft")->clone("SaveFormatOld");

SaveFormatOld::SaveFormatOld(std::filesystem::path savesDirectoryIn, DataFixer dataFixerIn)
	:dataFixer(dataFixerIn)
{
	if (!exists(savesDirectoryIn))
	{
		create_directory(savesDirectoryIn);
	}
	savesDirectory = savesDirectoryIn;
}

std::string SaveFormatOld::getName()
{
	return "Old Format";
}

std::vector<WorldSummary> SaveFormatOld::getSaveList()
{
	std::vector<WorldSummary> list;

	for (auto i = 0; i < 5; ++i) 
	{
		std::string s = "World" + (i + 1);
		auto worldinfo = getWorldInfo(s);
		if (worldinfo.has_value()) 
		{
			list.emplace_back(WorldSummary(worldinfo, s, "", worldinfo.value().getSizeOnDisk(), false));
		}
	}

	return list;
}

void SaveFormatOld::flushCache()
{
}

std::optional<WorldInfo> SaveFormatOld::getWorldInfo(std::string_view saveName)
{
	auto file1 = savesDirectory.append(saveName);
	if (!exists(file1))
	{
		return std::nullopt;
	}
	else 
	{
		auto file2 = file1.append("level.dat");
		if (exists(file2)) 
		{
			auto worldinfo = getWorldData(file2, dataFixer);
			if (worldinfo.has_value()) 
			{
				return worldinfo;
			}
		}

		file2 = file1.append("level.dat_old");
		return exists(file2) ? getWorldData(file2, dataFixer) : std::nullopt;
	}
}

std::optional<WorldInfo> SaveFormatOld::getWorldData(std::filesystem::path p_186353_0_, DataFixer dataFixerIn)
{
	try 
	{
		std::ifstream inputfile(p_186353_0_);
		auto nbttagcompound = CompressedStreamTools::readCompressed(inputfile);
		auto nbttagcompound1 = nbttagcompound->getCompoundTag("Data");
		return WorldInfo(dataFixerIn.process(FixTypes::LEVEL, nbttagcompound1));
	}
	catch (std::exception& var4) 
	{
		LOGGER->error("Exception reading {}", p_186353_0_, var4);
		return std::nullopt;
	}
}

void SaveFormatOld::renameWorld(std::string_view dirName, std::string_view newName)
{
	auto file1 = savesDirectory.append(dirName);
	if (exists(file1))
	{
		auto file2 = file1.append("level.dat");
		if (exists(file2)) 
		{
			try 
			{
				std::ifstream inputfile(file2);
				auto nbttagcompound = CompressedStreamTools::readCompressed(inputfile);
				auto nbttagcompound1 = nbttagcompound->getCompoundTag("Data");
				nbttagcompound1->setString("LevelName", newName);
				std::ofstream outputfile(file2);
				CompressedStreamTools::writeCompressed(nbttagcompound, outputfile);
			}
			catch (std::exception& var7)
			{
				var7.printStackTrace();
			}
		}
	}
}

bool SaveFormatOld::isNewLevelIdAcceptable(std::string_view saveName)
{
	auto file1 = savesDirectory.append(saveName);
	if (exists(file1)) 
	{
		return false;
	}
	else 
	{
		try 
		{
			create_directory(file1);
			remove(file1);
			return true;
		}
		catch (std::exception& var4)
		{
			LOGGER->warn("Couldn't make new level", var4);
			return false;
		}
	}
}

bool SaveFormatOld::deleteWorldDirectory(std::string_view saveName)
{
	auto file1 = savesDirectory.append(saveName);
	if (!exists(file1)) 
	{
		return true;
	}
	else 
	{
		LOGGER->info("Deleting level {}", saveName);

		for (auto i = 1; i <= 5; ++i) 
		{
			LOGGER->info("Attempt {}...", i);
			if (deleteFiles(file1.parent_path())) 
			{
				break;
			}

			LOGGER->warn("Unsuccessful in deleting contents.");
			if (i < 5) 
			{
				try 
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
				catch (std::exception& var5) 
				{
				}
			}
		}

		return remove(file1);
	}
}

ISaveHandler* SaveFormatOld::getSaveLoader(std::string_view saveName, bool storePlayerdata)
{
	return SaveHandler(savesDirectory, saveName, storePlayerdata, dataFixer);
}

bool SaveFormatOld::isConvertible(std::string_view saveName)
{
	return false;
}

bool SaveFormatOld::isOldMapFormat(std::string_view saveName)
{
	return false;
}

bool SaveFormatOld::convertMapFormat(std::string_view filename, IProgressUpdate* progressCallback)
{
	return false;
}

bool SaveFormatOld::canLoadWorld(std::string_view saveName)
{
	auto file1 = savesDirectory.append(saveName);
	return std::filesystem::is_directory(file1);
}

bool SaveFormatOld::deleteFiles(std::filesystem::path files) const
{
	for (const auto& file1 : std::filesystem::directory_iterator(files))
	{
		LOGGER->debug("Deleting {}", file1);
		if (std::filesystem::is_directory(file1) && !deleteFiles(file1.path())) 
		{
			LOGGER->warn("Couldn't delete directory {}", file1);
			return false;
		}

		if (!remove(file1)) 
		{
			LOGGER->warn("Couldn't delete file {}", file1);
			return false;
		}
	}

	return true;
}

std::filesystem::path SaveFormatOld::getFile(std::string_view p_186352_1_, std::string_view p_186352_2_)
{
	return savesDirectory.append(p_186352_1_).append(p_186352_2_);
}

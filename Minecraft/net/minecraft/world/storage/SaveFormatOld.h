#pragma once
#include "ISaveFormat.h"
#include "datafix/DataFixer.h"

class SaveFormatOld : public ISaveFormat
{
public:
	SaveFormatOld(std::filesystem::path savesDirectoryIn, DataFixer dataFixerIn);
	std::string getName() override;
	std::vector<WorldSummary> getSaveList() override;
	void flushCache() override;
	std::optional<WorldInfo> getWorldInfo(std::string_view saveName) override;
	static std::optional<WorldInfo> getWorldData(std::filesystem::path p_186353_0_, DataFixer dataFixerIn);
	void renameWorld(std::string_view dirName, std::string_view newName) override;
	bool isNewLevelIdAcceptable(std::string_view saveName) override;
	bool deleteWorldDirectory(std::string_view saveName) override;
	ISaveHandler* getSaveLoader(std::string_view saveName, bool storePlayerdata) override;
	bool isConvertible(std::string_view saveName) override;
	bool isOldMapFormat(std::string_view saveName) override;
	bool convertMapFormat(std::string_view filename, IProgressUpdate* progressCallback) override;
	bool canLoadWorld(std::string_view saveName) override;
	std::filesystem::path getFile(std::string_view p_186352_1_, std::string_view p_186352_2_) override;
protected:
	std::filesystem::path savesDirectory;
	DataFixer dataFixer;

	bool deleteFiles(std::filesystem::path files) const;
private:
	static std::shared_ptr<spdlog::logger> LOGGER;
};

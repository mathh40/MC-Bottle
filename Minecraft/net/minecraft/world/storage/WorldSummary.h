#include "GameType.h"

class WorldInfo;

class WorldSummary
{
public:
	WorldSummary(WorldInfo info, std::string fileNameIn, std::string displayNameIn, int64_t sizeOnDiskIn, bool requiresConversionIn);
	std::string getFileName() const;
	std::string getDisplayName() const;
	int64_t getSizeOnDisk() const;
	bool requiresConversion() const;
	int64_t getLastTimePlayed() const;
	friend bool operator<(const WorldSummary& lhs, const WorldSummary& rhs);
	GameType getEnumGameType() const;
	bool isHardcoreModeEnabled() const;
	bool getCheatsEnabled() const;
	std::string getVersionName() const;
	bool markVersionInList() const;
	bool askToOpenWorld() const;


private:
	std::string fileName;
	std::string displayName;
	int64_t lastTimePlayed;
	int64_t sizeOnDisk;
	bool brequiresConversion;
	GameType gameType;
	bool hardcore;
	bool cheatsEnabled;
	std::string versionName;
	int versionId;
	bool versionSnapshot;
};

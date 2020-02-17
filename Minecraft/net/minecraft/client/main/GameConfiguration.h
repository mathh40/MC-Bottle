#pragma once
#include "../../util/Session.h"
#include <filesystem>
#include <optional>

struct GameConfiguration
{
	struct UserInformation
	{
		const Session session;
		const PropertyMap userProperties;
		const PropertyMap profileProperties;
		//const Proxy proxy;

		UserInformation(Session sessionIn, PropertyMap userPropertiesIn, PropertyMap profilePropertiesIn);
	};

	struct DisplayInformation
	{
		const uint32_t width;
		const uint32_t height;
		const bool fullscreen;
		const bool checkGlErrors;

		DisplayInformation(uint32_t widthIn, uint32_t heightIn, bool fullscreenIn, bool checkGlErrorsIn);
	};

	struct FolderInformation
	{
		const std::filesystem::path gameDir;
		const std::filesystem::path resourcePacksDir;
		const std::filesystem::path assetsDir;
		const std::optional <std::string> assetIndex;

		FolderInformation(std::filesystem::path mcDataDirIn, std::filesystem::path resourcePacksDirIn, std::filesystem::path assetsDirIn, std::optional<std::string> assetIndexIn);
		ResourceIndex getAssetsIndex();
	};

	struct GameInformation
	{
		const bool isDemo;
		const std::string version;
		const std::string versionType;

		GameInformation(bool demo, std::string versionIn, std::string versionTypeIn);
	};

	struct ServerInformation
	{
		const std::string serverName;
		const std::uint32_t serverPort;

		ServerInformation(std::string serverNameIn, std::uint32_t serverPortIn);
	};

	const UserInformation userInfo;
	const DisplayInformation displayInfo;
	const FolderInformation folderInfo;
	const GameInformation gameInfo;
	const ServerInformation serverInfo;

	GameConfiguration(UserInformation userInfoIn, DisplayInformation displayInfoIn, FolderInformation folderInfoIn, GameInformation gameInfoIn, ServerInformation serverInfoIn);
};

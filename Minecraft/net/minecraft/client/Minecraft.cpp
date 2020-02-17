#include "Minecraft.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"
#include "main/GameConfiguration.h"
#include "text/TextComponentKeybind.h"
#include "registry/RegistryDefaulted.h"

Minecraft::Minecraft(const GameConfiguration & gameConfig)
	:gameDir(gameConfig.folderInfo.gameDir), fileAssets(gameConfig.folderInfo.assetsDir),
fileResourcepacks(gameConfig.folderInfo.resourcePacksDir), launchedVersion(gameConfig.gameInfo.version),
versionType(gameConfig.gameInfo.versionType), twitchDetails(gameConfig.userInfo.userProperties),
profileProperties(gameConfig.userInfo.profileProperties), defaultResourcePack(gameConfig.folderInfo.getAssetsIndex()),
//sessionService(YggdrasilAuthenticationService(this.proxy, UUID.randomUUID().toString())).createMinecraftSessionService()),
session(gameConfig.userInfo.session), isDemo(gameConfig.gameInfo.isDemo), displayWidth(gameConfig.displayInfo.width > 0 ? gameConfig.displayInfo.width : 640),
displayHeight(gameConfig.displayInfo.height > 0 ? gameConfig.displayInfo.height : 480),
tempDisplayWidth(gameConfig.displayInfo.width), tempDisplayHeight(gameConfig.displayInfo.height),
fullscreen(gameConfig.displayInfo.fullscreen), integratedServer(std::nullopt), dataFixer(DataFixesManager.createFixer()),
toastGui(this), tutorial(this)
{
	std::vector<spdlog::sink_ptr> sinks;

	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logfile"));

	LOGGER = std::make_shared<spdlog::async_logger>("Minecraft", begin(sinks), end(sinks), spdlog::thread_pool());
	LOGGER->flush_on(spdlog::level::err);

	LOGGER->info("Setting user: {}", session.getUsername());
	LOGGER->debug("(Session ID is {})", session.getSessionID());

	if (!gameConfig.serverInfo.serverName.empty()) {
		serverName = gameConfig.serverInfo.serverName;
		serverPort = gameConfig.serverInfo.serverPort;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	initEnumFacing();

	Bootstrap.register();
	TextComponentKeybind.displaySupplierFunction = KeyBinding::getDisplayString;
}

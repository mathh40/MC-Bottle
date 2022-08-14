#pragma once
#include <filesystem>
#include <optional>
#include "../util/Session.h"
#include <queue>
#include "../util/FrameTimer.h"
#include "../util/MouseHelper.h"
#include "ResourceLocation.h"
#include "Timer.h"
#include "math/RayTraceResult.h"
#include "datafix/DataFixer.h"
#include "../world/storage/ISaveFormat.h"

struct GameConfiguration;

class Minecraft
{
public:
	PlayerControllerMP playerController;
	uint32_t displayWidth;
	uint32_t displayHeight;
	WorldClient world;
	RenderGlobal renderGlobal;
	EntityPlayerSP player;
	Entity pointedEntity;
	ParticleManager effectRenderer;
	FontRenderer fontRenderer;
	FontRenderer standardGalacticFontRenderer;
	std::optional<GuiScreen> currentScreen;
	LoadingScreenRenderer loadingScreen;
	EntityRenderer entityRenderer;
	DebugRenderer debugRenderer;
	GuiIngame ingameGUI;
	bool skipRenderWorld;
	RayTraceResult objectMouseOver;
	GameSettings gameSettings;
	CreativeSettings creativeSettings;
	MouseHelper mouseHelper;
	std::filesystem::path gameDir;
	bool inGameHasFocus;
	FrameTimer frameTimer;
	Profiler profiler;
	std::string debug = "";
	bool renderChunksMany = true;

	Minecraft(const GameConfiguration &gameConfig);
private:
	std::shared_ptr<spdlog::async_logger> LOGGER;
	const ResourceLocation LOCATION_MOJANG_PNG = ResourceLocation("textures/gui/title/mojang.png");
	bool IS_RUNNING_ON_MAC;
	std::vector<std::uint8_t> memoryReserve;
	std::list<> MAC_DISPLAY_MODES;
	std::filesystem::path fileResourcepacks;
	PropertyMap twitchDetails;
	PropertyMap profileProperties;
	ServerData currentServerData;
	TextureManager renderEngine;
	DataFixer dataFixer;
	bool fullscreen;
	bool enableGLErrorChecking = true;
	bool hasCrashed;
	CrashReport crashReporter;
	bool connectedToRealms;
	Timer timer = Timer(20.0f);
	Snooper usageSnooper = Snooper("client", this, MinecraftServer.getCurrentTimeMillis());
	RenderManager renderManager;
	RenderItem renderItem;
	ItemRenderer itemRenderer;
	std::optional<Entity> renderViewEntity;
	SearchTreeManager searchTreeManager;
	Session session;
	bool isGamePaused;
	float renderPartialTicksPaused;
	uint64_t leftClickCounter;
	uint32_t tempDisplayWidth;
	uint32_t tempDisplayHeight;
	std::optional<IntegratedServer> integratedServer;
	std::filesystem::path fileAssets;
	std::string launchedVersion;
	std::string versionType;
	//Proxy proxy;
	ISaveFormat saveLoader;
	uint64_t debugFPS;
	uint64_t rightClickDelayTimer;
	std::string serverName;
	uint32_t serverPort;
	std::chrono::system_clock::time_point systemTime = std::chrono::system_clock::now();
	uint32_t joinPlayerCounter;
	uint64_t startNanoTime;
	bool jvm64bit;
	bool isDemo;
	std::optional<NetworkManager> networkManager;
	bool integratedServerIsRunning;
	int64_t debugCrashKeyPressTime = -1;
	IReloadableResourceManager resourceManager;
	MetadataSerializer metadataSerializer;
	std::list<std::string> defaultResourcePacks;
	DefaultResourcePack defaultResourcePack;
	ResourcePackRepository resourcePackRepository;
	LanguageManager languageManager;
	BlockColors blockColors;
	ItemColors itemColors;
	Framebuffer framebuffer;
	TextureMap textureMapBlocks;
	SoundHandler soundHandler;
	MusicTicker musicTicker;
	ResourceLocation mojangLogo;
	MinecraftSessionService sessionService;
	SkinManager skinManager;
	std::queue<std::thread> scheduledTasks;
	ModelManager modelManager;
	BlockRendererDispatcher blockRenderDispatcher;
	GuiToast toastGui;
	std::atomic_bool running = true;
	std::chrono::system_clock::time_point debugUpdateTime = std::chrono::system_clock::now();
	uint64_t fpsCounter;
	bool actionKeyF3;
	Tutorial tutorial;
	int64_t prevFrameTime = -1;
	std::string debugProfilerName = "root";
};

#include "../args/args.hxx"
#include "ThreadName.h"
#include "net/minecraft/client/Minecraft.h"
#include "net/minecraft/client/main/GameConfiguration.h"
#include "net/minecraft/util/Session.h"

#include <filesystem>
#include <internal_use_only/config.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char **argv)
{
    args::ArgumentParser parser("Minecraft.", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "This group is all exclusive:", args::Group::Validators::Xor);
    args::Flag demo(group, "demo", "Start Demo", {"demo"});
    args::Flag fullscreen(group, "fullscreen", "Fullscreen", {"fullscreen"});
    args::Flag checkGlErrors(group, "checkGlErrors", "Check GlErrors", {"checkGlErrors"});
    args::ValueFlag<std::string> server(parser, "server", "Server", {"server"});
    args::ValueFlag<uint32_t> port(parser, "port", "Port", {"port"}, 25565);
    args::ValueFlag<std::string> gameDir(parser, "gameDir", "Game Direction", {"gameDir"}, ".");
    args::ValueFlag<std::string> assetsDir(parser, "assetsDir", "Assets Direction", {"assetsDir"});
    args::ValueFlag<std::string> resourcePackDir(parser, "resourcePackDir", "Resource Pack Direction",
                                                 {"resourcePackDir"});
    args::ValueFlag<std::string> proxyHost(parser, "proxyHost", "Proxy Host", {"proxyHost"});
    args::ValueFlag<uint32_t> proxyPort(parser, "proxyPort", "Proxy Port", {"proxyPort"}, 8080);
    args::ValueFlag<std::string> proxyUser(parser, "proxyUser", "Proxy User", {"proxyUser"});
    args::ValueFlag<std::string> proxyPass(parser, "proxyPass", "Proxy Pass", {"proxyPass"});
    args::ValueFlag<std::string> username(parser, "username", "Username", {"username"}, "Player");
    args::ValueFlag<std::string> uuid(parser, "uuid", "UUID", {"uuid"});
    args::ValueFlag<std::string> accessToken(parser, "accessToken", "access Token", {"accessToken"});
    args::ValueFlag<std::string> version(parser, "version", "Version", {"version"});
    args::ValueFlag<uint32_t> width(parser, "width", "Width", {"width"}, 854);
    args::ValueFlag<uint32_t> height(parser, "height", "Height", {"height"}, width);
    args::ValueFlag<std::string> userProperties(parser, "userProperties", "user Properties", {"userProperties"}, "{}");
    args::ValueFlag<std::string> profileProperties(parser, "profileProperties", "profile Properties",
                                                   {"profileProperties"}, "{}");
    args::ValueFlag<std::string> assetIndex(parser, "assetIndex", "Asset Index", {"assetIndex"});
    args::ValueFlag<std::string> userType(parser, "userType", "User Type", {"userType"}, "legacy");
    args::ValueFlag<std::string> versionType(parser, "versionType", "Version Type", {"versionType"}, "release");

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion &e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help &)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    auto propertymap               = json::parse(userProperties.Get()).get<PropertyMap>();
    auto propertymap1              = json::parse(profileProperties.Get()).get<PropertyMap>();
    std::filesystem::path asset    = assetsDir.HasFlag() ? assetsDir.Get() : gameDir + "assets/";
    std::filesystem::path resource = resourcePackDir.HasFlag() ? resourcePackDir.Get() : gameDir + "resourcepacks/";
    auto u                         = uuid.HasFlag() ? uuid.Get() : username.Get();
    auto index                     = assetIndex.HasFlag() ? assetIndex.Get() : "";

    Session session                     = Session(username.Get(), u, accessToken.Get(), userType.Get());
    GameConfiguration gameconfiguration = GameConfiguration(
        GameConfiguration::UserInformation(session, propertymap, propertymap1),
        GameConfiguration::DisplayInformation(width.Get(), height.Get(), fullscreen.Get(), checkGlErrors.Get()),
        GameConfiguration::FolderInformation(gameDir.Get(), resourcePackDir.Get(), assetsDir.Get(), assetIndex.Get()),
        GameConfiguration::GameInformation(demo, version.Get(), versionType.Get()),
        GameConfiguration::ServerInformation(server.Get(), port.Get()));

    Minecraft clientTask(gameconfiguration);

    std::thread client([&]() { clientTask.run(); });

    setName(client, "Client thread");

    return 0;
}

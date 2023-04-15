//
// Created by RedbeanW on 10/30/2022.
//

#include "Plugin.h"
#include "Config.h"
#include "PacketHelper.h"

#include "llapi/EventAPI.h"

#include <filesystem>

using namespace std;
using namespace ll;
using json = nlohmann::json;
#define DATA_PATH "plugins/MovingLight/"

namespace fs = filesystem;

Logger logger("MovingLight");
Version PLUGIN_VERSION { 1,9,2,Version::Release };

void PluginMain()
{
    logger.info("loaded, ver " + PLUGIN_VERSION.toString() + ", author: redbeanw.");
}

void PluginInit()
{
    // Read config
    if (!fs::exists(DATA_PATH)) {
        logger.warn("Could not find the directory for the configuration file, creating...");
        fs::create_directory(DATA_PATH);
    }
    if (!fs::exists(DATA_PATH"config.json")) {
        logger.warn("Configuration file not found, creating...");
        ofstream ofile;
        ofile.open(DATA_PATH"config.json");
        ofile << config.toJson();
        ofile.close();
    }
    ifstream ifile;
    ifile.open(DATA_PATH"config.json");
    auto j = json::parse(ifile);
    config.fromJson(j);
    ifile.close();
    
    // Register plugin to LL.
    ll::registerPlugin("MovingLight", "The moving light.", PLUGIN_VERSION, { {"Author","RedbeanW"}, {"Github","https://github.com/Redbeanw44602/LiteLoaderMods.git"} });
    Event::ServerStartedEvent::subscribe([](Event::ServerStartedEvent ev) -> bool {
        PluginMain();
        new PacketHelper();
        return true;
    });

    Event::ConsoleCmdEvent::subscribe([](const Event::ConsoleCmdEvent& ev) -> bool {
        if (ev.mCommand == "stop")
            packetHelper.shutdown();
        return true;
    });

}
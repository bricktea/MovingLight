#include "Plugin.h"
#include "EventAPI.h"
#include <filesystem>

using namespace std;
using namespace LL;
using json = nlohmann::json;

namespace fs = filesystem;

Logger logger("MovingLight");
Version PLUGIN_VERSION { 1,8,0,Version::Release };

void PluginMain()
{
    logger.info("loaded, ver " + PLUGIN_VERSION.toString() + ", author: redbeanw.");
}

void PluginInit()
{
    // Read config
    if (!fs::exists("plugins/MovingLight/")) {
        logger.warn("Could not find the directory for the configuration file, creating...");
        fs::create_directory("plugins/MovingLight/");
    }
    if (!fs::exists("plugins/MovingLight/config.json")) {
        logger.warn("Configuration file not found, creating...");
        ofstream ofile;
        ofile.open("plugins/MovingLight/config.json");
        ofile << Config::to_json();
        ofile.close();
    }
    ifstream ifile;
    ifile.open("plugins/MovingLight/config.json");
    auto j = json::parse(ifile);
    Config::from_json(j);
    ifile.close();
    
    // Register plugin to LL.
    LL::registerPlugin("MovingLight", "The moving light.", PLUGIN_VERSION, { {"Author","RedbeanW"}, {"Github","https://github.com/Redbeanw44602/LiteLoaderMods.git"} });
    Event::ServerStartedEvent::subscribe([](Event::ServerStartedEvent ev) -> bool {
        PluginMain();
        return true;
    });

}
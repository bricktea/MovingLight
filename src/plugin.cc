#include <llapi/LoggerAPI.h>

#include <filesystem>

#include "config.h"
#include "light_manager.h"

#define DATA_PATH "plugins/MovingLight/"

Config config;
LightManager lightManager;

Logger logger("MovingLight");

ll::Version PLUGIN_VERSION{2, 1, 2, ll::Version::Release};

void PluginMain() {
  logger.info("loaded, ver " + PLUGIN_VERSION.toString() +
              ", author: redbeanw.");
}

void PluginInit() {
  if (!std::filesystem::exists(DATA_PATH)) {
    logger.warn(
        "Could not find the directory for the configuration file, creating...");
    std::filesystem::create_directory(DATA_PATH);
  }

  config.loadFromFile(DATA_PATH "config.json");

  // Register plugin to LL.

  ll::registerPlugin(
      "MovingLight", "Items always emit light~", PLUGIN_VERSION,
      {{"Author", "RedbeanW"},
       {"Github", "https://github.com/Redbeanw44602/MovingLight"}});

  Event::ServerStartedEvent::subscribe(
      [](Event::ServerStartedEvent ev) -> bool {
        PluginMain();
        return true;
      });
}

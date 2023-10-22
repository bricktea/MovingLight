#include <llapi/EventAPI.h>
#include <llapi/LoggerAPI.h>

#include <filesystem>

#include "config.h"
#include "light_manager.h"

#define DATA_PATH "plugins/MovingLight/"

Config config;
LightManager lightManager;

Logger logger("MovingLight");

ll::Version PLUGIN_VERSION{2, 3, 1, ll::Version::Release};

void PluginInit() {
  if (!std::filesystem::exists(DATA_PATH)) {
    logger.warn(
        "No configuration file found, creating a new one with default values.");
    std::filesystem::create_directory(DATA_PATH);
  }

  config.loadFromFile(DATA_PATH "config.json");

  // Register plugin to LL.

  ll::registerPlugin(
      "MovingLight",
      "Enabling you to hold a light source in your hand and move it around",
      PLUGIN_VERSION,
      {{"Author", "RedbeanW"},
       {"Github", "https://github.com/LiteLDev/MovingLight"}});
}

//
// Created by RedbeanW on 10/30/2022.
//

#include "Config.h"

#include "llapi/mc/BlockTypeRegistry.hpp"
#include "llapi/mc/Brightness.hpp"
#include "llapi/mc/Block.hpp"

Config config;

Config::Config() noexcept {
    Event::ServerStartedEvent::subscribe([&](Event::ServerStartedEvent ev) -> bool {
        // Basic load is completed.
        if (mAutoDiscoverGlowingBlock) {
            _computeLightBlocks();
        }
        return true;
    });
}

void Config::loadFromFile(const string &path) {
    mPath = path;
    if (!std::filesystem::exists(mPath)) {
        logger.warn("Configuration file not found, creating...");
        _save();
    }
    std::ifstream file;
    file.open(mPath);
    if (!file.is_open()) {
        logger.warn("Unable to open file, using default configuration.");
        return;
    }
    try {
        auto j = json::parse(file);
        _fromJson(j);
    } catch(json::exception& e) {
        logger.error("An exception occurred while reading the configuration file.");
        logger.error(e.what());
        logger.error("Preparing new configuration file, please wait...");
        _save();
    }
    file.close();
}

string Config::_toString() {
    json cfg = {
            {"version", mVersion},
            {"enabled", mEnabled},
            {"items", mItems},
            {"offhand", mOffHandItems},
            {"enableItemEntity", mEnableItemActor},
            {"enableAutoGlowingBlockDiscover", mAutoDiscoverGlowingBlock}
    };
    return cfg.dump(4);
}

bool Config::_fromJson(json &cfg) {
    _update(cfg);
    cfg.at("enabled").get_to(mEnabled);
    cfg.at("items").get_to(mItems);
    cfg.at("offhand").get_to(mOffHandItems);
    cfg.at("enableItemEntity").get_to(mEnableItemActor);
    cfg.at("enableAutoGlowingBlockDiscover").get_to(mAutoDiscoverGlowingBlock);
    return true;
}

void Config::_update(json& cfg) {
    if (!cfg.contains("version") || !cfg.at("version").is_number_integer()) {
        _save();
        return;
    }
    int version = cfg["version"];
    bool needSave = false;
    if (version < 200) {
        cfg["enableItemEntity"] = true;
        cfg["enableAutoGlowingBlockDiscover"] = true;
        cfg["version"] = 200;
        needSave = true;
    }
    if (needSave) {
        logger.warn("The configuration file has been updated to v{}.", mVersion);
        _save();
    }
}

void Config::_save() {
    std::ofstream ofile;
    ofile.open(mPath);
    ofile << _toString();
    ofile.close();
}

bool Config::isLightSource(const HashedString& name) {
    return mItems.find(name) != mItems.end();
}

bool Config::isOffhandItem(const HashedString& name) {
    return std::find(mOffHandItems.begin(), mOffHandItems.end(), name) != mOffHandItems.end();
}

unsigned int Config::getBrightness(const ItemStack* item) {
    if (item->isNull()) return 0;
    auto& name = item->getFullNameHash();
    return isLightSource(name) ? mItems[name] : 0;
}

void Config::_computeLightBlocks() {
    size_t beforeLightBlocks = mItems.size();
    BlockTypeRegistry::forEachBlock([&](const BlockLegacy& legacy) -> bool {
        auto& block = legacy.getRenderBlock();
        auto& typeName = block.getName();
        int light = block.getLightEmission().value;
        if (light > 0 && mItems.find(typeName) == mItems.end()) {
            mItems[typeName] = light;
        }
        return true;
    });
    size_t afterLightBlocks = mItems.size();
    if (afterLightBlocks - beforeLightBlocks > 0) {
        logger.warn("{} new glowing blocks have been discovered!", afterLightBlocks - beforeLightBlocks);
        _save();
    }
}

bool Config::isEnabled() const {
    return mEnabled;
}

bool Config::isItemActorEnabled() const {
    return mEnableItemActor;
}

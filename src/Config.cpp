//
// Created by RedbeanW on 10/30/2022.
//

#include "Config.h"

Config config;

bool Config::fromJson(json &cfg) {
    cfg.at("enabled").get_to(mEnabled);
    cfg.at("items").get_to(mItems);
    cfg.at("offhand").get_to(mOffHandItems);
    return true;
}

string Config::toJson() {
    json cfg = {
            {"version",mVersion},
            {"enabled",mEnabled},
            {"items",mItems},
            {"offhand",mOffHandItems}
    };
    return cfg.dump(4);
}

bool Config::isLightSource(const string &name) {
    return mItems.count(name);
}

bool Config::isOffhandItem(const string &name) {
    return count(mOffHandItems.begin(), mOffHandItems.end(), name);
}

unsigned int Config::getBrightness(const ItemStack *it) {
    if (it->isNull())
        return 0;
    auto name = it->getTypeName();
    return isLightSource(name) ? mItems[name] : 0;
}

bool Config::isEnabled() const {
    return mEnabled;
}

bool Config::isItemActorEnabled() const {
    return true;
}

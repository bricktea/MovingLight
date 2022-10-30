//
// Created by RedbeanW on 10/30/2022.
//

#include "Config.h"

bool Config::from_json(json &cfg) {
    cfg.at("enabled").get_to(enable);
    cfg.at("items").get_to(items);
    cfg.at("offhand").get_to(OffHandItems);
    return true;
}

string Config::to_json() {
    json cfg = {
            {"version",VERSION},
            {"enabled",enable},
            {"items",items},
            {"offhand",OffHandItems}
    };
    return cfg.dump(4);
}

bool Config::isLightSource(const string &name) {
    return items.count(name);
}

bool Config::isOffhandItem(const string &name) {
    return count(OffHandItems.begin(), OffHandItems.end(), name);
}

unsigned int Config::getBrightness(const ItemStack *it) {
    if (it->isNull())
        return 0;
    auto name = it->getTypeName();
    return isLightSource(name) ? Config::items[name] : 0;
}

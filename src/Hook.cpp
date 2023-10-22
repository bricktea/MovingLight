//
// Created by RedbeanW on 10/30/2022.
//

#include "Plugin.h"

#include "Config.h"
#include "LightMgr.h"

#include "llapi/mc/Player.hpp"

// Remove

TInstanceHook(ItemActor *, "??_EItemActor@@UEAAPEAXI@Z",
              ItemActor, char a2) {
    lightMgr.clear((identity_t)this);
    return original(this, a2);
}

TInstanceHook(void, "??1Player@@UEAA@XZ",
              Player) {
    lightMgr.clear((identity_t)this);
    original(this);
}

// Tick

TInstanceHook(void, "?normalTick@Player@@UEAAXXZ",
              Player) {
    original(this);
    if (!config.isEnabled() || !hasDimension())
        return;
    auto light = std::max(config.getBrightness(getSelectedItem()), config.getBrightness(getOffhandSlot()));
    if (light != 0) {
        lightMgr.turnOn((identity_t)this, getDimension(), getBlockPos(), light, isImmersedInWater());
    } else {
        lightMgr.turnOff((identity_t)this);
    }
}

TInstanceHook(void, "?postNormalTick@ItemActor@@QEAAXXZ",
              ItemActor) {
    original(this);
    if (!config.isEnabled() || !config.isItemActorEnabled() || !hasDimension()) return;
    auto item = getItemStack();
    if (!item) return;
    auto light = config.getBrightness(*item);
    if (light != 0) {
        lightMgr.turnOn((identity_t)this, getDimension(), getBlockPos(), light, isImmersedInWater());
    } else {
        lightMgr.turnOff((identity_t)this);
    }
}

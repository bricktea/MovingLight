//
// Created by RedbeanW on 10/30/2022.
//

#include "Plugin.h"

#include "Config.h"
#include "LightMgr.h"

#include "llapi/mc/Player.hpp"
#include "llapi/mc/Container.hpp"

// OffHand Helper

TClasslessInstanceHook(void, "?sendBlockDestructionStarted@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlockPos@@@Z",
                       Player* pl, BlockPos* bp) {
    original(this, pl, bp);
    if (!config.isEnabled())
        return;
    auto mainHand = &pl->getSelectedItem();
    if (mainHand->isNull() || !config.isOffhandItem(mainHand->getFullNameHash()))
        return;
    auto newHand = mainHand->clone_s();
    if (config.isLightSource(newHand->getFullNameHash()) && pl->getOffhandSlot().isNull()) {
        pl->getInventory().removeItem_s(pl->getSelectedItemSlot(), mainHand->getCount());
        pl->setOffhandSlot(*newHand);
        pl->sendInventory(mainHand);
    }
}

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
    auto light = std::max(config.getBrightness(&getSelectedItem()), config.getBrightness(&getOffhandSlot()));
    if (light != 0)
        lightMgr.turnOn((identity_t)this, &getDimension(), getBlockPos(), light);
    else
        lightMgr.turnOff((identity_t)this);
}

TInstanceHook(void, "?postNormalTick@ItemActor@@QEAAXXZ",
              ItemActor) {
    original(this);
    if (!config.isEnabled() || !config.isItemActorEnabled() || !hasDimension())
        return;
    auto light = config.getBrightness(getItemStack());
    if (light != 0)
        lightMgr.turnOn((identity_t)this, &getDimension(), getBlockPos(), light);
    else
        lightMgr.turnOff((identity_t)this);
}

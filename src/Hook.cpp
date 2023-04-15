//
// Created by RedbeanW on 10/30/2022.
//

#include "Plugin.h"

#include "Config.h"
#include "LightMgr.h"

// OffHand Helper

TClasslessInstanceHook(void, "?sendBlockDestructionStarted@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlockPos@@@Z",
                       Player * pl, BlockPos * bp)
{
    original(this, pl, bp);
    if (!config.isEnabled())
        return;
    auto mainhand = &pl->getSelectedItem();
    if (mainhand->isNull() || !config.isOffhandItem(mainhand->getTypeName()))
        return;
    auto newHand = mainhand->clone_s();
    if (config.isLightSource(newHand->getTypeName()) && pl->getOffhandSlot().isNull())
    {
        pl->getInventory().removeItem_s(pl->getSelectedItemSlot(),mainhand->getCount());
        pl->setOffhandSlot(*newHand);
        pl->sendInventory(true);
    }
}

// Remove

TInstanceHook(ItemActor *, "??_EItemActor@@UEAAPEAXI@Z",
              ItemActor, char a2)
{
    lightMgr.clear((identity_t)this);
    return original(this, a2);
}

TClasslessInstanceHook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
                       ServerPlayer * sp, char a3)
{
    lightMgr.clear((identity_t)sp);
    original(this, sp, a3);
}

// Tick

TInstanceHook(void, "?normalTick@Player@@UEAAXXZ",
              Player)
{
    original(this);
    if (!config.isEnabled() || !hasDimension())
        return;
    auto light = max(config.getBrightness(&getSelectedItem()), config.getBrightness(&getOffhandSlot()));
    if (light != 0)
        lightMgr.turnOn((identity_t)this, &getRegion(), getBlockPos(), light);
    else
        lightMgr.turnOff((identity_t)this);
}

TInstanceHook(void, "?postNormalTick@ItemActor@@QEAAXXZ",
              ItemActor)
{
    original(this);
    if (!config.isEnabled() || !config.isItemActorEnabled() || !hasDimension())
        return;
    auto light = config.getBrightness(getItemStack());
    if (light != 0)
        lightMgr.turnOn((identity_t)this, &getRegion(), getBlockPos(), light);
    else
        lightMgr.turnOff((identity_t)this);
}
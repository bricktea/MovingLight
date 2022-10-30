#include "Plugin.h"

#include "Config.h"
#include "PacketHelper.h"
#include "LightMgr.h"

// OffHand Helper

TClasslessInstanceHook(void, "?sendBlockDestructionStarted@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlockPos@@@Z",
                       Player * pl, BlockPos * bp)
{
    original(this, pl, bp);
    if (!Config::enable)
        return;
    auto mainhand = &pl->getSelectedItem();
    if (mainhand->isNull() || !Config::isOffhandItem(mainhand->getTypeName()))
        return;
    auto newHand = mainhand->clone_s();
    if (Config::isLightSource(newHand->getTypeName()) && pl->getOffhandSlot().isNull())
    {
        pl->getInventory().removeItem_s(pl->getSelectedItemSlot(),mainhand->getCount());
        pl->setOffhandSlot(*newHand);
        pl->sendInventory(true);
    }
}

// Remove

TClasslessInstanceHook(void, "?execute@StopCommand@@UEBAXAEBVCommandOrigin@@AEAVCommandOutput@@@Z",
                       CommandOrigin * a2, void* a3)
{
    PacketHelper::stopSending = true;
    original(this, a2, a3);
}

TInstanceHook(ItemActor *, "??_EItemActor@@UEAAPEAXI@Z",
              ItemActor, char a2)
{
    LightMgr::clear(this->getUniqueID());
    return original(this, a2);
}

TClasslessInstanceHook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
                       ServerPlayer * sp, char a3)
{
    LightMgr::clear(sp->getUniqueID());
    original(this, sp, a3);
}

// Tick

TInstanceHook(void, "?normalTick@Player@@UEAAXXZ",
              Player)
{
    original(this);
    if (!Config::enable || !this->isRegionValid())
        return;
    auto light = max(Config::getBrightness(&this->getSelectedItem()), Config::getBrightness(&this->getOffhandSlot()));
    auto& id = this->getUniqueID();
    if (light != 0)
        LightMgr::turnOn(id, &this->getRegion(), this->getBlockPos(), light);
    else
        LightMgr::turnOff(id);
}

TInstanceHook(void, "?normalTick@ItemActor@@UEAAXXZ",
              ItemActor)
{
    original(this);
    if (!Config::enable || !Config::enableItemActor || !this->isRegionValid())
        return;
    auto light = Config::getBrightness(this->getItemStack());
    auto& id = this->getUniqueID();
    if (light != 0)
        LightMgr::turnOn(id, &this->getRegion(), this->getBlockPos(), light);
    else
        LightMgr::turnOff(id);
}
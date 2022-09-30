#include "Plugin.h"

#include "llapi/mc/ItemStack.hpp"
#include "llapi/mc/ItemActor.hpp"
#include "llapi/mc/Container.hpp"
#include "llapi/mc/ServerPlayer.hpp"

// OffHand Helper

THook(void, "?sendBlockDestructionStarted@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlockPos@@@Z",
    void* self, Player* pl, BlockPos* bp)
{
    original(self, pl, bp);
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

THook(void, "?execute@StopCommand@@UEBAXAEBVCommandOrigin@@AEAVCommandOutput@@@Z",
    void* self, void* a2, void* a3)
{
    PacketHelper::stopSending = true;
    original(self, a2, a3);
}

THook(ItemActor*, "??_EItemActor@@UEAAPEAXI@Z",
    ItemActor* self, char a2)
{
    LightMgr::clear(self->getUniqueID());
    return original(self, a2);
}

THook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
    void* self, ServerPlayer* sp, bool broadcast)
{
    LightMgr::clear(sp->getUniqueID());
    original(self, sp, broadcast);
}

// Tick

THook(void, "?normalTick@Player@@UEAAXXZ",
    Player* pl)
{
    original(pl);
    if (!Config::enable || !pl->isRegionValid())
        return;
    int light = max(Config::getBrightness(&pl->getSelectedItem()), Config::getBrightness(&pl->getOffhandSlot()));
    auto& id = pl->getUniqueID();
    if (light != 0)
        LightMgr::turnOn(id, &pl->getRegion(), pl->getBlockPos(), light);
    else
        LightMgr::turnOff(id);
}

THook(void, "?normalTick@ItemActor@@UEAAXXZ",
    ItemActor* self)
{
    original(self);
    if (!Config::enable || !Config::enableItemActor || !self->isRegionValid())
        return;
    int light = Config::getBrightness(self->getItemStack());
    auto& id = self->getUniqueID();
    if (light != 0)
        LightMgr::turnOn(id, &self->getRegion(), self->getBlockPos(), light);
    else
        LightMgr::turnOff(id);
}
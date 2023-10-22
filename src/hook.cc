#include <llapi/HookAPI.h>

#include <llapi/mc/Block.hpp>
#include <llapi/mc/Player.hpp>
#include <llapi/mc/VanillaBlockTypeIds.hpp>

#include "config.h"
#include "light_manager.h"

extern Config config;
extern LightManager lightManager;

// Remove

TInstanceHook(ItemActor*, "??_EItemActor@@UEAAPEAXI@Z", ItemActor, char a2) {
  lightManager.clear((identity_t)this);
  return original(this, a2);
}

TInstanceHook(void, "??1Player@@UEAA@XZ", Player) {
  lightManager.clear((identity_t)this);
  original(this);
}

// Tick

TInstanceHook(void, "?normalTick@Player@@UEAAXXZ", Player) {
  original(this);
  if (!config.isEnabled() || !hasDimension()) return;
  auto light = std::max(config.getBrightness(getSelectedItem()),
                        config.getBrightness(getOffhandSlot()));
  if (light != 0) {
    lightManager.turnOn((identity_t)this, getDimension(), getBlockPos(), light,
                        isImmersedInWater());
  } else {
    lightManager.turnOff((identity_t)this);
  }
}

TInstanceHook(void, "?postNormalTick@ItemActor@@QEAAXXZ", ItemActor) {
  original(this);
  if (!config.isEnabled() || !config.isItemActorEnabled() || !hasDimension())
    return;
  auto item = getItemStack();
  if (!item) return;
  auto light = config.getBrightness(*item);
  if (light != 0) {
    lightManager.turnOn((identity_t)this, getDimension(), getBlockPos(), light,
                        isImmersedInWater());
  } else {
    lightManager.turnOff((identity_t)this);
  }
}

TClasslessInstanceHook(bool,
                       "?shouldStopFalling@TopSnowBlock@@UEBA_NAEAVActor@@@Z",
                       Actor* a2) {
  auto ret = original(this, a2);
  if (ret) {
    lightManager.markBadArea(a2->getRegion(), a2->getBlockPos());
  }
  return ret;
}

TInstanceHook(void, "?setRuntimeId@Block@@IEBAXAEBI@Z", Block,
              unsigned int* a2) {
  original(this, a2);
  if (VanillaBlockTypeIds::LightBlock == getName()) {
    lightManager.setLightBlockNetworkId(getTileData(), *a2);
  }
}

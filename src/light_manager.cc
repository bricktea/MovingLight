#include "light_manager.h"

#include <llapi/LoggerAPI.h>
#include <llapi/ScheduleAPI.h>

#include <llapi/mc/Container.hpp>
#include <llapi/mc/Dimension.hpp>
#include <llapi/mc/Level.hpp>
#include <llapi/mc/Material.hpp>
#include <llapi/mc/Player.hpp>
#include <llapi/mc/StaticVanillaBlocks.hpp>
#include <llapi/mc/UpdateBlockPacket.hpp>
#include <llapi/mc/VanillaBlockTypeIds.hpp>

#include "config.h"

extern Config config;

LightManager::LightManager() noexcept {
  Schedule::repeat([this] { _runBadAreaJanitor(); }, 20);
  Event::PlayerDestroyBlockEvent::subscribe(
      [](const Event::PlayerDestroyBlockEvent& ev) -> bool {
        if (!config.isEnabled()) {
          return true;
        }
        auto pl = ev.mPlayer;
        auto mainHand = pl->getSelectedItem();
        if (mainHand.isNull() ||
            !config.isOffhandItem(mainHand.getFullNameHash())) {
          return true;
        }
        auto newHand = mainHand.clone_s();
        if (config.isLightSource(newHand->getFullNameHash()) &&
            pl->getOffhandSlot().isNull()) {
          pl->getInventory().removeItem_s(pl->getSelectedItemSlot(),
                                          mainHand.getCount());
          pl->setOffhandSlot(*newHand);
          pl->sendInventory(mainHand);
          return false;
        }
        return true;
      });
  Event::ConsoleCmdEvent::subscribe(
      [this](const Event::ConsoleCmdEvent& ev) -> bool {
        if (ev.mCommand == "stop") {
          mStopPacketSending = true;
        }
        return true;
      });
}

bool LightManager::isValid(identity_t id) { return mRecordedInfo.contains(id); }

void LightManager::init(identity_t id) {
  LightInfo info;
  mRecordedInfo[id] = info;
}

bool LightManager::isTurningOn(identity_t id) {
  return isValid(id) && mRecordedInfo[id].mLighting;
}

void LightManager::turnOff(identity_t id) {
  if (!isTurningOn(id)) return;
  mRecordedInfo[id].mLighting = false;
  auto pos = mRecordedInfo[id].mPos;
  auto dim = Global<Level>->getDimension(mRecordedInfo[id].mDimId).get();
  if (dim) {
    auto& block = dim->getBlockSourceFromMainChunkSource().getBlock(pos);
    if (block == *StaticVanillaBlocks::mWater) {
      UpdateBlockPacket updateBlock(pos, 0, block.getRuntimeId(), 3);
      _sendPacket(dim, pos, updateBlock);
    } else {
      UpdateBlockPacket updateBlock(pos, 1, block.getRuntimeId(), 3);
      _sendPacket(dim, pos, updateBlock);
    }
  }
}

void LightManager::turnOn(identity_t id, Dimension& dim, BlockPos bp,
                          unsigned int lightLv, bool underWater) {
  if (underWater && !config.isUnderWaterEnabled()) return;
  if (!isValid(id)) init(id);
  auto& rec = mRecordedInfo[id];
  bool isOpened = isTurningOn(id);
  bp.y = bp.y + 1;
  bool isSamePos = bp == rec.mPos;
  bool isSameLight = lightLv == rec.mLevel;
  if (isOpened && isSamePos && isSameLight) return;

  auto& region = dim.getBlockSourceFromMainChunkSource();
  if (_isBadArea(region, bp)) return;

  auto& blk = region.getBlock(bp);
  if (underWater) {
    if (blk != *StaticVanillaBlocks::mWater) return;
    UpdateBlockPacket updateBlock(bp, 0, getLightBlockNetworkId(lightLv), 3);
    _sendPacket(&dim, bp, updateBlock);
  } else {
    if (std::find(mBannedBlocks.begin(), mBannedBlocks.end(), blk.getName()) !=
        mBannedBlocks.end())
      return;
    UpdateBlockPacket updateBlock(bp, 1, getLightBlockNetworkId(lightLv), 3);
    _sendPacket(&dim, bp, updateBlock);
  }

  if (!isSamePos && (isOpened || !isSameLight)) turnOff(id);

  rec.mLighting = true;
  rec.mDimId = dim.getDimensionId();
  rec.mPos = bp;
  rec.mLevel = lightLv;
}

void LightManager::clear(identity_t id) {
  turnOff(id);
  mRecordedInfo.erase(id);
}

void LightManager::markBadArea(BlockSource& region, const BlockPos& pos) {
  for (auto& i : mRecordedInfo) {
    if (i.second.mPos == pos) turnOff(i.first);
  }
  mBadAreaLocker.lock();
  mBadAreas[region.getDimensionId()][pos] =
      Global<Level>->getCurrentServerTick().t;
  mBadAreaLocker.unlock();
}

void LightManager::_runBadAreaJanitor() {
  auto current = Global<Level>->getCurrentServerTick().t;
  mBadAreaLocker.lock();
  for (auto& i : mBadAreas) {
    auto j = i.second.begin();
    while (j != i.second.end()) {
      if (j->second - current > 10) {
        i.second.erase(j++);
      }
      j++;
    }
  }
  mBadAreaLocker.unlock();
}

bool LightManager::_isBadArea(const BlockSource& region, const BlockPos& pos) {
  int dimId = region.getDimensionId();
  mBadAreaLocker.lock();
  bool ret = mBadAreas.contains(dimId) && mBadAreas[dimId].contains(pos);
  mBadAreaLocker.unlock();
  return ret;
}

void LightManager::setLightBlockNetworkId(unsigned short tileData,
                                          unsigned int networkId) {
  mLightBlockNetworkIdLookupMap[tileData] = networkId;
}

unsigned int LightManager::getLightBlockNetworkId(unsigned short tileData) {
  if (mLightBlockNetworkIdLookupMap.contains(tileData)) {
    return mLightBlockNetworkIdLookupMap[tileData];
  }
  return 0;
}

void LightManager::_sendPacket(Dimension* dim, const BlockPos& pos,
                               const Packet& pkt) {
  if (mStopPacketSending) return;
  dim->sendPacketForPosition(pos, pkt, nullptr);
}

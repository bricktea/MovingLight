//
// Created by RedbeanW on 10/30/2022.
//

#include "LightMgr.h"
#include "Config.h"

#include "llapi/ScheduleAPI.h"

#include "llapi/mc/Material.hpp"
#include "llapi/mc/StaticVanillaBlocks.hpp"
#include "llapi/mc/Level.hpp"
#include "llapi/mc/UpdateBlockPacket.hpp"
#include "llapi/mc/Dimension.hpp"
#include "llapi/mc/VanillaBlockTypeIds.hpp"

LightMgr lightMgr;

LightMgr::LightMgr() noexcept {
    Schedule::repeat([this] { _runBadAreaJanitor(); }, 20);
    Event::ConsoleCmdEvent::subscribe([this](const Event::ConsoleCmdEvent& ev) -> bool {
        if (ev.mCommand == "stop") {
            mStopPacketSending = true;
        }
        return true;
    });
}

bool LightMgr::isValid(identity_t id) {
    return mRecordedInfo.contains(id);
}

void LightMgr::init(identity_t id) {
    LightInfo info;
    mRecordedInfo[id] = info;
}

bool LightMgr::isTurningOn(identity_t id) {
    return isValid(id) && mRecordedInfo[id].mLighting;
}

void LightMgr::turnOff(identity_t id) {
    if (!isTurningOn(id))
        return;
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

void LightMgr::turnOn(identity_t id, Dimension& dim, BlockPos bp, unsigned int lightLv, bool underWater) {
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
        if (std::find(mBannedBlocks.begin(), mBannedBlocks.end(), blk.getName()) != mBannedBlocks.end()) return;
        UpdateBlockPacket updateBlock(bp, 1, getLightBlockNetworkId(lightLv), 3);
        _sendPacket(&dim, bp, updateBlock);
    }

    if (!isSamePos && (isOpened || !isSameLight)) turnOff(id);

    rec.mLighting = true;
    rec.mDimId = dim.getDimensionId();
    rec.mPos = bp;
    rec.mLevel = lightLv;

}

void LightMgr::clear(identity_t id) {
    turnOff(id);
    mRecordedInfo.erase(id);
}

void LightMgr::markBadArea(BlockSource &region, const BlockPos &pos) {
    for (auto& i : mRecordedInfo) {
        if (i.second.mPos == pos) turnOff(i.first);
    }
    mBadAreaLocker.lock();
    mBadAreas[region.getDimensionId()][pos] = Global<Level>->getCurrentServerTick().t;
    mBadAreaLocker.unlock();
}

void LightMgr::_runBadAreaJanitor() {
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

bool LightMgr::_isBadArea(const BlockSource &region, const BlockPos &pos) {
    int dimId = region.getDimensionId();
    mBadAreaLocker.lock();
    bool ret = mBadAreas.contains(dimId) && mBadAreas[dimId].contains(pos);
    mBadAreaLocker.unlock();
    return ret;
}

void LightMgr::setLightBlockNetworkId(unsigned short tileData, unsigned int networkId) {
    mLightBlockNetworkIdLookupMap[tileData] = networkId;
}

unsigned int LightMgr::getLightBlockNetworkId(unsigned short tileData) {
    if (mLightBlockNetworkIdLookupMap.contains(tileData)) {
        return mLightBlockNetworkIdLookupMap[tileData];
    }
    return 0;
}

void LightMgr::_sendPacket(Dimension* dim, const BlockPos &pos, const Packet& pkt) {
    if (mStopPacketSending) return;
    dim->sendPacketForPosition(pos, pkt, nullptr);
}

TClasslessInstanceHook(bool, "?shouldStopFalling@TopSnowBlock@@UEBA_NAEAVActor@@@Z",
                       Actor* a2) {
    auto ret = original(this, a2);
    if (ret) {
        lightMgr.markBadArea(a2->getRegion(), a2->getBlockPos());
    }
    return ret;
}

TInstanceHook(void, "?setRuntimeId@Block@@IEBAXAEBI@Z",
              Block, unsigned int* a2)
{
    original(this, a2);
    if (VanillaBlockTypeIds::LightBlock == getName()) {
        // logger.debug("tile = {}, networkId = {}", getTileData(), *a2);
        lightMgr.setLightBlockNetworkId(getTileData(), *a2);
    }
}
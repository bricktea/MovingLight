//
// Created by RedbeanW on 10/30/2022.
//

#include "LightMgr.h"
#include "PacketHelper.h"

#include "llapi/mc/Material.hpp"
#include "llapi/mc/StaticVanillaBlocks.hpp"
#include "llapi/mc/Level.hpp"

LightMgr lightMgr;

bool LightMgr::isVaild(identity_t id) {
    return mRecordedInfo.find(id) != mRecordedInfo.end();
}

void LightMgr::init(identity_t id) {
    LightInfo li;
    mRecordedInfo[id] = li;
}

bool LightMgr::isTurningOn(identity_t id) {
    return isVaild(id) && mRecordedInfo[id].mLighting;
}

void LightMgr::turnOff(identity_t id) {
    if (!isTurningOn(id))
        return;
    mRecordedInfo[id].mLighting = false;
    auto pos = mRecordedInfo[id].mPos;
    auto dim = Global<Level>->getDimension(mRecordedInfo[id].mDimId).get();
    if (dim) {
        packetHelper.UpdateBlockPacket(dim, pos, dim->getBlockSourceFromMainChunkSource().getBlock(pos).getRuntimeId(), BlockUpdateNoGraphics);
    }
}

void LightMgr::turnOn(identity_t id, Dimension* dim, BlockPos bp, unsigned int lightLv) {
    if (!dim) return;
    if (!isVaild(id)) init(id);
    auto& rec = mRecordedInfo[id];
    bool isOpened = isTurningOn(id);
    bp.y = bp.y + 1;
    bool isSamePos = bp == rec.mPos;
    bool isSameLight = lightLv == rec.mLevel;
    if (isOpened && isSamePos && isSameLight)
        return;

    auto& blk = dim->getBlockSourceFromMainChunkSource().getBlock(bp);
    if (std::find(mBannedBlocks.begin(), mBannedBlocks.end(), blk.getName()) != mBannedBlocks.end()) return;

    packetHelper.UpdateBlockPacket(dim, bp, StaticVanillaBlocks::mLightBlock->getRuntimeId() - 15 + lightLv, BlockUpdateNoGraphics);
    if (!isSamePos && (isOpened || !isSameLight))
        turnOff(id);

    rec.mLighting = true;
    rec.mDimId = dim->getDimensionId();
    rec.mPos = bp;
    rec.mLevel = lightLv;

}

void LightMgr::clear(identity_t id) {
    turnOff(id);
    mRecordedInfo.erase(id);
}

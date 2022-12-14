//
// Created by RedbeanW on 10/30/2022.
//

#include "LightMgr.h"
#include "PacketHelper.h"

bool LightMgr::isVaild(ActorUniqueID id) {
    return RecordedInfo.count(id);
}

void LightMgr::init(ActorUniqueID id) {
    LightInfo li;
    RecordedInfo[id] = li;
}

bool LightMgr::isTurningOn(ActorUniqueID id) {
    return isVaild(id) && RecordedInfo[id].mLighting;
}

void LightMgr::turnOff(ActorUniqueID id) {
    if (!isTurningOn(id))
        return;
    RecordedInfo[id].mLighting = false;
    auto pos = RecordedInfo[id].mPos;
    auto dim = (Dimension*)Global<Level>->getOrCreateDimension(RecordedInfo[id].mDimId).mHandle.lock().get();
    if (dim)
    {
        auto region = &dim->getBlockSourceFromMainChunkSource();
        Singleton<PacketHelper>->UpdateBlockPacket(dim, pos, region->getBlock(pos).getRuntimeId());
    }
}

void LightMgr::turnOn(ActorUniqueID id, BlockSource *region, BlockPos bp, unsigned int lightLv) {
    if (!region)
        return;
    if (!isVaild(id))
        init(id);
    auto& Info = RecordedInfo[id];
    bool isOpened = isTurningOn(id);
    bp.y = bp.y + 1;
    bool isSamePos = bp.operator==(Info.mPos);
    bool isSameLight = lightLv == Info.mLevel;
    if (isOpened && isSamePos && isSameLight)
        return;

    auto& name = region->getBlock(bp).getName().getString();
    if (count(bannedBlocks.begin(), bannedBlocks.end(), name))
        return;

    auto dimId = region->getDimensionId();
    Singleton<PacketHelper>->UpdateBlockPacket(dimId, bp, StaticVanillaBlocks::mLightBlock->getRuntimeId() - 15 + lightLv);
    if (!isSamePos && (isOpened || !isSameLight))
        turnOff(id);

    Info.mLighting = true;
    Info.mDimId = dimId;
    Info.mPos = bp;
    Info.mLevel = lightLv;

}

void LightMgr::clear(ActorUniqueID id) {
    turnOff(id);
    RecordedInfo.erase(id);
}

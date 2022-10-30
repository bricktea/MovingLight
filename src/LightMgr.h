//
// Created by RedbeanW on 10/30/2022.
//

#ifndef MOVINGLIGHT_LIGHTMGR_H
#define MOVINGLIGHT_LIGHTMGR_H

#include "Plugin.h"

namespace LightMgr {

    struct LightInfo {
        bool mLighting = false;
        unsigned int mLevel = 0;
        BlockPos mPos = BlockPos::ZERO;
        int mDimId = -1;
    };

    static unordered_map<UniqueID, LightInfo> RecordedInfo;
    static vector<string> bannedBlocks = {
            "minecraft:lava",
            "minecraft:water",
            "minecraft:snow_layer"
    };

    bool isVaild(ActorUniqueID id);

    void init(ActorUniqueID id);

    bool isTurningOn(ActorUniqueID id);

    void turnOff(ActorUniqueID id);

    void turnOn(ActorUniqueID id, BlockSource* region, BlockPos bp, int lightLv);

    void clear(ActorUniqueID id);

}

#endif //MOVINGLIGHT_LIGHTMGR_H

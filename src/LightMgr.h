//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

class LightMgr {
public:

    bool isVaild(ActorUniqueID id);

    void init(ActorUniqueID id);

    bool isTurningOn(ActorUniqueID id);

    void turnOff(ActorUniqueID id);

    void turnOn(ActorUniqueID id, BlockSource* region, BlockPos bp, unsigned int lightLv);

    void clear(ActorUniqueID id);

private:

    struct LightInfo {
        bool mLighting      = false;
        unsigned int mLevel = 0;
        BlockPos mPos       = BlockPos::ZERO;
        int mDimId          = -1;
    };

    unordered_map<UniqueID, LightInfo> mRecordedInfo;
    vector<string> mBannedBlocks = {
            "minecraft:lava",
            "minecraft:water",
            "minecraft:snow_layer"
    };


};

extern LightMgr lightMgr;

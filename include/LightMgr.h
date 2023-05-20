//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

class LightMgr {
public:

    bool isVaild(identity_t id);

    void init(identity_t id);

    bool isTurningOn(identity_t id);

    void turnOff(identity_t id);

    void turnOn(identity_t id, BlockSource* region, BlockPos bp, unsigned int lightLv);

    void clear(identity_t id);

private:

    struct LightInfo {
        bool mLighting      = false;
        unsigned int mLevel = 0;
        BlockPos mPos       = BlockPos::ZERO;
        int mDimId          = -1;
    };

    unordered_map<identity_t, LightInfo> mRecordedInfo;
    vector<string> mBannedBlocks = {
            "minecraft:lava",
            "minecraft:water",
            "minecraft:snow_layer"
    };


};

extern LightMgr lightMgr;

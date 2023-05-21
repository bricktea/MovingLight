//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

#include "llapi/mc/BlockSource.hpp"
#include "llapi/mc/HashedString.hpp"

using identity_t = QWORD;

class LightMgr {
public:

    [[nodiscard]] bool isVaild(identity_t id);

    void init(identity_t id);

    [[nodiscard]] bool isTurningOn(identity_t id);

    void turnOff(identity_t id);

    void turnOn(identity_t id, Dimension* dim, BlockPos bp, unsigned int lightLv);

    void clear(identity_t id);

private:

    struct LightInfo {
        bool mLighting      = false;
        unsigned int mLevel = 0;
        BlockPos mPos       = BlockPos::ZERO;
        int mDimId          = -1;
    };

    std::unordered_map<identity_t, LightInfo> mRecordedInfo;
    const vector<HashedString> mBannedBlocks = {
            "minecraft:lava",
            "minecraft:water",
            //"minecraft:snow_layer",
            "standing_sign",
            "spruce_standing_sign",
            "birch_standing_sign",
            "jungle_standing_sign",
            "acacia_standing_sign",
            "darkoak_standing_sign",
            "mangrove_standing_sign",
            "cherry_standing_sign",
            "bamboo_standing_sign",
            "crimson_standing_sign",
            "warped_standing_sign",
            "wall_sign",
            "spruce_wall_sign",
            "birch_wall_sign",
            "jungle_wall_sign",
            "acacia_wall_sign",
            "darkoak_wall_sign",
            "mangrove_wall_sign",
            "cherry_wall_sign",
            "bamboo_wall_sign",
            "crimson_wall_sign",
            "warped_wall_sign",
            "oak_hanging_sign",
            "spruce_hanging_sign",
            "birch_hanging_sign",
            "jungle_hanging_sign",
            "acacia_hanging_sign",
            "dark_oak_hanging_sign",
            "mangrove_hanging_sign",
            "cherry_hanging_sign",
            "bamboo_hanging_sign",
            "crimson_hanging_sign",
            "warped_hanging_sign"
    };


};

extern LightMgr lightMgr;

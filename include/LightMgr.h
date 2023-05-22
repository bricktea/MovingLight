//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

#include "llapi/mc/BlockSource.hpp"
#include "llapi/mc/HashedString.hpp"
#include "llapi/mc/Tick.hpp"

using identity_t = QWORD;

class LightMgr {
public:

    explicit LightMgr() noexcept;

    void init(identity_t id);

    [[nodiscard]] bool isVaild(identity_t id);

    [[nodiscard]] bool isTurningOn(identity_t id);

    void turnOff(identity_t id);

    void turnOn(identity_t id, Dimension* dim, BlockPos bp, unsigned int lightLv);

    void clear(identity_t id);

    void markBadArea(BlockSource& region, const BlockPos& pos);

private:

    void _runBadAreaJanitor();

    bool _isBadArea(const BlockSource& region, const BlockPos& pos);

    CsLock mBadAreaLocker;

    struct LightInfo {
        bool mLighting      = false;
        unsigned int mLevel = 0;
        BlockPos mPos       = BlockPos::ZERO;
        int mDimId          = -1;
    };

    std::unordered_map<int, std::unordered_map<BlockPos, unsigned long long>> mBadAreas;

    std::unordered_map<identity_t, LightInfo> mRecordedInfo;
    const vector<HashedString> mBannedBlocks = {
            "minecraft:lava",
            "minecraft:water",
            "minecraft:snow",
            "minecraft:snow_layer",
            "minecraft:standing_sign",
            "minecraft:spruce_standing_sign",
            "minecraft:birch_standing_sign",
            "minecraft:jungle_standing_sign",
            "minecraft:acacia_standing_sign",
            "minecraft:darkoak_standing_sign",
            "minecraft:mangrove_standing_sign",
            "minecraft:cherry_standing_sign",
            "minecraft:bamboo_standing_sign",
            "minecraft:crimson_standing_sign",
            "minecraft:warped_standing_sign",
            "minecraft:wall_sign",
            "minecraft:spruce_wall_sign",
            "minecraft:birch_wall_sign",
            "minecraft:jungle_wall_sign",
            "minecraft:acacia_wall_sign",
            "minecraft:darkoak_wall_sign",
            "minecraft:mangrove_wall_sign",
            "minecraft:cherry_wall_sign",
            "minecraft:bamboo_wall_sign",
            "minecraft:crimson_wall_sign",
            "minecraft:warped_wall_sign",
            "minecraft:oak_hanging_sign",
            "minecraft:spruce_hanging_sign",
            "minecraft:birch_hanging_sign",
            "minecraft:jungle_hanging_sign",
            "minecraft:acacia_hanging_sign",
            "minecraft:dark_oak_hanging_sign",
            "minecraft:mangrove_hanging_sign",
            "minecraft:cherry_hanging_sign",
            "minecraft:bamboo_hanging_sign",
            "minecraft:crimson_hanging_sign",
            "minecraft:warped_hanging_sign"
    };


};

extern LightMgr lightMgr;

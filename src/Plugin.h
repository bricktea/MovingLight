#pragma once

#ifndef PLUGIN_H
#define PLUGIN_H

#include "llapi/mc/ItemStack.hpp"
#include "llapi/mc/BlockSource.hpp"
#include "llapi/mc/VanillaBlocks.hpp"
#include "llapi/mc/HashedString.hpp"
#include "llapi/mc/Block.hpp"
#include "Nlohmann/json.hpp"
#include "PacketHelper.h"

using namespace std;
using json = nlohmann::json;

typedef long long UniqueID;

namespace Config {

    static bool enable = true;
    static bool enableItemActor = true;
    static unsigned int VERSION = 100;

    static unordered_map<string, unsigned int> items = {
        {"minecraft:beacon",15},            // 信标
        {"minecraft:campfire",15},          // 营火
        {"minecraft:soul_campfire",10},     // 灵魂营火
        {"minecraft:glowstone",15},         // 荧石
        {"minecraft:lit_pumpkin",15},       // 南瓜灯
        {"minecraft:sealantern",15},        // 海晶灯
        {"minecraft:lantern",15},           // 灯笼
        {"minecraft:soul_lantern",10},      // 灵魂灯笼
        {"minecraft:shroomlight",15},       // 菌光体
        {"minecraft:end_rod",14},           // 末地烛
        {"minecraft:torch",14},             // 火把
        {"minecraft:soul_torch",10},        // 灵魂火把
        {"minecraft:underwater_torch",14},  // 水下火把
        {"minecraft:colored_torch_rg",14},  // 彩色火把[红/绿]
        {"minecraft:colored_torch_bp",14},  // 彩色火把[蓝/紫]
        {"minecraft:glow_berries",14},      // 发光浆果
        {"minecraft:crying_obsidian",10},   // 哭泣的黑曜石
        {"minecraft:enchanting_table",7},   // 附魔台
        {"minecraft:ender_chest",7},        // 末影箱
        {"minecraft:redstone_torch",7},     // 红石火把
        {"minecraft:sculk_catalyst",6},     // Sculk Catalyst
        {"minecraft:large_amethyst_bud",4}, // 大型紫晶芽
        {"minecraft:amethyst_cluster",5},   // 紫水晶簇
        {"minecraft:lava_bucket",15}        // 熔岩桶
    };

    static vector<string> OffHandItems = {
        "minecraft:campfire",
        "minecraft:soul_campfire",
        "minecraft:lit_pumpkin",
        "minecraft:sealantern",
        "minecraft:lantern",
        "minecraft:soul_lantern",
        "minecraft:torch",
        "minecraft:soul_torch",
        "minecraft:underwater_torch",
        "minecraft:colored_torch_rg",
        "minecraft:colored_torch_bp",
        "minecraft:redstone_torch",
        "minecraft:ender_chest",
        "minecraft:glow_berries",
        "minecraft:large_amethyst_bud",
        "minecraft:amethyst_cluster",
        "minecraft:lava_bucket"
    };

    inline bool from_json(json& cfg) {
        cfg.at("enabled").get_to(enable);
        cfg.at("items").get_to(items);
        cfg.at("offhand").get_to(OffHandItems);
        return true;
    }

    inline string to_json() {
        json cfg = {
            {"version",VERSION},
            {"enabled",enable},
            {"items",items},
            {"offhand",OffHandItems}
        };
        return cfg.dump(4);
    }

    inline bool isLightSource(const string& name) {
        return items.count(name);
    }

    inline bool isOffhandItem(const string& name) {
        return count(OffHandItems.begin(), OffHandItems.end(), name);
    }

    inline int getBrightness(const ItemStack* it) {
        if (it->isNull())
            return 0;
        auto name = it->getTypeName();
        return isLightSource(name) ? Config::items[name] : 0;
    }

}

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
    
    inline bool isVaild(ActorUniqueID id) {
        return RecordedInfo.count(id);
    }

    inline void init(ActorUniqueID id) {
        LightInfo li;
        RecordedInfo[id] = li;
    }

    inline bool isTurningOn(ActorUniqueID id) {
        return isVaild(id) && RecordedInfo[id].mLighting;
    }

    inline void turnOff(ActorUniqueID id) {
        if (!isTurningOn(id))
            return;
        RecordedInfo[id].mLighting = false;
        auto pos = RecordedInfo[id].mPos;
        auto dim = Global<Level>->createDimension(RecordedInfo[id].mDimId);
        if (dim)
        {
            auto region = &dim->getBlockSourceFromMainChunkSource();
            PacketHelper::UpdateBlockPacket(dim, pos, region->getBlock(pos).getRuntimeId());
        }
    }

    inline void turnOn(ActorUniqueID id, BlockSource* region, BlockPos bp, int lightLv) {
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
        PacketHelper::UpdateBlockPacket(dimId, bp, VanillaBlocks::mLightBlock->getRuntimeId() - 15 + lightLv);
        if (!isSamePos && (isOpened || !isSameLight))
            turnOff(id);

        Info.mLighting = true;
        Info.mDimId = dimId;
        Info.mPos = bp;
        Info.mLevel = lightLv;
        
    }

    inline void clear(ActorUniqueID id) {
        turnOff(id);
        RecordedInfo.erase(id);
    }

}

#endif
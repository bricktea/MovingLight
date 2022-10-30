//
// Created by RedbeanW on 10/30/2022.
//

#ifndef MOVINGLIGHT_CONFIG_H
#define MOVINGLIGHT_CONFIG_H

#include "Plugin.h"

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

    bool from_json(json& cfg);

    string to_json();

    bool isLightSource(const string& name);

    bool isOffhandItem(const string& name);

    unsigned int getBrightness(const ItemStack* it);

}

#endif //MOVINGLIGHT_CONFIG_H

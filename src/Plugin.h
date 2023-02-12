//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "llapi/mc/ItemStack.hpp"
#include "llapi/mc/BlockSource.hpp"
#include "llapi/mc/VanillaBlocks.hpp"
#include "llapi/mc/HashedString.hpp"
#include "llapi/mc/Block.hpp"
#include "llapi/mc/Dimension.hpp"
#include "llapi/mc/BlockPos.hpp"
#include "llapi/mc/BinaryStream.hpp"
#include "llapi/mc/Packet.hpp"
#include "llapi/mc/MinecraftPackets.hpp"
#include "llapi/mc/Level.hpp"
#include "llapi/mc/ItemActor.hpp"
#include "llapi/mc/Container.hpp"
#include "llapi/mc/ServerPlayer.hpp"
#include "llapi/mc/StaticVanillaBlocks.hpp"

#include "Nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

using identity_t = QWORD;
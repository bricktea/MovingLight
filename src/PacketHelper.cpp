//
// Created by RedbeanW on 10/30/2022.
//

#include "PacketHelper.h"

#include "llapi/mc/BlockPos.hpp"
#include "llapi/mc/BinaryStream.hpp"
#include "llapi/mc/MinecraftPackets.hpp"
#include "llapi/mc/Packet.hpp"
#include "llapi/mc/Level.hpp"

PacketHelper packetHelper;

void PacketHelper::UpdateBlockPacket(Dimension *dim, BlockPos bp, const unsigned int runtimeId, BlockUpdateFlags flag, BlockUpdateLayer layer) const {
    if (!dim || mShutdown)
        return;
    BinaryStream wp;
    wp.writeVarInt(bp.x);
    wp.writeUnsignedVarInt(bp.y);
    wp.writeVarInt(bp.z);
    wp.writeUnsignedVarInt(runtimeId);
    wp.writeUnsignedVarInt((unsigned int)flag);
    wp.writeUnsignedVarInt((unsigned int)layer);
    std::shared_ptr<Packet> pkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateBlock);
    pkt->read(wp);
    dim->sendPacketForPosition({ bp.x, bp.y, bp.z }, *pkt, nullptr);
}

void PacketHelper::shutdown() {
    mShutdown = true;
}
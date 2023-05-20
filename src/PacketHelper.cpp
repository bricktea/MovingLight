//
// Created by RedbeanW on 10/30/2022.
//

#include "PacketHelper.h"

PacketHelper packetHelper;

void PacketHelper::UpdateBlockPacket(Dimension *dim, BlockPos bp, const unsigned int runtimeId, unsigned int layer) const {
    if (!dim || mShutdown)
        return;
    BinaryStream wp;
    wp.writeVarInt(bp.x);
    wp.writeUnsignedVarInt(bp.y);
    wp.writeVarInt(bp.z);
    wp.writeUnsignedVarInt(runtimeId);
    wp.writeUnsignedVarInt(3);  // flag
    wp.writeUnsignedVarInt(layer);  // layer
    std::shared_ptr<Packet> pkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateBlock);
    pkt->read(wp);
    dim->sendPacketForPosition({ bp.x, bp.y, bp.z }, *pkt, nullptr);
}

void PacketHelper::UpdateBlockPacket(int dimId, BlockPos bp, const unsigned int runtimeId, unsigned int layer) const {
    auto dim = (Dimension*)Global<Level>->getOrCreateDimension(dimId).mHandle.lock().get();
    UpdateBlockPacket(dim, bp, runtimeId);
}

void PacketHelper::shutdown() {
    mShutdown = true;
}
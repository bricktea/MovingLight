//
// Created by RedbeanW on 10/30/2022.
//

#include "PacketHelper.h"

#include "llapi/mc/BlockPos.hpp"
#include "llapi/mc/BinaryStream.hpp"
#include "llapi/mc/MinecraftPackets.hpp"
#include "llapi/mc/Packet.hpp"
#include "llapi/mc/Level.hpp"

#include "llapi/EventAPI.h"

PacketHelper packetHelper;

PacketHelper::PacketHelper() noexcept {

    Event::ConsoleCmdEvent::subscribe([&](const Event::ConsoleCmdEvent& ev) -> bool {
        if (ev.mCommand == "stop") _shutdown();
        return true;
    });

}


void PacketHelper::UpdateBlockPacket(Dimension& dim, const BlockPos& bp, const unsigned int runtimeId, BlockUpdateFlags flag, BlockUpdateLayer layer) const {
    if (mShutdown) return;
    BinaryStream wp;
    wp.writeVarInt(bp.x);
    wp.writeUnsignedVarInt(bp.y);
    wp.writeVarInt(bp.z);
    wp.writeUnsignedVarInt(runtimeId);
    wp.writeUnsignedVarInt(flag);
    wp.writeUnsignedVarInt(layer);
    std::shared_ptr<Packet> pkt = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateBlock);
    pkt->read(wp);
    dim.sendPacketForPosition(bp, *pkt, nullptr);
}

void PacketHelper::_shutdown() {
    mShutdown = true;
}

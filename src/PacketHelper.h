#ifndef PACKETHELPER_H
#define PACKETHELPER_H

#include "Plugin.h"

class PacketHelper {

    bool stopSending;

public:

    PacketHelper();

    void shutdown();

    void UpdateBlockPacket(Dimension* dim, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1) const;

    void UpdateBlockPacket(int dimId, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1) const;

};

#endif // !PACKETHELPER_H

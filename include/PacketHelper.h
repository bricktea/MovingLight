//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

class PacketHelper {
public:

    void shutdown();

    void UpdateBlockPacket(Dimension* dim, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1) const;

    void UpdateBlockPacket(int dimId, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1) const;

private:

    bool mShutdown = false;

};

extern PacketHelper packetHelper;

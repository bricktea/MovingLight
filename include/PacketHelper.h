//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

#include "llapi/mc/Dimension.hpp"

enum BlockUpdateFlags {
    BlockUpdateNeighbours = 1,
    BlockUpdateNetwork,
    BlockUpdateNoGraphics,
    BlockUpdatePriority
};

enum BlockUpdateLayer {
    BlockUpdateDefault,
    BlockUpdateLiquid
};

class PacketHelper {
public:

    void shutdown();

    void UpdateBlockPacket(Dimension* dim, BlockPos bp, unsigned int runtimeId, BlockUpdateFlags flag = BlockUpdateNetwork, BlockUpdateLayer layer = BlockUpdateDefault) const;

private:

    bool mShutdown = false;

};

extern PacketHelper packetHelper;

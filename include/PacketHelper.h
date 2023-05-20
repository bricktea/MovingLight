//
// Created by RedbeanW on 10/30/2022.
//

#pragma once

#include "Plugin.h"

enum BlockUpdateFlags {
    BlockUpdateNeighbours = 1,
    BlockUpdateNetwork,
    BlockUpdateNoGraphics,
    BlockUpdatePriority
};

enum BlockUpdateLayer {
    UpdateBlockDefault,
    UpdateBlockLiquid
};

class PacketHelper {
public:

    void shutdown();

    void UpdateBlockPacket(Dimension* dim, BlockPos bp, unsigned int runtimeId, BlockUpdateFlags flag = BlockUpdateNetwork, BlockUpdateLayer layer = UpdateBlockDefault) const;

    void UpdateBlockPacket(int dimId, BlockPos bp, unsigned int runtimeId, BlockUpdateFlags flag = BlockUpdateNetwork, BlockUpdateLayer layer = UpdateBlockDefault) const;

private:

    bool mShutdown = false;

};

extern PacketHelper packetHelper;

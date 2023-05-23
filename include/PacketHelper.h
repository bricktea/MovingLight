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
    BlockUpdateDefault = 1,
    BlockUpdateLiquid
};

class PacketHelper {
public:

    explicit PacketHelper() noexcept;

    void UpdateBlockPacket(Dimension& dim, const BlockPos& bp, unsigned int runtimeId, BlockUpdateFlags flag = BlockUpdateNetwork, BlockUpdateLayer layer = BlockUpdateDefault) const;

private:

    void _shutdown();

    std::atomic<bool> mShutdown = false;

};

extern PacketHelper packetHelper;

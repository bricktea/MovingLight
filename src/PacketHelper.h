#ifndef PACKETHELPER_H
#define PACKETHELPER_H

#include "Plugin.h"

namespace PacketHelper {

    /*
    * UpdateBlock:
        Flags:
            NONE = 0
            NEIGHBORS = 1
            NETWORK = 2
            ALL = 3
            NOGRAPHIC = 4
            PRIORITY = 8
            ALL_PRIORITY = 11
        DataLayer:
            NONE = 0
            PENETRATE = 1
            NOUPDATE = 2
    */

    static bool stopSending = false;

    void UpdateBlockPacket(Dimension* dim, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1);

    void UpdateBlockPacket(int dimId, BlockPos bp, unsigned int runtimeId, unsigned int layer = 1);

}

#endif // !PACKETHELPER_H

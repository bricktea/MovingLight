# MovingLight
This plugin provides mobile light source function on bedrock dedicated servers, which is plug and play without the need for addons.

### Requirements
 - Recommend using the latest version of [BDS](https://www.minecraft.net/en-us/download/server/bedrock).
 - [LiteLoaderBDS](https://github.com/LiteLDev/LiteLoaderBDS) plugin loader needs to be installed in advance.

### Features
 - Using fake blocks to create light sources is very efficient.
 - Even if unexpected situations occur, they will not damage the terrain.
 - Support for custom glowing items, and will automatically detect new items that can emit light.
 - You can climb the ladder with a torch! (Support partially hollow blocks)
 - Support for placing light-emitting items in the off-hand slot——this is configurable.
 - It will not damage your redstone machine, for example: it will not trigger the observer's response.
 - Authorized under the MIT license, you may use within the scope of the license.

### Known Issue
 - Some blocks (such as top snow and signs) do not create light sources to avoid client crashes or rendering errors.
 - When the player is moving at high speed, the light source may not appear as realistic.

### How To Switch Item To Off-Hand?
 - Take the item and try to break the block.

### Configuration file
```json5
// Generally speaking, you don't need to modify them.
{
    // If enabled, the plugin will automatically check for new items that can emit light every time server starts up.
    "enableAutoGlowingBlockDiscover": true,
    // Will dropped items emit light?
    "enableItemEntity": true,
    // Main switch.
    "enabled": true,
    // Items that can emit light.
    "items": {
        // key-value, The  key is the full name of the item, and the value represents its brightness level ranging from 0 to 15. If it's set to 0, no light-emitting block will be generated.
        "minecraft:amethyst_cluster": 5,  
...
    },
    "offhand": [
        // Items that can be placed in the off-hand.
        // Usually, this requires you to test it yourself. Some blocks cannot be placed in the off-hand——rendering errors may occur.
        "minecraft:campfire",
...
    ],
    // Don't modify it!
    "version": 200
}
```

### Contact Us
 - Join our telegram group [here](https://t.me/bdsplugin)
 - Send a issue.


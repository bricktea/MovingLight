# MovingLight

Enabling you to hold a light source in your hand and move it around

- Using fake blocks to create light sources is very efficient.
- Even if unexpected situations occur, they will not damage the terrain.
- Support for custom glowing items, and will automatically detect new items that can emit light.
- You can climb the ladder with a torch! (Support partially hollow blocks)
- Support for placing light-emitting items in the off-hand slot——this is configurable.
- It will not damage your redstone machine, for example: it will not trigger the observer's response.
- Authorized under the MIT license, you may use within the scope of the license.

## Install

### Via lip

If you have installed [lip](https://github.com/lippkg/lip), run the following command in the terminal:

```bash
lip install github.com/tooth-hub/movinglight
```

### Manually

1. Download the latest release from [here](https://github.com/LiteLDev/MovingLight/releases).
2. Put `MovingLight.dll` and `MovingLight.pdb` into the `plugins` folder of your server.

## Usage

To put a light source item to your off-hand, just grab it in your main hand and swing your arm.

### Configuration File

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
    },
    "offhand": [
        // Items that can be placed in the off-hand.
        // Usually, this requires you to test it yourself. Some blocks cannot be placed in the off-hand——rendering errors may occur.
        "minecraft:campfire",
    ],
    // Don't modify it!
    "version": 200
}
```

## Known Issues

- Some blocks (such as top snow and signs) do not create light sources to avoid client crashes or rendering errors.
- When the player is moving at high speed, the light source may not appear as realistic.

## Thanks

This project exists thanks to all the people who contribute.

![Contributors](https://contrib.rocks/image?repo=LiteLDev/MovingLight)

## License

[MIT](LICENSE) © 2021-2023 LiteLDev

## Disclaimer

MovingLight (hereinafter referred to as "this software") is developed and 
provided by LiteLDev (hereinafter referred to as "the developer"). This software is designed 
to help users manage and install various software packages, but is not responsible for any 
content, quality, functionality, security or legality of any software package. Users should 
use this software at their own discretion and assume all related risks.

The developer does not guarantee the stability, reliability, accuracy or completeness of this 
software. The developer is not liable for any defects, errors, viruses or other harmful components 
that may exist in this software. The developer is not liable for any direct or indirect damages 
(including but not limited to data loss, device damage, profit loss etc.) caused by the use of 
this software.

The developer reserves the right to modify, update or terminate this software and its related 
services at any time without prior notice to users. Users should back up important data and check 
regularly for updates of this software.

Users should comply with relevant laws and regulations when using this software, respect the 
intellectual property rights and privacy rights of others, and not use this software for any 
illegal or infringing activities. If users violate the above provisions and cause any damage 
to any third party or are claimed by any third party, the developer does not bear any 
responsibility.

If you have any questions or comments about this disclaimer, please contact the developer.

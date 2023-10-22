#ifndef CONFIG_H_
#define CONFIG_H_

#include <llapi/EventAPI.h>

#include <Nlohmann/json.hpp>
#include <llapi/mc/HashedString.hpp>
#include <llapi/mc/ItemStack.hpp>

using json = nlohmann::json;

class Config {
 public:
  explicit Config() noexcept;

  void loadFromFile(const string& path);

  [[nodiscard]] bool isLightSource(const HashedString& name);

  [[nodiscard]] bool isOffhandItem(const HashedString& name);

  [[nodiscard]] bool isEnabled() const;

  [[nodiscard]] bool isItemActorEnabled() const;

  [[nodiscard]] unsigned int getBrightness(const ItemStack& it);

  [[nodiscard]] bool isUnderWaterEnabled() const;

 private:
  bool _fromJson(json& cfg);

  [[nodiscard]] std::string _toString();

  // Call after read is complete.
  void _computeLightBlocks();

  void _update(json& cfg);

  void _save();

 private:
  std::string mPath;

  const unsigned int mVersion = 211;

  bool mEnabled = true;
  bool mEnableItemActor = true;
  bool mAutoDiscoverGlowingBlock = true;
  bool mEnableUnderWater = true;

  std::unordered_map<HashedString, unsigned int> mItems;

  vector<HashedString> mOffHandItems = {"minecraft:campfire",
                                        "minecraft:soul_campfire",
                                        "minecraft:lit_pumpkin",
                                        "minecraft:sealantern",
                                        "minecraft:lantern",
                                        "minecraft:soul_lantern",
                                        "minecraft:torch",
                                        "minecraft:soul_torch",
                                        "minecraft:underwater_torch",
                                        "minecraft:colored_torch_rg",
                                        "minecraft:colored_torch_bp",
                                        "minecraft:redstone_torch",
                                        "minecraft:ender_chest",
                                        "minecraft:glow_berries",
                                        "minecraft:large_amethyst_bud",
                                        "minecraft:amethyst_cluster",
                                        "minecraft:lava_bucket"};
};

#endif  // CONFIG_H_

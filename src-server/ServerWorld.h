#pragma once

#include "SoundPacket/CustomSoundPacket.h"
#include "Weather/WeatherConfig.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/network/packet/PlaySoundPacket.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/events/gameevents/GameEventRegistry.h"
#include "mc/world/item/ItemInstance.h"
#include <ll/api/network/packet/Packet.h>
#include <ll/api/network/packet/PacketRegistrar.h>
#include <ll/api/service/Bedrock.h>
#include <ll/api/service/TargetedBedrock.h>
#include <map>
#include <mc/deps/core/math/Vec3.h>
#include <mc/deps/core/sound/LoopingSoundState.h>
#include <mc/deps/core/sound/SoundPlayerInterface.h>
#include <mc/deps/ecs/WeakEntityRef.h>
#include <mc/legacy/ActorRuntimeID.h>
#include <mc/network/LoopbackPacketSender.h>
#include <mc/server/ServerLevel.h>
#include <mc/world/actor/ModelPartLocator.h>
#include <mc/world/actor/animation/AnimationComponent.h>
#include <mc/world/item/registry/ItemRegistryManager.h>
#include <mc/world/level/BlockPos.h>
#include <mc/world/level/ILevelBlockDestroyerProxy.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/LevelBlockDestroyer.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/BlockChangeContext.h>
#include <mc/world/level/block/BlockType.h>
#include <mc/world/level/dimension/Dimension.h>
#include <string>


namespace World {
// 基岩版底层的方块更新标志位枚举
enum class BlockUpdateFlag : int {
    None      = 0, // 0：什么都不做（静默放置）
    Neighbors = 1, // 1：仅更新邻居（触发物理/红石）
    Network   = 2, // 2：仅同步网络客户端（玩家可见）
    NoGraphic = 4, // 4：抑制图形渲染
    Priority  = 8, // 8：高优先级更新

    // 下面是官方预先组合好的常用掩码：
    All         = Neighbors | Network, // 3：全更新（最常用的正常放置）
    AllPriority = All | Priority       // 11：高优先级全更新
};

static BlockChangeContext context(false);

class ServerWorld {
private:
    std::map<int, ::BlockSource*> blockSources;
    ::Level*                      level;
    ServerWorld();

public:
    static ServerWorld& getInstance() {
        static ServerWorld instance;
        return instance;
    }

    void init(ServerLevel*);

    [[nodiscard]] ::Level* getLevel() const;

    [[nodiscard]] ::BlockSource* getBlockSource(::DimensionType);
};

void setBlock(int, int, int, ::Block const&, ::DimensionType const&);
void destroyBlock(int, int, int, ::DimensionType const&, bool dropResources = true);
void drop(float, float, float, short, int, int, ::DimensionType const&); // 缺少额外数据
void explode(float, float, float, float, bool, ::DimensionType const&);
void setWorldTime(int);
void setWeather(const WeatherConfig& weather);
void playSound(float, float, float, const std::string&, float, ::DimensionType const&, float pitch = 0.5);

void playSoundAtEntity(Actor* actor, const std::string& soundName);
} // namespace World
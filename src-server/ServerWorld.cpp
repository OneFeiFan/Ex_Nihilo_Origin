#include "ServerWorld.h"
#include "ll/api/service/TargetedBedrock.h"
#include "mc/common/SharedPtr.h"
#include <string>

namespace World {


ServerWorld::ServerWorld() : level(nullptr) {}

void ServerWorld::init(ServerLevel* serverLevel) {
    level = serverLevel;
    if (level == nullptr) {
        throw std::runtime_error("Level is null");
    }
}

::Level* ServerWorld::getLevel() const { return level; }

::BlockSource* ServerWorld::getBlockSource(::DimensionType dimId) {
    if (!level) {
        return nullptr;
    }

    auto it = blockSources.find(dimId.id);
    if (it != blockSources.end()) {
        return it->second;
    }

    auto dim = level->getDimension(dimId).lock();
    if (dim != nullptr) {
        ::BlockSource* source  = &dim->getBlockSourceFromMainChunkSource();
        blockSources[dimId.id] = source;
        return source;
    }

    return nullptr;
}

void setBlock(int x, int y, int z, ::Block const& block, ::DimensionType const& dimensionType) {
    auto blockSource = ServerWorld::getInstance().getBlockSource(dimensionType);
    if (blockSource != nullptr) {
        blockSource->setBlock(*(new BlockPos(x, y, z)), block, (int)BlockUpdateFlag::All, nullptr, context);
    } else {
        std::cout << "BlockSource is null" << std::endl;
    }
}

void destroyBlock(int x, int y, int z, ::DimensionType const& dimensionType, bool dropResources) {
    auto world       = ServerWorld::getInstance();
    auto level       = world.getLevel();
    auto blockSource = world.getBlockSource(dimensionType);
    if (level != nullptr && blockSource != nullptr) {
        auto blockPos = new BlockPos(x, y, z);
        auto block    = &blockSource->getBlock(*blockPos);
        if (block->getBlockItemId() == 0) {
            return;
        }

        auto levelEventManager        = level->getLevelEventManager().get().get();
        auto levelBlockDestroyerProxy = level->getLevelBlockDestroyer()->mLevelBlockDestroyerProxy->get().get();
        blockSource->removeBlock(*blockPos, context);
        levelBlockDestroyerProxy->sendEvents(*levelEventManager, *blockSource, *blockPos, *block);
        if (dropResources) {
            levelBlockDestroyerProxy->dropResources(*blockSource, *blockPos, *block);
        }
    } else {
        std::cout << "BlockSource/Level is null" << std::endl;
    }
}

void drop(float x, float y, float z, short id, int count, int data, ::DimensionType const& dimensionType) {
    auto blockSource = ServerWorld::getInstance().getBlockSource(dimensionType);
    if (blockSource != nullptr) {
        if (id == 0) {
            return;
        }
        auto itemRegistry = ItemRegistryManager::getItemRegistry();
        auto item         = itemRegistry.getItem(id).get();
        if (item != nullptr) {
            ItemInstance instance(*item, count, data);
            BlockType::popResource(*blockSource, *(new BlockPos(x, y, z)), instance);
        }
    } else {
        std::cout << "BlockSource is null" << std::endl;
    }
}
void explode(float x, float y, float z, float power, bool fire, ::DimensionType const& dimensionType) {
    auto world       = ServerWorld::getInstance();
    auto level       = world.getLevel();
    auto blockSource = world.getBlockSource(dimensionType);
    if (level != nullptr && blockSource != nullptr) {
        level->explode(*blockSource, nullptr, Vec3(x, y, z), power, fire, true, power, false);
    } else {
        std::cout << "BlockSource/Level is null" << std::endl;
    }
}
void setWorldTime(int time) {
    auto level = ServerWorld::getInstance().getLevel();
    if (level != nullptr) {
        level->setTime(time);
    } else {
        std::cout << "Level is null" << std::endl;
    }
}
void setWeather(const WeatherConfig& weather) {
    auto level = ServerWorld::getInstance().getLevel();
    if (level != nullptr) {
        level->updateWeather(
            weather.getRainLevel(),
            weather.getRainTime(),
            weather.getLightningLevel(),
            weather.getLightningTime()
        );
    } else {
        std::cout << "Level is null" << std::endl;
    }
}
void playSound(
    float                  x,
    float                  y,
    float                  z,
    const std::string&     name,
    float                  volume,
    ::DimensionType const& dimensionType,
    float                  pitch
) {
    auto world       = ServerWorld::getInstance();
    auto level       = world.getLevel();
    auto blockSource = world.getBlockSource(dimensionType);
    if (level != nullptr && blockSource != nullptr) {
        auto            blockPos = new BlockPos(x, y, z);
        PlaySoundPacket soundPacket(PlaySoundPacketPayload(name, *blockPos, volume, pitch));
        level->getDimension(dimensionType).lock()->sendPacketForPosition(*blockPos, soundPacket, nullptr);
        blockSource->postGameEvent(nullptr, GameEventRegistry::NoteBlockPlay, *blockPos, nullptr);
    } else {
        std::cout << "BlockSource/Level is null" << std::endl;
    }
}
void playSoundAtEntity(Actor* actor, const std::string& soundName) {
    if (actor != nullptr) {
        auto       entityId = actor->getRuntimeID().rawID;
        TestPacket packet(soundName, entityId);
        packet.sendBroadcast();
    } else {
        std::cout << "Actor is null" << std::endl;
    }
}
} // namespace World
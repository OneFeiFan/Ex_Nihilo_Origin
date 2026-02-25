#include "ClientWorld.h"
#include "LoopSoundManager/LoopSoundManager.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ModelPartLocator.h"
#include "mc/world/actor/animation/AnimationComponent.h"
#include "mc/world/level/dimension/Dimension.h"


namespace World {
ClientWorld::ClientWorld() : clientInstance(nullptr) {}

void ClientWorld::init(ClientInstance* client) {
    clientInstance = client;
    if (clientInstance == nullptr) {
        throw std::runtime_error("ClientInstance is null");
    }
}

::Level* ClientWorld::getLevel() const {
    if (clientInstance == nullptr) {
        return nullptr;
    }
    return (Level*)clientInstance->getLevel();
}

::BlockSource* ClientWorld::getBlockSource() {
    if (!clientInstance) {
        return nullptr;
    }

    auto dimId = clientInstance->getLocalPlayer()->getDimensionId();

    auto it = blockSources.find(dimId.id);
    if (it != blockSources.end()) {
        return it->second;
    }

    auto dim = getLevel()->getDimension(dimId).lock();
    if (dim != nullptr) {
        ::BlockSource* source  = &dim->getBlockSourceFromMainChunkSource();
        blockSources[dimId.id] = source;
        return source;
    }

    return nullptr;
}
void playSoundAtEntity(uint64 id, const std::string& soundName) {
    auto level = ClientWorld::getInstance().getLevel();
    if (level == nullptr) {
        return;
    };
    auto         soundPlayer = level->getSoundPlayer();
    HashedString locatorName("mouth");
    LoopSoundManager::getInstance().stopEntityLoop(id);
    uint64_t loopId = soundPlayer->registerLoop(
        soundName,
        [id, locatorName, soundPlayer](LoopingSoundState& props) {
            auto level = ll::service::getMultiPlayerLevel();
            if (level == nullptr) {
                props.mVolume = 0.0;
                LoopSoundManager::getInstance().stopEntityLoop(id);
                return;
            }
            auto player = level->getRuntimeEntity(id, false);
            if (player == nullptr) {
                props.mVolume = 0.0;
                LoopSoundManager::getInstance().stopEntityLoop(id);
                return;
            }
            if (player->isAlive() && !player->mRemoved) {
                Vec3  audioPos;
                auto* locator = player->mAnimationComponent.get()->getLocator(locatorName);

                if (locator) {
                    audioPos = locator->mPosition;
                } else {
                    audioPos = player->getPosition();
                }
                props.mPos    = audioPos;
                props.mPitch  = 0.5;
                props.mVolume = 1.0;
            } else {
                props.mVolume = 0.0;
                LoopSoundManager::getInstance().stopEntityLoop(id);
            }
        },
        1.0f,
        1.0f
    );
    LoopSoundManager::getInstance().setEntityLoop(id, loopId, soundPlayer.get());
}
} // namespace World
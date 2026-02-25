#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <ll/api/service/TargetedBedrock.h>
#include <mc/client/game/ClientInstance.h>
#include <mc/client/player/LocalPlayer.h>
#include <mc/deps/core/sound/LoopingSoundState.h>
#include <mc/deps/core/sound/SoundPlayerInterface.h>
#include <mc/legacy/ActorRuntimeID.h>
#include <mc/world/level/Level.h>


namespace World {
class ClientWorld {
private:
    std::map<int, ::BlockSource*> blockSources;
    ::ClientInstance*             clientInstance;
    ClientWorld();

public:
    static ClientWorld& getInstance() {
        static ClientWorld instance;
        return instance;
    }

    void init(ClientInstance*);

    [[nodiscard]] ::Level* getLevel() const;

    [[nodiscard]] ::BlockSource* getBlockSource();
};

void playSoundAtEntity(uint64 id, const std::string& soundName);
} // namespace World
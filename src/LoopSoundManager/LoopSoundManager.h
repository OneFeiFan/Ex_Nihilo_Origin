#pragma once

#include <cstdint>
#include <mc/deps/core/sound/SoundPlayerInterface.h>
#include <mutex>
#include <unordered_map>

// 绑定音效 ID 和对应的播放器
struct LoopInfo {
    uint64_t              loopId;
    SoundPlayerInterface* soundPlayer;
};

class LoopSoundManager {
public:
    static LoopSoundManager& getInstance() {
        static LoopSoundManager instance;
        return instance;
    }

    void setEntityLoop(uint64_t actorId, uint64_t loopId, SoundPlayerInterface* soundPlayer);

    void stopEntityLoop(uint64_t actorId);

private:
    LoopSoundManager()  = default;
    ~LoopSoundManager() = default;

    std::unordered_map<uint64_t, LoopInfo> mActiveLoops; // ActorID -> LoopInfo
    std::mutex                             mMutex;
};
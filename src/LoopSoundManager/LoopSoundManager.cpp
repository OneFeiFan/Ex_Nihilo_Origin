#include "LoopSoundManager.h"

void LoopSoundManager::setEntityLoop(uint64_t actorId, uint64_t loopId, SoundPlayerInterface* soundPlayer) {
    if (!soundPlayer) return; // 基础安全检查

    std::lock_guard<std::mutex> lock(mMutex);
    mActiveLoops[actorId] = {loopId, soundPlayer};
}

void LoopSoundManager::stopEntityLoop(uint64_t actorId) {
    std::lock_guard<std::mutex> lock(mMutex);

    // 使用 find 可以一次性拿到 key 和 value，避免二次查找
    auto it = mActiveLoops.find(actorId);
    if (it != mActiveLoops.end()) {
        // 取出保存的播放器和 ID 进行注销
        if (it->second.soundPlayer) {
            it->second.soundPlayer->unregisterLoop(it->second.loopId, false);
        }
        // 抹除记录
        mActiveLoops.erase(it);
    }
}
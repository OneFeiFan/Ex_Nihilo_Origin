#include "WeatherConfig.h"
#include <algorithm>
#include <random>

int WeatherConfig::generateVanillaDefaultTime() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 599);
    
    // 返回 6000 ~ 17980 Ticks (5到15分钟)
    return 20 * distrib(gen) + 6000;
}

void WeatherConfig::validate() {
    // 约束时间范围：1 到 1,000,000 Ticks
    rainTime = std::clamp(rainTime, 1, 1000000);
    lightningTime = std::clamp(lightningTime, 1, 1000000);

    // 约束天气强度：0.0f 到 1.0f
    rainLevel = std::clamp(rainLevel, 0.0f, 1.0f);
    lightningLevel = std::clamp(lightningLevel, 0.0f, 1.0f);
}

WeatherConfig::WeatherConfig(WeatherType type, int durationTicks) {
    int finalTime = (durationTicks <= 0) ? generateVanillaDefaultTime() : durationTicks;

    switch (type) {
        case WeatherType::Clear:
            rainLevel = 0.0f;
            lightningLevel = 0.0f;
            break;
        case WeatherType::Rain:
            rainLevel = 1.0f;
            lightningLevel = 0.0f;
            break;
        case WeatherType::Thunder:
            rainLevel = 1.0f;
            lightningLevel = 1.0f;
            break;
    }

    rainTime = finalTime;
    lightningTime = finalTime;

    validate(); // 构造完毕后执行范围校验
}

WeatherConfig::WeatherConfig(float rLevel, int rTime, float lLevel, int lTime) 
    : rainLevel(rLevel), rainTime(rTime), lightningLevel(lLevel), lightningTime(lTime) 
{
    validate(); // 构造完毕后执行范围校验
}

float WeatherConfig::getRainLevel() const { return rainLevel; }
int WeatherConfig::getRainTime() const { return rainTime; }
float WeatherConfig::getLightningLevel() const { return lightningLevel; }
int WeatherConfig::getLightningTime() const { return lightningTime; }
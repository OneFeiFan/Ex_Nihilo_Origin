#pragma once

enum class WeatherType {
    Clear,   // 晴天
    Rain,    // 雨天
    Thunder  // 雷暴
};

class WeatherConfig {
private:
    float rainLevel;
    int rainTime;
    float lightningLevel;
    int lightningTime;

    // 内部辅助方法声明
    static int generateVanillaDefaultTime();
    void validate();

public:
    // 构造函数 1：使用预设天气类型 (支持手动设置持续时间)
    explicit WeatherConfig(WeatherType type, int durationTicks = 0);

    // 构造函数 2：完全自定义参数
    WeatherConfig(float rLevel, int rTime, float lLevel, int lTime);

    // Getters 声明
    [[nodiscard]] float getRainLevel() const;
    [[nodiscard]] int getRainTime() const;
    [[nodiscard]] float getLightningLevel() const;
    [[nodiscard]] int getLightningTime() const;
};
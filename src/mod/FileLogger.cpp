#include "FileLogger.h"
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <iostream>

namespace MyLogger {

    // 静态变量用于存储路径和锁
    static std::filesystem::path gLogPath;
    static std::mutex gLogMutex;

    void init(std::filesystem::path const& modDir, std::string const& fileName) {
        std::lock_guard<std::mutex> lock(gLogMutex);

        // 1. 确保目录存在
        if (!std::filesystem::exists(modDir)) {
            std::filesystem::create_directories(modDir);
        }

        // 2. 拼接完整路径
        gLogPath = modDir / fileName;

        // 3. 以 trunc 模式打开，这会直接清空文件内容（实现“每次启动更新”）
        std::ofstream file(gLogPath, std::ios::out | std::ios::trunc);
        if (file.is_open()) {
            // 可以在开头写一行启动时间
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            file << "=== Log Started at " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << " ===" << std::endl;
        }
        // file 析构时会自动关闭
    }

    void log(std::string const& message) {
        std::lock_guard<std::mutex> lock(gLogMutex);

        if (gLogPath.empty()) return; // 防止未初始化就调用

        // 以 app (append) 模式打开，追加内容
        std::ofstream file(gLogPath, std::ios::out | std::ios::app);
        if (file.is_open()) {
            // 获取当前时间
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            
            // 写入格式: [时间] 内容
            file << "[" << std::put_time(std::localtime(&now), "%H:%M:%S") << "] " 
                 << message << std::endl;
        }
    }
}
#pragma once

#include <string>
#include <filesystem>

namespace MyLogger {

    /**
     * @brief 初始化日志系统。必须在插件启动时调用一次。
     * 
     * @param modDir 模组的根目录 (通过 getSelf().getModDir() 获取)
     * @param fileName 日志文件名 (默认为 mod.log)
     */
    void init(std::filesystem::path const& modDir, std::string const& fileName = "mod.log");

    /**
     * @brief 写入日志到文件
     * 
     * @param message 要写入的内容
     */
    void log(std::string const& message);

}
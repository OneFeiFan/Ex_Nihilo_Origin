#include "TextLocalizer.h"

namespace TextLocalizer {

// 存储所有已注册物品/方块的完整 Key (如 item.ns:name.name)
std::vector<std::string> g_registryKeys;

// 存储翻译：Map<语言代码, Map<短名, 翻译内容>>
std::unordered_map<std::string, std::unordered_map<std::string, std::string>> g_translationMap;

// 输入: "item.my_mod:sword.name"
// 输出 pair: {"my_mod:sword", "sword"}
std::pair<std::string, std::string> parseIdentifiers(const std::string& fullKey) {
    // 简单解析逻辑：假设格式为 prefix.namespace:name.suffix

    size_t firstDot = fullKey.find('.');
    size_t lastDot  = fullKey.rfind('.');

    if (firstDot == std::string::npos || lastDot == std::string::npos || firstDot == lastDot) {
        return {fullKey, fullKey}; // 无法解析，返回原样
    }

    // 提取中间部分：my_mod:sword
    std::string identifier = fullKey.substr(firstDot + 1, lastDot - firstDot - 1);

    std::string shortName = identifier;
    size_t      colon     = identifier.find(':');
    if (colon != std::string::npos) {
        // 提取纯名：sword
        shortName = identifier.substr(colon + 1);
    }

    return {identifier, shortName};
}

void registerFullKey(const std::string& fullKey) { g_registryKeys.emplace_back(fullKey); }

void addTranslation(const std::string& raw, const std::string& value, const std::string& locale) {
    std::string partialName               = ll::utils::string_utils::toLowerCase(raw);
    g_translationMap[locale][partialName] = value;
}

void inject(I18n& i18n) {
    if (g_translationMap.empty() || g_registryKeys.empty()) return;

    // 准备最终注入的数据包
    std::unordered_multimap<std::string, std::pair<std::string, std::string>> batchData;

    // 获取当前语言代码
    std::string currentLang = i18n.getCurrentLanguage()->mCode;
    auto        transMap    = g_translationMap[currentLang];

    if (transMap.empty()) {
        return;
    }

    // 遍历所有已注册的完整 Key (item.my_mod:sword.name)
    for (const auto& fullKey : g_registryKeys) {

        // 解析出两个候选匹配键
        // idFull = "my_mod:sword"
        // idShort = "sword"
        auto [idFull, idShort] = parseIdentifiers(fullKey);


        // 精确匹配 (Namespace:Name)
        if (transMap.count(idFull)) {
            batchData.insert({
                currentLang,
                {fullKey, transMap[idFull]}
            });
        }
        // 模糊匹配 (Name only)
        else if (transMap.count(idShort)) {
            batchData.insert({
                currentLang,
                {fullKey, transMap[idShort]}
            });
        }
    }

    if (!batchData.empty()) {
        i18n.loadLangaugesByLocale(batchData);
    }
}

void clear() {
    g_registryKeys.clear();
    g_translationMap.clear();
}
} // namespace TextLocalizer
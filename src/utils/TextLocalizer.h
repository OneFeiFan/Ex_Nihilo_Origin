#pragma once
#include <ll/api/utils/StringUtils.h>
#include <mc/locale/I18n.h>
#include <mc/locale/Localization.h>
#include <string>


namespace TextLocalizer {

struct TranslationEntry {
    std::string partialKey; // 短名 (如 "sword" 或 "ns:sword")
    std::string value;      // 翻译 (如 "剑")
};

void registerFullKey(const std::string&);

// 第一个参数可以是 "sword" 也可以是 "my_mod:sword"
void addTranslation(const std::string&, const std::string&, const std::string&);

void inject(I18n&);

void clear();
} // namespace TextLocalizer
#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/world/item/DiggerItem.h"
#include "mc/world/item/PickaxeItem.h"
#include "mc/world/item/ShovelItem.h"
#include "mc/world/item/HatchetItem.h"
#include "mc/world/item/HoeItem.h"
#include "mc/world/item/WeaponItem.h"
#include "mc/world/item/FoodItemComponentLegacy.h"
#include <ll/api/utils/StringUtils.h>
#include <mc/deps/core/string/HashedString.h>
#include <mc/deps/json/Value.h>
#include <mc/deps/json/ValueType.h>
#include <mc/deps/shared_types/legacy/item/UseAnimation.h>
#include <mc/world/item/Item.h>
#include <mc/world/item/VanillaItemTiers.h>
#include <mc/world/item/registry/ItemRegistryManager.h>
#include <mc/world/item/registry/ItemRegistryRef.h>
#include <string>
#include <vector>


namespace Items {

class Item {
public:
    HashedString name;
    std::string  icon;


    Item(const std::string& name, const std::string& icon);

    void setIcon();

    virtual SharedPtr<::Item> initNativeItem(short id);

    WeakPtr<::Item> getItem() const;

    virtual ~Item() = default;
};

class WeaponItem : public Item {
public:
    ItemTier tier;

    WeaponItem(const std::string& name, const std::string& icon);

    WeaponItem& setTier(const ItemTier& tier);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~WeaponItem() = default;
};

class DiggerItem : public Item {
public:
    ItemTier tier;

    DiggerItem(const std::string& name, const std::string& icon);

    DiggerItem& setTier(const ItemTier& tier);

    virtual ~DiggerItem() = default;
};

class PickaxeItem : public DiggerItem {
public:

    PickaxeItem(const std::string& name, const std::string& icon);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~PickaxeItem() = default;
};

class ShovelItem : public DiggerItem {
public:

    ShovelItem(const std::string& name, const std::string& icon);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~ShovelItem() = default;
};

class HatchetItem : public DiggerItem {
public:

    HatchetItem(const std::string& name, const std::string& icon);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~HatchetItem() = default;
};

class HoeItem : public DiggerItem {
public:

    HoeItem(const std::string& name, const std::string& icon);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~HoeItem() = default;
};

class FoodItem : public Item {
public:
    int         nutrition;   // 饥饿值
    std::string saturation;  // 饱和度恢复水平
    int         useDuration; // 使用时长十进制数，毫秒？

    FoodItem(const std::string& name, const std::string& icon);

    FoodItem& setSaturation(const std::string& saturation);

    FoodItem& setSaturation(const char* saturation);

    FoodItem& setNutrition(int nutrition);

    FoodItem& setUseDuration(int useDuration);

    SharedPtr<::Item> initNativeItem(short id) override;

    virtual ~FoodItem() = default;
};

extern std::vector<Items::Item*> items; // 保持原有容器
} // namespace Items
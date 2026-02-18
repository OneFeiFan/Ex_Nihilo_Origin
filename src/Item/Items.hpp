#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/world/item/DiggerItem.h"
#include "mc/world/item/FoodItemComponentLegacy.h"
#include "mc/world/item/HatchetItem.h"
#include "mc/world/item/HoeItem.h"
#include "mc/world/item/PickaxeItem.h"
#include "mc/world/item/ShovelItem.h"
#include "mc/world/item/WeaponItem.h"
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


    Item(const std::string&, const std::string&);

    void setIcon();

    virtual SharedPtr<::Item> initNativeItem(short);

    WeakPtr<::Item> getItem() const;

    virtual ~Item() = default;
};

class WeaponItem : public Item {
public:
    ItemTier tier;

    WeaponItem(const std::string&, const std::string&);

    WeaponItem& setTier(const ItemTier&);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~WeaponItem() = default;
};

class DiggerItem : public Item {
public:
    ItemTier tier;

    DiggerItem(const std::string&, const std::string&);

    DiggerItem& setTier(const ItemTier&);

    virtual ~DiggerItem() = default;
};

class PickaxeItem : public DiggerItem {
public:
    PickaxeItem(const std::string&, const std::string&);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~PickaxeItem() = default;
};

class ShovelItem : public DiggerItem {
public:
    ShovelItem(const std::string&, const std::string&);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~ShovelItem() = default;
};

class HatchetItem : public DiggerItem {
public:
    HatchetItem(const std::string&, const std::string&);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~HatchetItem() = default;
};

class HoeItem : public DiggerItem {
public:
    HoeItem(const std::string&, const std::string&);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~HoeItem() = default;
};

class FoodItem : public Item {
public:
    int         nutrition;   // 饥饿值
    std::string saturation;  // 饱和度恢复水平
    int         useDuration; // 使用时长十进制数，毫秒？

    FoodItem(const std::string&, const std::string&);

    FoodItem& setSaturation(const std::string&);

    FoodItem& setSaturation(const char*);

    FoodItem& setNutrition(int);

    FoodItem& setUseDuration(int);

    SharedPtr<::Item> initNativeItem(short) override;

    virtual ~FoodItem() = default;
};

extern std::vector<Items::Item*> items; // 保持原有容器
} // namespace Items
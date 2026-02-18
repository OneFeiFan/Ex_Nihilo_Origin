#include "Items.hpp"
#include "mc/common/SharedPtr.h"
#include "mod/FileLogger.h"

namespace Items {

std::vector<Items::Item*> items;

Item::Item(const std::string& name, const std::string& icon) {
    this->name = HashedString(ll::utils::string_utils::toLowerCase(NAME_SPACE ":" + name));
    this->icon = icon;
    Items::items.emplace_back(this);
}

WeaponItem::WeaponItem(const std::string& name, const std::string& icon)
: Item(name, icon),
  tier(VanillaItemTiers::WOOD()) {}

WeaponItem& WeaponItem::setTier(const ItemTier& tier) {
    this->tier = tier;
    return *this;
}

DiggerItem::DiggerItem(const std::string& name, const std::string& icon)
: Item(name, icon),
  tier(VanillaItemTiers::WOOD()) {}

DiggerItem& DiggerItem::setTier(const ItemTier& tier) {
    this->tier = tier;
    return *this;
}

PickaxeItem::PickaxeItem(const std::string& name, const std::string& icon) : DiggerItem(name, icon) {}

ShovelItem::ShovelItem(const std::string& name, const std::string& icon) : DiggerItem(name, icon) {}

HatchetItem::HatchetItem(const std::string& name, const std::string& icon) : DiggerItem(name, icon) {}

HoeItem::HoeItem(const std::string& name, const std::string& icon) : DiggerItem(name, icon) {}

FoodItem::FoodItem(const std::string& name, const std::string& icon) : Item(name, icon), nutrition(0), useDuration(0) {}

FoodItem& FoodItem::setSaturation(const char* saturation) {
    this->saturation = saturation;
    return *this;
}

FoodItem& FoodItem::setSaturation(const std::string& saturation) {
    this->saturation = saturation;
    return *this;
}

FoodItem& FoodItem::setNutrition(int nutrition) {
    this->nutrition = nutrition;
    return *this;
}

FoodItem& FoodItem::setUseDuration(int useDuration) {
    this->useDuration = useDuration;
    return *this;
}
SharedPtr<::Item> Item::initNativeItem(short id) {
    auto item = SharedPtr<::Item>::make(name, id);
    return item;
}

SharedPtr<::Item> WeaponItem::initNativeItem(short id) { return SharedPtr<::WeaponItem>::make(name, id, tier); }

SharedPtr<::Item> PickaxeItem::initNativeItem(short id) { return SharedPtr<::PickaxeItem>::make(name, id, tier); }

SharedPtr<::Item> ShovelItem::initNativeItem(short id) { return SharedPtr<::ShovelItem>::make(name, id, tier); }
SharedPtr<::Item> HatchetItem::initNativeItem(short id) { return SharedPtr<::HatchetItem>::make(name, id, tier); }
SharedPtr<::Item> HoeItem::initNativeItem(short id) { return SharedPtr<::HoeItem>::make(name, id, tier); }

SharedPtr<::Item> FoodItem::initNativeItem(short id) {
    auto item = SharedPtr<::Item>::make(name, id);
    item->setMaxUseDuration(useDuration);
    item->mUseAnim = SharedTypes::Legacy::UseAnimation::Eat;

    auto*         itemComponent = new FoodItemComponentLegacy(*(item.get()));
    ::Json::Value data          = Json::ValueType::ObjectValue;
    data["nutrition"]           = nutrition;
    data["saturation_modifier"] = saturation;
    itemComponent->init(data, MolangVersion::Latest);
    item->mFoodComponentLegacy = std::unique_ptr<FoodItemComponentLegacy>(itemComponent);
    return item;
}

void Item::setIcon() { this->getItem().get()->setIconInfo(icon, 0); }

WeakPtr<::Item> Item::getItem() const { return ItemRegistryManager::getItemRegistry().getItem(name); }

} // namespace Items

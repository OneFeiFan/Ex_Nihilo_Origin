#include "WeaponItem.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"

WeaponItem::WeaponItem(const std::string& name, short id, const ItemTier& tier) : ::Item(name, id), mTier(tier) {
    this->mCreativeCategory = CreativeItemCategory::Equipment;
    this->mHandEquipped     = true;
    this->setMaxDamage(tier.mUses); // 最大耐久
    this->mMaxStackSize = 1;        // 工具和武器必须只能堆叠 1 个
    this->setMaxUseDuration(7200);
    this->mDamage = tier.mDamage + 4;
    this->addTag(HashedString("minecraft:is_sword"));
    this->addTag(HashedString("minecraft:is_tool"));
}
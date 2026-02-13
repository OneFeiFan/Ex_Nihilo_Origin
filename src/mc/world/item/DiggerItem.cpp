#include "DiggerItem.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/world/item/Item.h>

DiggerItem::DiggerItem(
    const std::string&  name,
    short               id,
    int                 attackBonus,
    const ItemTier&     tier,
    const HashedString& tags
)
: Item(name, id),
  mTier(tier),
  mBlockDestructionTag(tags),
  mAttackDamage(tier.mDamage + attackBonus) {
    this->mCreativeCategory = CreativeItemCategory::Equipment;
    this->mHandEquipped     = true;
    this->setMaxDamage(tier.mUses); // 最大耐久
    this->mMaxStackSize = 1;        // 工具和武器必须只能堆叠 1 个
    this->addTag(HashedString("minecraft:digger"));
}
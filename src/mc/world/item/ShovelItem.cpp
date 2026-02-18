#include "ShovelItem.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/world/item/Item.h>

static const HashedString tag("minecraft:is_shovel_item_destructible"); // 这个tag生命周期必须长，如果是局部变量会出问题

ShovelItem::ShovelItem(const std::string& name, short id, const ItemTier& tier) : DiggerItem(name, id, 1, tier, tag) {
    this->addTag(HashedString("minecraft:is_shovel"));
    this->addTag(HashedString("minecraft:is_tool"));
}
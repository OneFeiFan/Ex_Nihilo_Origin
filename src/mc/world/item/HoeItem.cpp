#include "HoeItem.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/world/item/Item.h>

static const HashedString tag("minecraft:is_hoe_item_destructible");

HoeItem::HoeItem(const std::string& name, short id, const ItemTier& tier) : DiggerItem(name, id, 1, tier, tag) {
    this->addTag(HashedString("minecraft:is_hoe"));
    this->addTag(HashedString("minecraft:is_tool"));
}
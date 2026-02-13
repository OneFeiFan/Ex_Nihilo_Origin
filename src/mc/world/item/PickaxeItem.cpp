#include "PickaxeItem.h"

static const HashedString tag("minecraft:is_pickaxe_item_destructible");//这个tag生命周期必须长，如果是局部变量会出问题

PickaxeItem::PickaxeItem(const std::string& name, short id, const ItemTier& tier) : DiggerItem(name, id, 2, tier, tag) {
    this->addTag(HashedString("minecraft:is_pickaxe"));
    this->addTag(HashedString("minecraft:is_tool"));
}
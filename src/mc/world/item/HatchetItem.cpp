#include "HatchetItem.h"

static const HashedString tag("minecraft:is_axe_item_destructible");//这个tag生命周期必须长，如果是局部变量会出问题

HatchetItem::HatchetItem(const std::string& name, short id, const ItemTier& tier) : DiggerItem(name, id, 3, tier, tag) {
    this->addTag(HashedString("minecraft:is_axe"));
    this->addTag(HashedString("minecraft:is_tool"));
}
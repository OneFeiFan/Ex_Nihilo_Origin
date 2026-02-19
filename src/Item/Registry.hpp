#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/deps/core/string/HashedString.h>
#include <mc/world/item/Item.h>
#include <mc/world/item/registry/ItemRegistry.h>
#include "utils/TextLocalizer.h"


namespace ItemRegistrar {
void registerItem(ItemRegistry*);
void setItemsIconInfo();
}; // namespace ItemRegistrar
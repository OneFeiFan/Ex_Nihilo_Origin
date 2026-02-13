#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/world/item/Item.h>
#include <mc/deps/core/string/HashedString.h>
// #include <mc/world/item/registry/ItemRegistryManager.h>
#include <mc/world/item/registry/ItemRegistry.h>

namespace ItemRegistrar {
    void registerItem(ItemRegistry *registry);
    void setItemsIconInfo();
};
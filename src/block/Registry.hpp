#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include <mc/client/renderer/block/BlockGraphics.h>
#include <mc/deps/core/string/HashedString.h>
#include <mc/deps/json/Value.h>
#include <mc/world/item/CreativeItemInitializer.h>
#include <mc/world/item/ItemInstance.h>
#include <mc/world/item/registry/ItemRegistryManager.h>
#include <mc/world/level/block/BlockShape.h>
#include <mc/world/level/block/BlockType.h>


namespace BlockRegistrar {
void registerBlock(BlockTypeRegistry&, int&);

void addCreativeBlocks(CreativeItemInitializer*);

void registerBlockGraphics();
}; // namespace BlockRegistrar
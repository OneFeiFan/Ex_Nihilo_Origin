#include "Block/Registry.hpp"
#include "FileLogger.h"
#include "Item/Registry.hpp"
#include <ll/api/memory/Hook.h>
#include <mc/client/renderer/block/BlockGraphics.h>
#include <mc/world/item/CreativeItemInitializer.h>
#include <mc/world/item/VanillaItems.h>
#include <mc/world/item/registry/ItemRegistryRef.h>
#include <mc/world/level/block/definition/BlockDefinitionGroup.h>


LL_AUTO_TYPE_STATIC_HOOK(
    registerItemsHook,
    ll::memory::HookPriority::Normal,
    VanillaItems,
    VanillaItems::registerItems,
    void,
    cereal::ReflectionCtx& ctx,
    ItemRegistryRef        itemRegistry,
    BaseGameVersion const& baseGameVersion,
    Experiments const&     experiments
) {
    origin(ctx, itemRegistry, baseGameVersion, experiments);
    auto itemRegistry_ = itemRegistry._lockRegistry();
    ItemRegistrar::registerItem(itemRegistry_.get());
}

LL_AUTO_TYPE_STATIC_HOOK(
    initClientDataHook,
    ll::memory::HookPriority::Normal,
    VanillaItems,
    VanillaItems::initClientData,
    void,
    ItemRegistryRef        itemRegistry,
    BaseGameVersion const& baseGameVersion,
    Experiments const&     experiments
) {
    MyLogger::log("VanillaItems::initClientData called");
    origin(itemRegistry, baseGameVersion, experiments);
    ItemRegistrar::setItemsIconInfo();
}


LL_AUTO_TYPE_INSTANCE_HOOK(
    loadResourcesHook,
    ll::memory::HookPriority::Normal,
    BlockDefinitionGroup,
    &BlockDefinitionGroup::loadResources,
    void,
    ResourcePackManager const&                     resourcePackManager,
    Experiments const&                             experiments,
    Bedrock::NonOwnerPointer<LinkedAssetValidator> validator
) {
    BlockRegistrar::registerBlock(BlockTypeRegistry::get(), this->mLastBlockId);
    origin(resourcePackManager, experiments, validator);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    addLooseBlocksHook,
    ll::memory::HookPriority::Normal,
    CreativeItemInitializer,
    &CreativeItemInitializer::addLooseBlocks,
    void,
    ::std::vector<::BlockDefinition const*> const& blockDefinitions
) {
    origin(blockDefinitions);
    BlockRegistrar::addCreativeBlocks(this);
}

LL_AUTO_TYPE_STATIC_HOOK(
    BlockGraphicsInitBlocksHook,
    ll::memory::HookPriority::Normal,
    BlockGraphics,
    BlockGraphics::initBlocks,
    void,
    ::ResourcePackManager& packManager,
    ::Experiments const&   experiments
) {
    origin(packManager, experiments);
    BlockRegistrar::registerBlockGraphics();
}
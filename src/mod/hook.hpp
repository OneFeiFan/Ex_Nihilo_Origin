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
    // BlockGraphics::mOwnedBlocks();
    // MyLogger::log("BlockGraphics::mBlockLookupMap().size() = " +
    // std::to_string(BlockGraphics::mBlockLookupMap().size()));
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

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     BlockGraphicsHook,
//     ll::memory::HookPriority::Normal,
//     BlockGraphics,
//     &BlockGraphics::$ctor,
//     void*,
//     ::std::string const& nameId
// ) {
//     std::cout << "BlockGraphics::ctor called" << std::endl;
//     return origin(nameId);
// }

// LL_AUTO_TYPE_STATIC_HOOK(
//     BlockGraphicsInitBlocksSymbol,
//     ll::memory::HookPriority::Normal,
//     BlockGraphics,
//     ll::memory::Symbol("?initBlocks@BlockGraphics@@SAXAEAVResourcePackManager@@AEBVExperiments@@@Z"),
//     void,
//     ::ResourcePackManager& packManager,
//     ::Experiments const&   experiments
// ) {
//     MyLogger::log("BlockGraphics::initBlocks called");
//     origin(packManager, experiments);
// }

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

// LL_AUTO_TYPE_STATIC_HOOK(
//     registerLooseBlockGraphicsHook,
//     ll::memory::HookPriority::Normal,
//     BlockGraphics,
//     BlockGraphics::registerLooseBlockGraphics,
//     void,
//     std::unordered_map<::HashedString, ::std::vector<::Json::Value>>& blockDataValuesMap
// ) {
//     origin(blockDataValuesMap);
    
// }
// BlockGraphics::createBlockGraphics
    // MCAPI ::BlockGraphics& setTextureItem(
    //     ::std::string const& nameUp,
    //     ::std::string const& nameDown,
    //     ::std::string const& nameNorth,
    //     ::std::string const& nameSouth,
    //     ::std::string const& nameWest,
    //     ::std::string const& nameEast
    // );

    // LL_AUTO_TYPE_INSTANCE_HOOK(
    //     setTextureItemHook,
    //     ll::memory::HookPriority::Normal,
    //     BlockGraphics,
    //     &BlockGraphics::setTextureItem,
    //     BlockGraphics &,
    //             ::std::string const& nameUp,
    //     ::std::string const& nameDown,
    //     ::std::string const& nameNorth,
    //     ::std::string const& nameSouth,
    //     ::std::string const& nameWest,
    //     ::std::string const& nameEast
    // ){
    //     // MyLogger::log(nameUp);
    //     return origin(nameUp, nameDown, nameNorth, nameSouth, nameWest, nameEast);
        
    // }
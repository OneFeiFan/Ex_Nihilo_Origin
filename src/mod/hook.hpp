#include "Block/Registry.hpp"
#include "FileLogger.h"
#include "Item/Registry.hpp"
#include "World.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mod/FileLogger.h"
#include "utils/TextLocalizer.h"
#include <ll/api/memory/Hook.h>
#include <mc/client/renderer/block/BlockGraphics.h>
#include <mc/locale/I18n.h>
#include <mc/server/ServerLevel.h>
#include <mc/world/gamemode/GameMode.h>
#include <mc/world/gamemode/InteractionResult.h>
#include <mc/world/item/CreativeItemInitializer.h>
#include <mc/world/item/ItemInstance.h>
#include <mc/world/item/VanillaItems.h>
#include <mc/world/item/registry/ItemRegistry.h>
#include <mc/world/item/registry/ItemRegistryRef.h>
#include <mc/world/level/block/definition/BlockDefinitionGroup.h>


LL_AUTO_TYPE_INSTANCE_HOOK(
    initClientHook,
    ll::memory::HookPriority::Normal,
    ItemRegistry,
    &ItemRegistry::initClient,
    void,
    std::vector<ItemData> const& serverItemData,
    Experiments const&           experiments,
    BaseGameVersion const&       baseGameVersion
) {
    TextLocalizer::inject(getI18n());
    origin(serverItemData, experiments, baseGameVersion);
}

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
    MyLogger::log("registerItemsHook");
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

LL_AUTO_TYPE_INSTANCE_HOOK(
    GameModeStartDestroyBlockHook,
    ll::memory::HookPriority::Normal,
    GameMode,
    &GameMode::$startDestroyBlock,
    bool,
    BlockPos const& pos,
    uchar           face,
    bool&           hasDestroyedBlock
) {
    if (this->mPlayer.isClientSide()) {
        return origin(pos, face, hasDestroyedBlock); // 客户端事件
    } else {
        return origin(pos, face, hasDestroyedBlock); // 服务端事件
    }
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    GameModeDestroyBlockHook,
    ll::memory::HookPriority::Normal,
    GameMode,
    &GameMode::$destroyBlock,
    bool,
    BlockPos const& pos,
    uchar           face
) {
    if (this->mPlayer.isClientSide()) {
        return origin(pos, face); // 客户端事件
    } else {
        return origin(pos, face); // 服务端事件
    }
}
// #include "World.h"
LL_AUTO_TYPE_INSTANCE_HOOK(
    useItemOnHook,
    ll::memory::HookPriority::Normal,
    GameMode,
    &GameMode::$useItemOn,
    ::InteractionResult,
    ::ItemStack&      item,
    ::BlockPos const& at,
    uchar             face,
    ::Vec3 const&     hit,
    ::Block const*    targetBlock,
    bool              isFirstEvent
) {

    auto res = origin(item, at, face, hit, targetBlock, isFirstEvent);
    if (this->mPlayer.isClientSide()) {
        return res; // 客户端事件
    } else {
        // auto fblock = BlockTypeRegistry::get().lookupByName("minecraft:stone").get()->mDefaultState;
        // World::setBlock(at.x, at.y, at.z, *fblock, this->mPlayer.getDimensionId());
        // World::destroyBlock(at.x, at.y, at.z, this->mPlayer.getDimensionId());
        // World::drop(at.x, at.y + 1, at.z, item.getId(), 64, 0, this->mPlayer.getDimensionId());
        // World::explode(at.x, at.y, at.z, 5, true, this->mPlayer.getDimensionId());
        // World::setWorldTime(0);
        // WeatherConfig weather(WeatherType::Thunder, 1000);
        // World::setWeather(weather);
        return res; // 服务端事件
    }
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ServerLevelHook,
    ll::memory::HookPriority::Normal,
    ServerLevel,
    &ServerLevel::$ctor,
    void*,
    ::ServerLevelArguments&& args
) {
    void* ori = origin(std::move(args));
    MyLogger::log("ServerLevel::ctor called");
    World::ServerWorld::getInstance().init(this);
    return ori;
}
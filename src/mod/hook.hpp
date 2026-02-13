#include "FileLogger.h"
#include "Item/Registry.hpp"
#include <ll/api/memory/Hook.h>
#include <mc/world/item/VanillaItems.h>
#include <mc/world/item/registry/ItemRegistryRef.h>


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
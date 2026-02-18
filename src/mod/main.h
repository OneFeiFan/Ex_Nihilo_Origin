#pragma once
#include "Block/Blocks.hpp"
#include "Item/Items.hpp"
#include "ll/api/mod/NativeMod.h"
#include "mc/world/level/material/MaterialType.h"
#include <ll/api/mod/NativeMod.h>
#include <mc/world/item/VanillaItemTiers.h>


class ENO {

public:
    static ENO& getInstance();

    ENO() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    [[nodiscard]] bool load() const;

    /// @return True if the mod is enabled successfully.
    [[nodiscard]] bool enable() const;

    /// @return True if the mod is disabled successfully.
    [[nodiscard]] bool disable() const;

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

private:
    ll::mod::NativeMod& mSelf;
};
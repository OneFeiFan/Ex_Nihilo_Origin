//不用完全自定义了，直接修补原版的构造函数


// #pragma once

// #include "mc/common/SharedPtr.h"
// #include "mc/common/WeakPtr.h"
// #include <mc/util/ColorFormat.h>
// #include <mc/locale/I18n.h>
// #include <mc/world/item/enchanting/EnchantUtils.h>
// #include <mc/world/item/enchanting/Enchant.h>
// #include <mc/world/item/ItemTier.h>
// #include <mc/world/item/Item.h>
// #include "mc/world/item/DiggerItem.h"
// #include <mc/world/level/block/Block.h>
// #include <mc/safety/RedactableString.h>

// // auto generated forward declare list
// // clang-format off
// class BaseGameVersion;
// class HashedString;
// class ItemStackBase;
// class Level;
// class RenderParams;
// namespace Bedrock::Safety { class RedactableString; }
// // clang-format on

// class CustomDiggerItem : public ::Item {
// public:
//     // member variables
//     // NOLINTBEGIN
//     ItemTier const* mTier;                // 0x1C8 (注意：用指针或引用)
//     HashedString const* mBlockDestructionTag; // 0x1D0
//     int mAttackDamage;                    // 0x1D8
//     // NOLINTEND
// CustomDiggerItem(const std::string& name, short id, int attackBonus, const ItemTier& tier, const HashedString& tags);
// public:
//     // virtual functions
//     // NOLINTBEGIN
//     virtual int getAttackDamage() const /*override*/;

//     virtual bool isHandEquipped() const /*override*/;

//     virtual int getEnchantValue() const /*override*/;

//     virtual bool isValidRepairItem(
//         ::ItemStackBase const&,
//         ::ItemStackBase const&   repairItem,
//         ::BaseGameVersion const& baseGameVersion
//     ) const /*override*/;

//     virtual void appendFormattedHovertext(
//         ::ItemStackBase const&               stack,
//         ::Level&                             level,
//         ::Bedrock::Safety::RedactableString& hovertext,
//         bool const                           showCategory
//     ) const /*override*/;

//     virtual bool canDestroySpecial(::Block const& block) const /*override*/;

//     virtual float getDestroySpeed(::ItemStackBase const& item, ::Block const& block) const /*override*/;

//     virtual void executeEvent(::ItemStackBase& item, ::std::string const& name, ::RenderParams& params) const
//         /*override*/;

//     virtual ~CustomDiggerItem() /*override*/;
//     // NOLINTEND

// };

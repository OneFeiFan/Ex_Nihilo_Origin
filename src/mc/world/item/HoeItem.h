#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/world/item/DiggerItem.h"

class HoeItem : public ::DiggerItem {
public:
    HoeItem(const std::string&, short, const ItemTier&);
    // virtual functions
    // NOLINTBEGIN
    virtual int getEnchantSlot() const /*override*/;

    virtual ~HoeItem() /*override*/ = default;
    // NOLINTEND

public:
    // virtual function thunks
    // NOLINTBEGIN
    MCFOLD int $getEnchantSlot() const;


    // NOLINTEND

public:
    // vftables
    // NOLINTBEGIN
    MCAPI static void** $vftable();
    // NOLINTEND
};

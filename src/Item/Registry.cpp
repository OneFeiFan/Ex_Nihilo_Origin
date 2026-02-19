#include "Registry.hpp"
#include "Items.hpp"

namespace ItemRegistrar {
void registerItem(ItemRegistry* registry) {
    for (auto wrapper : Items::items) {
        auto item = wrapper->initNativeItem(++registry->mMaxItemID);
        TextLocalizer::registerFullKey("item."+item->mFullName->getString()+".name");
        registry->registerItem(item);
    }
}
void setItemsIconInfo() {
    for (auto wrapper : Items::items) {
        wrapper->setIcon();
    }
}
}; // namespace ItemRegistrar
#include "Registry.hpp"
#include "Blocks.hpp"
#include "mc/world/level/block/Block.h"
#include "mod/FileLogger.h"
#include <mc/world/item/Item.h>


namespace BlockRegistrar {
void registerBlock(BlockTypeRegistry& registry, int& id) {
    for (auto wrapper : Blocks::blocks) {
        auto block =
            registry.registerBlock<BlockType>(wrapper->name, id++, Material::getMaterial(wrapper->materialType));
        block.get()->setCategory(wrapper->category);
    }
}

void addCreativeBlocks(CreativeItemInitializer* creative) {
    for (auto wrapper : Blocks::blocks) {
        auto         item = ItemRegistryManager::getItemRegistry().lookupByName(wrapper->name).get();
        ItemInstance blockItem(*item);
        creative->_addLooseCreativeItem(blockItem);
    }
}

void registerBlockGraphics() {
    // std::unordered_map<::HashedString, ::std::vector<::Json::Value>> blockDataValuesMap;
    for (auto wrapper : Blocks::blocks) {
        auto graphic = BlockGraphics::createBlockGraphics(wrapper->name, BlockShape::Block);
        graphic->setTextureItem(
            wrapper->blockTexture.nameUp,
            wrapper->blockTexture.nameDown,
            wrapper->blockTexture.nameNorth,
            wrapper->blockTexture.nameSouth,
            wrapper->blockTexture.nameWest,
            wrapper->blockTexture.nameEast
        );
        graphic->setCarriedTextureItem(
            wrapper->blockTexture.nameUp,
            wrapper->blockTexture.nameDown,
            wrapper->blockTexture.nameNorth,
            wrapper->blockTexture.nameSouth,
            wrapper->blockTexture.nameWest,
            wrapper->blockTexture.nameEast
        );
        graphic->setDefaultCarriedTextures();
    }
}
}; // namespace BlockRegistrar

#include "Registry.hpp"
#include "mod/FileLogger.h"

namespace BlockRegistrar {
void registerBlock(BlockTypeRegistry& registry, int& id) {
    for (auto wrapper : Blocks::blocks) {
        auto block =
            registry.registerBlock<BlockType>(wrapper->name, id++, Material::getMaterial(wrapper->materialType));
        block->setCategory(wrapper->category);
        TextLocalizer::registerFullKey("tile." + block->mNameInfo->mFullName->getString() + ".name");
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

#include "Blocks.hpp"
#include "mod/FileLogger.h"
#include <string>

namespace Blocks {

std::vector<Blocks::Block*> blocks;

BlockTexture::BlockTexture(
    std::string& nameUp,
    std::string& nameDown,
    std::string& nameNorth,
    std::string& nameSouth,
    std::string& nameWest,
    std::string& nameEast
)
: nameUp(nameUp),
  nameDown(nameDown),
  nameNorth(nameNorth),
  nameSouth(nameSouth),
  nameWest(nameWest),
  nameEast(nameEast) {}

BlockTexture::BlockTexture(
    const char* nameUp,
    const char* nameDown,
    const char* nameNorth,
    const char* nameSouth,
    const char* nameWest,
    const char* nameEast
)
: nameUp(std::string(nameUp)),
  nameDown(std::string(nameDown)),
  nameNorth(std::string(nameNorth)),
  nameSouth(std::string(nameSouth)),
  nameWest(std::string(nameWest)),
  nameEast(std::string(nameEast)) {}

Block::Block(const std::string& name, const MaterialType& materialType, const BlockTexture& blockTexture)
: materialType(materialType),
  blockTexture(blockTexture) {
    this->name     = HashedString(ll::utils::string_utils::toLowerCase(NAME_SPACE ":" + name));
    this->category = CreativeItemCategory::Nature;
    Blocks::blocks.emplace_back(this);
}

Block& Block::setCategory(CreativeItemCategory category) {
    this->category = category;
    return *this;
}

WeakPtr<::BlockType> Block::getBlockType() const { return BlockTypeRegistry::get().lookupByName(name); }
} // namespace Blocks
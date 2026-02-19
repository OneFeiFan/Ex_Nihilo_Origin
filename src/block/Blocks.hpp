#pragma once
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/material/MaterialType.h"
#include <mc/world/level/block/BlockType.h>
#include <string>
#include <vector>


namespace Blocks {

class BlockTexture {
public:
    std::string nameUp;
    std::string nameDown;
    std::string nameNorth;
    std::string nameSouth;
    std::string nameWest;
    std::string nameEast;

    BlockTexture(
        std::string& nameUp,
        std::string& nameDown,
        std::string& nameNorth,
        std::string& nameSouth,
        std::string& nameWest,
        std::string& nameEast
    );

    BlockTexture(
        const char* nameUp,
        const char* nameDown,
        const char* nameNorth,
        const char* nameSouth,
        const char* nameWest,
        const char* nameEast
    );
};

class Block {
public:
    HashedString         name;
    MaterialType         materialType;
    CreativeItemCategory category;
    BlockTexture const   blockTexture;

    Block(const std::string&, const MaterialType&, const BlockTexture&);

    Block& setCategory(CreativeItemCategory);

    [[nodiscard]] WeakPtr<::BlockType> getBlockType() const;

    virtual ~Block() = default;
};

extern std::vector<Blocks::Block*> blocks; // 保持原有容器
} // namespace Blocks
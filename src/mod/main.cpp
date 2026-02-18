// 定义基址偏移
#define REGISTER_BLOCK_OFFSET 0x425DC0


#include "main.h"
#include "FileLogger.h"
#include "hook.hpp"
#include "ll/api/service/Bedrock.h"
#include "mc/deps/core/resource/PackType.h"
#include "mc/resources/IPackLoadContext.h"
#include "mc/resources/IResourcePackRepository.h"
#include <ll/api/Config.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/data/KeyValueDB.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <ll/api/event/player/PlayerUseItemEvent.h>
#include <ll/api/form/ModalForm.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/memory/Memory.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/service/Bedrock.h>
#include <ll/api/utils/StringUtils.h>
#include <mc/locale/I18nImpl.h>
#include <mc/resources/IRepositoryFactory.h>
#include <mc/resources/ResourcePackRepository.h>
#include <mc/server/ServerLevel.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/events/LevelEventListener.h>
#include <mc/world/gamemode/InteractionResult.h>
#include <mc/world/item/BlockItem.h>
#include <mc/world/item/CreativeItemCategory.h>



#include <mc/world/item/ItemStack.h>
#include <mc/world/item/registry/CreativeItemGroupCategory.h>
#include <mc/world/item/registry/CreativeItemRegistry.h>
#include <mc/world/level/ActorBlockSyncMessage.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/IBlockWorldGenAPI.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/WorldBlockTarget.h>

#include <mc/world/level/block/CommandName.h>
#include <mc/world/level/block/StoneBlock.h>
#include <mc/world/level/block/VanillaBlockTags.h>
#include <mc/world/level/block/VanillaBlockTypes.h>
#include <mc/world/level/block/components/BlockComponentFactory.h>
#include <mc/world/level/block/definition/BlockDefinition.h>

#include <mc/world/level/block/definition/BlockDescription.h>
#include <mc/world/level/block/registry/BlockTypeRegistryModificationsLock.h>
#include <mc/world/level/material/Material.h>
#include <mc/world/level/material/MaterialType.h>
#include <windows.h>


ENO& ENO::getInstance() {
    static ENO instance;
    return instance;
}

bool ENO::load() const {
    // getSelf().getLogger().debug("Loading...");
    auto modDir = getSelf().getModDir();
    // 初始化日志系统 (这会清空上一次的 log)
    MyLogger::init(modDir, "debug_output.txt");

    MyLogger::log("模组已加载，日志系统初始化完毕！");

    new Items::Item("test0", "apple");
    (new Items::PickaxeItem("test1", "pickaxe"))->setTier(VanillaItemTiers::DIAMOND());
    (new Items::FoodItem("test2", "apple_golden"))->setNutrition(1).setSaturation("low").setUseDuration(32);
    (new Items::WeaponItem("test3", "sword"))->setTier(VanillaItemTiers::DIAMOND());
    // Material::getMaterial(MaterialType::Solid);
    Blocks::BlockTexture blockTexture("bell_top", "fletching_table_top", "campfire_fire", "gravel", "gravel", "gravel");
    new Blocks::Block("block0", MaterialType::Solid, blockTexture);
    (new Blocks::Block("block1", MaterialType::Solid, blockTexture))->setCategory(CreativeItemCategory::Items);

    return true;
}

bool ENO::enable() const {
    // getSelf().getLogger().debug("Enabling...");
    return true;
}

bool ENO::disable() const {
    // getSelf().getLogger().debug("Disabling...");
    return true;
}

LL_REGISTER_MOD(ENO, ENO::getInstance());

// 以下为试验田，不用看。

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     ResourceInitHook,
//     ll::memory::HookPriority::Normal,
//     ResourcePackRepository,
//     &ResourcePackRepository::$ctor,
//     void*,
//     ::gsl::not_null<::std::shared_ptr<::RepositoryPacks>>                 repositoryPacks,
//     ::PackManifestFactory&                                                manifestFactory,
//     ::Bedrock::NotNullNonOwnerPtr<::IContentAccessibilityProvider> const& contentAccessibility,
//     ::Bedrock::NotNullNonOwnerPtr<::Core::FilePathManager> const&         pathManager,
//     ::Bedrock::NonOwnerPointer<::PackCommand::IPackCommandPipeline>       commands,
//     ::PackSourceFactory&                                                  packSourceFactory,
//     bool                                                                  initAsync,
//     ::std::unique_ptr<::IRepositoryFactory>                               factory
// ) {
//     void* ori = origin(
//         repositoryPacks,
//         manifestFactory,
//         contentAccessibility,
//         pathManager,
//         commands,
//         packSourceFactory,
//         initAsync,
//         std::move(factory)
//     );
//     MyLogger::log("ResourceInitHook");
//     this->addCustomResourcePackPath(ENO::getInstance().getSelf().getModDir() / "ResourcePacks", PackType::Resources);
//     this->addCustomResourcePackPath(ENO::getInstance().getSelf().getModDir() / "BehaviorPacks", PackType::Behavior);
//     return ori;
// }

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     isInitializedHook,
//     ll::memory::HookPriority::Normal,
//     ResourcePackRepository,
//     &ResourcePackRepository::$isInitialized,
//     bool
// ){
//     MyLogger::log("是否已经初始化");
//     return origin();
// }

// 基于LL的1.9.4版本编译

// 下面是直接基于1.21.132.01客户端内部偏移定位的方块注册方法之一，应该等价于registerBlock<BlockType>,旧版本基岩版里面矿石方块会走这个方法注册，现在经过测试应该也是这样的

// 不靠谱的特征码 48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33
// C4 48 89 45 ? 49 8B D9 49 8B F0 4C 8B E9
// 全部匹配的话可以匹配到十几个结果大部分都是registerBlock<xxx>这种模板生成的注册方法，可能的辨别方法是找伪代码是否有形如下面处理
// : 的行为
//                 (char *)v19 + v16,
//                 ":",
//                 1),
using RegisterBlockFunc = BlockType* (*)(BlockTypeRegistry & registry,
                                         const HashedString& name,
                                         int*                blockId,
                                         const Material&     material);

BlockType* CallRegisterBlock(
    BlockTypeRegistry&  registryInstance,
    const HashedString& name,
    int*                blockId,
    const Material&     material
) {
    static const uintptr_t offset     = 0x425DC0;
    static const uintptr_t base       = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
    static const uintptr_t targetAddr = base + offset;
    auto                   func       = reinterpret_cast<RegisterBlockFunc>(targetAddr);

    return func(registryInstance, name, blockId, material);
}

// LL_AUTO_STATIC_HOOK(
//     VanillaBlockTypesRegisterBlocksHook,
//     ll::memory::HookPriority::Normal,
//     VanillaBlockTypes::registerBlocks,
//     void,
//     ::BaseGameVersion const& baseGameVersion,
//     ::Experiments const&     experiments
// ) {
//     MyLogger::log("VanillaBlockTypes::registerBlocks called");
//     origin(baseGameVersion, experiments);
//     auto&               registry = BlockTypeRegistry::get();
//     const Material&     material = Material::getMaterial(MaterialType::Solid);
//     int                 id       = 10000; // 这个id是瞎写的，不确定原版的id是怎么确定的。。。
//     const HashedString& name("my:custom_block");
//     WeakPtr<BlockType>  blockptr = registerBlock<BlockType>(&registry, name, id, material);
//     BlockType*          block    = blockptr.get();
//     block->setCategory(CreativeItemCategory::Nature);
// }

// hook了刚才的偏移定位的注册方法，通过自定义模板注册，发现原版方块正常注册
// blockId是指针类型，如果不传入指针，直接int
// blockId基本上方块可以注册（建议超平坦测试防止崩溃），但是会出现很多异常情况，比如矿石没注册全，命令方块出现在了不该出现的位置
// 由此可以通过反证法得出偏移找对了，模板也基本没问题
//  LL_AUTO_STATIC_HOOK(
//      RegisterBlockHook,
//      ll::memory::HookPriority::Normal,
//      reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + REGISTER_BLOCK_OFFSET,
//      BlockType*,
//      BlockTypeRegistry*  ptr,
//      const HashedString& name,
//      int*                blockId,
//      const Material&     material
//  ) {
//      WeakPtr<BlockType> res = registerBlock<BlockType>(ptr, name, *blockId, material);
//      return res.get();
//  }
// MCAPI static void VanillaItems::registerItems(
//     ::cereal::ReflectionCtx& ctx,
//     ::ItemRegistryRef        itemRegistry,
//     ::BaseGameVersion const& baseGameVersion,
//     ::Experiments const&     experiments
// );

#include <mc/world/level/block/definition/BlockDescription.h>
#include <mc/world/level/block/definition/BlockMenuCategory.h>
#include <mc/world/level/block/definition/BlockStateDefinition.h>
#include <mc/world/level/block/registry/VanillaBlockData.h>
#include <mc/world/level/block/traits/block_trait/ITrait.h>

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     BlockSourceSetBlockHook,
//     ll::memory::HookPriority::Normal,
//     BlockSource,
//     &BlockSource::$setBlock,
//     bool,
//     ::BlockPos const&              pos,
//     ::Block const&                 block,
//     int                            updateFlags,
//     ::ActorBlockSyncMessage const* syncMsg,
//     ::BlockChangeContext const&    changeSourceContext
// ) {
//     MyLogger::log("Item::useOn called");


//     // BlockSource blockSource = entity.getDimensionBlockSource();
//     // ActorBlockSyncMessage syncMsg;
//     // syncMsg.mEntityUniqueID = entity.getSourceUniqueID();
//     // syncMsg.mMessage = ActorBlockSyncMessage::MessageId::Create;
//   auto&               blockRegistry = BlockTypeRegistry::get();
//   auto               itemRegistry = ItemRegistryManager::getItemRegistry();

//     const HashedString& name("my:custom_block");
//     auto  bid = blockRegistry.lookupByName(name).get()->mID->mValue;
//     auto iid = itemRegistry.lookupByName(name).get()->mId;
//     auto test = itemRegistry.lookupByName(name).get()->mBlockType;
//     MyLogger::log("方块id:" + std::to_string(bid));
//     MyLogger::log("物品id:" + std::to_string(iid));
//     if(test->get()){
//         MyLogger::log("方块存在");
//     }

//    return origin(pos, block,updateFlags, syncMsg, changeSourceContext);

//     // return origin(pos, block, updateFlags, syncMsg, changeSourceContext);
// }


// LL_AUTO_TYPE_INSTANCE_HOOK(
//     BlockOnPlaceHook,
//     ll::memory::HookPriority::Normal,
//     Block,
//     &Block::onPlace,
//     void,
//     BlockSource& region,
//     BlockPos const& pos,
//     Block const& previousBlock
// ) {

//     MyLogger::log("Block::onPlace called");
//     auto&               registry = BlockTypeRegistry::get();

//     const HashedString& name("my:custom_block");
//     auto * test = registry.lookupByName(name).get()->mDefaultState;
//     origin(region, pos, *(test));
//     // MyLogger::log(test->mNameInfo->mRawName->getString());
//     // test->getRenderBlock().onPlace(region, pos, previousBlock);
// }
// #include <mc/resources/PackLoadContext.h>
// #include <mc/resources/PackInstance.h>
// LL_AUTO_TYPE_INSTANCE_HOOK(
//     _loadBlockDescriptionHook,
//     ll::memory::HookPriority::Normal,
//     BlockDefinitionGroup,
//     &BlockDefinitionGroup::_loadBlockDescription,
//     bool,
//     ::Json::Value const&      root,
//     ::BlockDescription&       desc,
//     ::JsonBetaState&          canUseBeta,
//     ::IPackLoadContext const& packLoadContext,
//     ::SemVersion const&       jsonVersion
// ) {
//     MyLogger::log("BlockDefinitionGroup::_loadBlockDescription called");
//     return origin(root, desc, canUseBeta, packLoadContext, jsonVersion);
// }

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     BlockDefinitionLoaderConstructorHook,
//     ll::memory::HookPriority::Normal,
//     BlockComponentFactory,
//     ll::memory::Symbol("??0BlockComponentFactory@@QEAA@PEBVIPackLoadContext@@@Z"),
//     void,
//     IPackLoadContext const* packLoadContext
// ) {
//     // 在这里添加你的hook逻辑
//     MyLogger::log("BlockDefinitionLoader constructor called");
//     // MyLogger::log("Resource pack location: " + resourcePackLocation);

//     // 调用原始构造函数
//     origin(packLoadContext);
// }


// LL_AUTO_TYPE_INSTANCE_HOOK(
//     ItemRegistryInitServerHook,
//     ll::memory::HookPriority::Normal,
//     ItemRegistry,
//     &ItemRegistry::initServer,
//     void,
//     Experiments const&                                   experiments,
//     BaseGameVersion const&                               baseGameVersion,
//     ResourcePackManager*                                 rpm,
//     Bedrock::NonOwnerPointer<class LinkedAssetValidator> validator,
//     IMinecraftEventing&                                  eventing
// ) {
//     origin(experiments, baseGameVersion, rpm, validator, eventing);
// }

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     initializeHook,
//     ll::memory::HookPriority::Normal,
//     ServerLevel,
//     &ServerLevel::$initialize,
//     bool,
//     std::string const&   levelName,
//     LevelSettings const& levelSettings,
//     Experiments const&   experiments,
//     std::string const*   levelId,
//     std::optional<std::reference_wrapper<
//         std::unordered_map<std::string, std::unique_ptr<BiomeJsonDocumentGlue::ResolvedBiomeData>>>>
//         biomeIdToResolvedData
// ) {
//     bool result = origin(levelName, levelSettings, experiments, levelId, biomeIdToResolvedData);
//     MyLogger::log("ServerLevel::initialize called");
//     return result;
// }

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     LevelinitializeHook,
//     ll::memory::HookPriority::Normal,
//     Level,
//     &Level::$initialize,
//     bool,
//     std::string const&   levelName,
//     LevelSettings const& levelSettings,
//     Experiments const&   experiments,
//     std::string const*   levelId,
//     std::optional<std::reference_wrapper<
//         std::unordered_map<std::string, std::unique_ptr<BiomeJsonDocumentGlue::ResolvedBiomeData>>>>
//         biomeIdToResolvedData
// ) {
//     bool result = origin(levelName, levelSettings, experiments, levelId, biomeIdToResolvedData);
//     MyLogger::log("Level::initialize called");
//     return result;
// }
#include <mc/nbt/CompoundTag.h>
#include <mc/nbt/Tag.h>
#include <mc/world/item/ItemStackBase.h>
//  MCAPI void ItemStackBase::setUserData(::std::unique_ptr<::CompoundTag> tag);
// LL_AUTO_TYPE_INSTANCE_HOOK(
//     setUserDataHook,
//     ll::memory::HookPriority::Normal,
//     ItemStackBase,
//     &ItemStackBase::setUserData,
//     void,
//     ::std::unique_ptr<::CompoundTag> tag
// ){
//     // MyLogger::log("ItemStackBase::setUserData called");

//    if (tag) {
//         // tag 不是空指针，可以安全读取

//         // 方案 A: 直接打印整个 NBT 结构 (最推荐，简单直观)
//         // CompoundTag 已经实现了 toString，会自动递归打印所有内容
//         // MyLogger::log("Setting NBT: " + tag->toString());

//         // 方案 B: 如果你非要遍历 (注意：tag-> 直接访问即可，不需要 clone)

//         for (const auto& [key, variant] : *tag) {
//            std::string text = tag->toSnbt();
//            MyLogger::log(text);
//             // 打印值需要处理 variant，toString() 最方便
//         }

//     } else {
//         // tag 是空的，意味着游戏正在清除这个物品的 NBT
//         // MyLogger::log("Clearing NBT (tag is null)");
//     }

//     // for (const auto& [key, variant] : *data) {
//     //     // key 是 std::string
//     //     // variant 是 CompoundTagVariant (虽然头文件没给出定义，但通常它是一个包含具体 Tag 指针或引用的变体)

//     //     // std::cout << "Key: " << key << " -> ";
//     //     // MyLogger::log("Key: " + key + " -> ");
//     //     // 注意：CompoundTagVariant 具体怎么打印取决于它的实现。
//     //     // 但通常变体也会有 toString() 或者你需要根据类型访问 raw tag
//     //     // 假设 variant 可以直接转换或访问：

//     //     // 这里的处理取决于 CompoundTagVariant.h，但通常你可以这样递归调用：
//     //     // std::cout << variant.toString() << std::endl;

//     //     // 或者简单地打印键名
//     //     // std::cout << "[Value Type]" << std::endl;
//     // }
//     origin(std::move(tag));
// }
// namespace  Item{
// struct Tier {
// public:
//     // member variables
//     // NOLINTBEGIN
//     ::ll::TypedStorage<4, 4, int>   mLevel;
//     ::ll::TypedStorage<4, 4, int>   mUses;
//     ::ll::TypedStorage<4, 4, float> mSpeed;
//     ::ll::TypedStorage<4, 4, int>   mDamage;
//     ::ll::TypedStorage<4, 4, int>   mEnchantmentValue;
//     // NOLINTEND
// };
// }
// template <>
// WeakPtr<Item> ItemRegistry::registerItemShared<Item, short&>(HashedString const& name, short& id) {
//     // 你的实现
//     return WeakPtr<Item>();
// }
// #include <mc/world/item/PickaxeItem.h>
// using testptr = WeakPtr<Item> (ItemRegistry::*)(HashedString const&, short&);

// LL_AUTO_TYPE_INSTANCE_HOOK(
//     ItemRegistry_registerPickaxe_Hook,            // 1. DEF_TYPE: 你的 Hook 命名（随便起，不重复即可）
//     ll::memory::HookPriority::Normal,             // 2. PRIORITY: 优先级
//     PickaxeItem,                                 // 3. TYPE: 目标类名
//     ll::memory::Symbol("??0PickaxeItem@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HAEBVItemTier@@@Z"),
//     void,                         // 5. RET_TYPE: 返回值类型
//     std::string const& name,
//     short & id,
//     ItemTier const& t
// ) {
//     MyLogger::log("Registering Pickaxe: " + name);
//     // --- 这里是你的 Hook 逻辑 (Detour) ---

//     // 示例：在注册镐子前，打印它的名字和ID
//     // std::cout << "Registering Pickaxe: " << name.getString() << " with ID: " << id << std::endl;

//     // 你也可以在这里修改传入的参数引用，比如强制修改 ID (不推荐随便改，容易崩溃)

//     // 调用原函数并拿到返回值 (必须 return)
//     // WeakPtr<PickaxeItem> result = origin(name, id, tier);

//     // 你也可以在 return 前对 result 做一些后置处理
//      origin(name,id,t);
// }
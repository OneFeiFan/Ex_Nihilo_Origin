//不用完全自定义了，直接修补原版的构造函数

// #include "CustomDiggerItem.h"
// #include "mc/deps/core/string/HashedString.h"
// #include "mc/world/item/enchanting/Enchant.h"
// #include "mod/FileLogger.h"
// #include <string>

// CustomDiggerItem::CustomDiggerItem(const std::string& name, short id, int attackBonus, const ItemTier& tier, const HashedString& tags) 
//         : Item(name, id) // 1. 调用父类 Item 的构造函数 (这个通常没被内联，可以调用)
//     {
//         this->mTier = &tier;
//         this->mBlockDestructionTag = &tags;
//         this->mAttackDamage = tier.mDamage + attackBonus; 
//         this->mCreativeCategory = CreativeItemCategory::Equipment; 
//         this->mHandEquipped = true; 

//         // 4. 调用父类方法完成最后设置
//         this->setMaxDamage(tier.mUses);//最大耐久
//         this->mMaxStackSize = 1; // 工具和武器必须只能堆叠 1 个
//         // 理论上还需要打上标签，原版是 VanillaItemTags::Digger
//         // 如果你的框架支持，可以直接调用 addTag
//         // this->addTag(HashedString("minecraft:is_pickaxe")); 
//         // this->addTag(HashedString("minecraft:is_tool")); 
//         this->addTag(HashedString("minecraft:digger"));
//     }

// // ==========================================
// // 1. 基础属性获取
// // ==========================================

// // 获取工具的攻击力
// int CustomDiggerItem::getAttackDamage() const /*override*/ {
//     // 之前构造函数里计算好的: mAttackDamage = mTier.mDamage + attackBonus
//     return this->mAttackDamage; 
// }

// // 是否像工具/武器一样拿在手上？
// bool CustomDiggerItem::isHandEquipped() const /*override*/ {
//     // 挖掘工具（镐、斧、锹）在第三人称渲染时，都是垂直握持的。
//     // 返回 true 会让物品渲染成工具的握持姿势，而不是像苹果一样平举着。
//     MyLogger::log("工具握持");
//     return true; 
// }

// // 获取附魔价值 (决定附魔台能刷出多好的属性)
// int CustomDiggerItem::getEnchantValue() const /*override*/ {
//     // 直接返回我们之前在 ItemTier 里解析出来的那个隐藏属性！
//     // 比如金质工具这个值很高，石头工具很低。
//     return this->mTier->mEnchantmentValue; 
// }

// // ==========================================
// // 2. 核心机制：挖掘与破坏
// // ==========================================

// // 判断是否能“特殊破坏”这个方块 (即是否能掉落物品，比如铁镐挖金矿)
// bool CustomDiggerItem::canDestroySpecial(::Block const& block) const /*override*/ {
//     // 原版逻辑：检查被挖的方块是否拥有当前工具的破坏标签
//     // mBlockDestructionTag 通常是 "minecraft:mineable/pickaxe" 之类的标签
//     // 如果方块带有这个标签，且工具等级够，就能破坏并掉落。
//     // 注意：具体判断函数名取决于你的 BDS 框架，这里用 hasTag 作为示意
//     MyLogger::log("检查方块标签");
//     if (block.hasTag(*this->mBlockDestructionTag)) {
//         return true;
//     }
    
//     // 如果没标签，调用父类 Item 的默认判断
//     return ::Item::canDestroySpecial(block); 
// }

// // 获取破坏速度
// float CustomDiggerItem::getDestroySpeed(::ItemStackBase const& item, ::Block const& block) const /*override*/ {
//     // 1. 首先检查这把工具对付这个方块是否“专业对口”
//     // 如果方块属于这个工具能破坏的类型 (比如用镐子敲石头)
//     MyLogger::log("检查工具专业对口");
//     if (this->canDestroySpecial(block)) {
//         // 返回 Tier 里设定的速度！比如钻石大概是 8.0f，木头是 2.0f
//         return this->mTier->mSpeed; 
//     }
    
//     // 2. 如果不对口 (比如用镐子敲树)
//     // 速度就是普通手的速度，即 1.0f
//     return 1.0f; 
// }

// // ==========================================
// // 3. 物品维护与 UI 显示
// // ==========================================

// // 判断在铁砧里能否用 repairItem 修复当前工具
// bool CustomDiggerItem::isValidRepairItem(
//     ::ItemStackBase const& item,
//     ::ItemStackBase const& repairItem,
//     ::BaseGameVersion const& baseGameVersion
// ) const /*override*/ {
//     // 原版逻辑极其巧妙：它通常不会在 DiggerItem 里写死，
//     // 而是委托给 Tier 或组件去判断 repairItem 的材质是否匹配（比如钻石镐认钻石）。
//     // 在纯净实现中，你可能需要比对 repairItem 的名字或 tag。
    
//     // 伪代码示例：
//     // if (repairItem.hasTag("minecraft:is_repair_material_" + this->mTierName)) return true;
    
//     // 如果你没有特别的自定义修复需求，可以直接调用父类
//     return ::Item::isValidRepairItem(item, repairItem, baseGameVersion);
// }

// // 追加物品的悬浮提示文字 (Hover Text)
// void CustomDiggerItem::appendFormattedHovertext(
//     ::ItemStackBase const& stack,
//     ::Level& level,
//     ::Bedrock::Safety::RedactableString& hovertext,
//     bool const showCategory
// ) const /*override*/ {
    
//     //     if (this->mAttackDamage > 0) {
//     //         MyLogger::log(hovertext.mUnredactedString);
//     //         MyLogger::log(*hovertext.mRedactedString);
//     //     std::string damageStr = "\n§9+" + std::to_string(this->mAttackDamage) + " Attack Damage§r";
//     //     hovertext.append(damageStr);
//     // }
//     // 1. 先让父类把基础信息（名字、耐久度）拼上去
//     ::Item::appendFormattedHovertext(stack, level, hovertext, showCategory);
// if (this->mAttackDamage > 0) {
//     // 2. 计算附魔带来的额外伤害
//         // 9 代表 Sharpness (锋利) 附魔
//         // 1.25 是该版本中每级锋利增加的伤害值
//         int sharpLvl = EnchantUtils::getEnchantLevel(Enchant::Type::Sharpness, stack);
//         float bonusDamage = (float)sharpLvl * 1.25f;

//         // 3. 计算总伤害
//         // 基础伤害 + 附魔伤害
//         float totalDamageFloat = (float)this->mAttackDamage + bonusDamage;
//         int totalDamage = (int)totalDamageFloat;

//         // 4. 获取本地化字符串
//         // 获取 "攻击伤害" 的翻译文本 (attribute.name.generic.attackDamage)
//         auto& i18n = getI18n();
//         std::string attrName = i18n.get("attribute.name.generic.attackDamage",i18n.getCurrentLanguage());

//         // 5. 格式化数值字符串
//         // 将数值转为字符串，并在前面加上 "+"
//         std::string valStr = std::to_string(totalDamage);
//         valStr.insert(0, "+"); // 变成 "+7" 这种格式
//         valStr += " ";         // 在数字后加一个空格，变成 "+7 "

//         // 6. 构建最终显示的文本行
//         // 原始代码中的 2570 (0x0A0A) 代表两个换行符 \n\n
//         // 这里的逻辑是：换行 -> 蓝色字体 -> 数值 -> 属性名
        
//         std::string finalLine;
//         finalLine.append("\n");             // 或者是 "\n\n"，用于与上方文本隔开
//         finalLine.append(ColorFormat::BLUE); // 设置颜色为蓝色
//         finalLine.append(valStr);            // 添加 "+7 "
//         finalLine.append(attrName);          // 添加 "攻击伤害"

//         // 最终效果类似： 
//         // [空行]
//         // [蓝色] +7 攻击伤害

//         // 7. 将构建好的行追加到悬浮提示中
//         hovertext.append(finalLine);
// }

// }

// // ==========================================
// // 4. 事件与析构
// // ==========================================

// // 执行物品相关的事件 (如原版数据驱动机制中的 events)
// void CustomDiggerItem::executeEvent(
//     ::ItemStackBase& item, 
//     ::std::string const& name, 
//     ::RenderParams& params
// ) const /*override*/ {
//     // 这个通常是留给 JSON 数据驱动组件用的。
//     // 对于写死的代码逻辑工具，通常直接抛给基类处理即可。
//     ::Item::executeEvent(item, name, params);
// }

// // 析构函数
// CustomDiggerItem::~CustomDiggerItem() /*override*/ {
//     // 默认析构即可。因为我们的 mTier 和 mBlockDestructionTag 
//     // 在 PC 原版中定义为 const&（或者用指针），它们只是对外部常量的引用，
//     // 生命周期不由 DiggerItem 管理，所以不需要在这里 delete。
// }
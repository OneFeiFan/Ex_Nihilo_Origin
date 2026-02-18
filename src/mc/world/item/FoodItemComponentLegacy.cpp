#include "mc/world/item/FoodItemComponentLegacy.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include <mc/world/item/Item.h>


FoodItemComponentLegacy::FoodItemComponentLegacy(Item& owner) : mOwner(owner) {
    mNutrition          = 0;                 // ok
    mSaturationModifier = 0.6f;              // ok
    mUsingConvertsTo    = "";                // 吃完后item变成什么，默认空
    mOnUseAction        = OnUseAction::None; // 吃完后的特殊动作
    mOnUseRange         = Vec3(0, 0, 0);     // ok
    mCooldownCategory   = HashedString("");  // 冷却分类默认空
    mCooldownDuration   = 0;                 // 冷却时间默认0
    mCanAlwaysEat       = false;
    mEffects            = std::vector<Effect>(); // 效果
    mRemoveEffects      = std::vector<uint>();   // 移除效果
}
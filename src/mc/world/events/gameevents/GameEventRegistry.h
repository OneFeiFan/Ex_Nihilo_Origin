#pragma once
#include <mc/world/events/gameevents/GameEvent.h>

namespace GameEventRegistry {

    // ==========================================
    // 0: 基础/无效事件
    // ==========================================
    inline constexpr GameEvent Invalid{ GameEventConfig::GameEventType::Invalid, 0 };

    // ==========================================
    // 1 - 12: 方块与容器操作
    // ==========================================
    inline constexpr GameEvent BlockActivate{ GameEventConfig::GameEventType::BlockActivate, 1 };
    inline constexpr GameEvent BlockAttach{ GameEventConfig::GameEventType::BlockAttach, 1 };
    inline constexpr GameEvent BlockChange{ GameEventConfig::GameEventType::BlockChange, 1 };
    inline constexpr GameEvent BlockClose{ GameEventConfig::GameEventType::BlockClose, 1 };
    inline constexpr GameEvent BlockDeactivate{ GameEventConfig::GameEventType::BlockDeactivate, 1 };
    inline constexpr GameEvent BlockDestroy{ GameEventConfig::GameEventType::BlockDestroy, 9 };    // 无视潜行
    inline constexpr GameEvent BlockDetach{ GameEventConfig::GameEventType::BlockDetach, 1 };
    inline constexpr GameEvent BlockOpen{ GameEventConfig::GameEventType::BlockOpen, 1 };
    inline constexpr GameEvent BlockPlace{ GameEventConfig::GameEventType::BlockPlace, 9 };      // 无视潜行
    inline constexpr GameEvent ContainerClose{ GameEventConfig::GameEventType::ContainerClose, 1 };
    inline constexpr GameEvent ContainerOpen{ GameEventConfig::GameEventType::ContainerOpen, 1 };
    inline constexpr GameEvent DispenseFail{ GameEventConfig::GameEventType::DispenseFail, 1 };

    // ==========================================
    // 13 - 15: 玩家/实体消耗与状态
    // ==========================================
    inline constexpr GameEvent Drink{ GameEventConfig::GameEventType::Drink, 1 };
    inline constexpr GameEvent Eat{ GameEventConfig::GameEventType::Eat, 1 };
    inline constexpr GameEvent ElytraGlide{ GameEventConfig::GameEventType::ElytraGlide, 1 };

    // ==========================================
    // 16 - 27: 实体核心行为
    // ==========================================
    inline constexpr GameEvent EntityDamage{ GameEventConfig::GameEventType::EntityDamage, 1 };
    inline constexpr GameEvent EntityDie{ GameEventConfig::GameEventType::EntityDie, 1 };
    inline constexpr GameEvent EntityDismount{ GameEventConfig::GameEventType::EntityDismount, 1 };
    inline constexpr GameEvent EntityInteract{ GameEventConfig::GameEventType::EntityInteract, 1 };
    inline constexpr GameEvent EntityMount{ GameEventConfig::GameEventType::EntityMount, 1 };
    inline constexpr GameEvent EntityMove{ GameEventConfig::GameEventType::EntityMove, 7 };      // 受潜行影响
    inline constexpr GameEvent EntityPlace{ GameEventConfig::GameEventType::EntityPlace, 1 };
    inline constexpr GameEvent EntityRoar{ GameEventConfig::GameEventType::EntityRoar, 1 };
    inline constexpr GameEvent EntityAct{ GameEventConfig::GameEventType::EntityAct, 1 };
    inline constexpr GameEvent Equip{ GameEventConfig::GameEventType::Equip, 1 };
    inline constexpr GameEvent Explode{ GameEventConfig::GameEventType::Explode, 1 };
    inline constexpr GameEvent Flap{ GameEventConfig::GameEventType::Flap, 1 };

    // ==========================================
    // 28 - 40: 流体、红石与特殊交互
    // ==========================================
    inline constexpr GameEvent FluidPickup{ GameEventConfig::GameEventType::FluidPickup, 1 };
    inline constexpr GameEvent FluidPlace{ GameEventConfig::GameEventType::FluidPlace, 1 };
    inline constexpr GameEvent HitGround{ GameEventConfig::GameEventType::HitGround, 7 };        // 受潜行影响
    inline constexpr GameEvent ItemInteractFinish{ GameEventConfig::GameEventType::ItemInteractFinish, 3 };
    inline constexpr GameEvent ItemInteractStart{ GameEventConfig::GameEventType::ItemInteractStart, 0 };
    inline constexpr GameEvent LightningStrike{ GameEventConfig::GameEventType::LightningStrike, 1 };
    inline constexpr GameEvent MultiItemSwap{ GameEventConfig::GameEventType::MultiItemSwap, 1 };
    inline constexpr GameEvent NoteBlockPlay{ GameEventConfig::GameEventType::NoteBlockPlay, 1 };
    inline constexpr GameEvent PistonContract{ GameEventConfig::GameEventType::PistonContract, 1 };
    inline constexpr GameEvent PistonExtend{ GameEventConfig::GameEventType::PistonExtend, 1 };
    inline constexpr GameEvent PrimeFuse{ GameEventConfig::GameEventType::PrimeFuse, 1 };
    inline constexpr GameEvent ProjectileLand{ GameEventConfig::GameEventType::ProjectileLand, 9 };  // 极易吸引 Warden
    inline constexpr GameEvent ProjectileShoot{ GameEventConfig::GameEventType::ProjectileShoot, 3 };

    // ==========================================
    // 41 - 55: 紫水晶共振系统 (完整 1-15 级)
    // ==========================================
    inline constexpr GameEvent Resonate1{ GameEventConfig::GameEventType::Resonate1, 1 };
    inline constexpr GameEvent Resonate2{ GameEventConfig::GameEventType::Resonate2, 1 };
    inline constexpr GameEvent Resonate3{ GameEventConfig::GameEventType::Resonate3, 1 };
    inline constexpr GameEvent Resonate4{ GameEventConfig::GameEventType::Resonate4, 1 };
    inline constexpr GameEvent Resonate5{ GameEventConfig::GameEventType::Resonate5, 1 };
    inline constexpr GameEvent Resonate6{ GameEventConfig::GameEventType::Resonate6, 1 };
    inline constexpr GameEvent Resonate7{ GameEventConfig::GameEventType::Resonate7, 1 };
    inline constexpr GameEvent Resonate8{ GameEventConfig::GameEventType::Resonate8, 1 };
    inline constexpr GameEvent Resonate9{ GameEventConfig::GameEventType::Resonate9, 1 };
    inline constexpr GameEvent Resonate10{ GameEventConfig::GameEventType::Resonate10, 1 };
    inline constexpr GameEvent Resonate11{ GameEventConfig::GameEventType::Resonate11, 1 };
    inline constexpr GameEvent Resonate12{ GameEventConfig::GameEventType::Resonate12, 1 };
    inline constexpr GameEvent Resonate13{ GameEventConfig::GameEventType::Resonate13, 1 };
    inline constexpr GameEvent Resonate14{ GameEventConfig::GameEventType::Resonate14, 1 };
    inline constexpr GameEvent Resonate15{ GameEventConfig::GameEventType::Resonate15, 1 };

    // ==========================================
    // 56 - 64: 幽匿系统专属及杂项
    // ==========================================
    inline constexpr GameEvent SculkTouch{ GameEventConfig::GameEventType::SculkTouch, 17 };         // 特殊标识: 0x11
    inline constexpr GameEvent SculkSensorTendrilsClicking{ GameEventConfig::GameEventType::SculkSensorTendrilsClicking, 0 };
    inline constexpr GameEvent Shear{ GameEventConfig::GameEventType::Shear, 1 };
    inline constexpr GameEvent Shriek{ GameEventConfig::GameEventType::Shriek, 0 };
    inline constexpr GameEvent SingleItemSwap{ GameEventConfig::GameEventType::SingleItemSwap, 1 };
    inline constexpr GameEvent Splash{ GameEventConfig::GameEventType::Splash, 1 };
    inline constexpr GameEvent Swim{ GameEventConfig::GameEventType::Swim, 3 };
    inline constexpr GameEvent Teleport{ GameEventConfig::GameEventType::Teleport, 1 };
    inline constexpr GameEvent Unequip{ GameEventConfig::GameEventType::Unequip, 1 };

} // namespace GameEventRegistry
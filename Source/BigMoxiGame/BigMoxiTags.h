// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <NativeGameplayTags.h>

/** Declare the Platform trait GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Platform_Trait);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Platform_Trait_DedicatedGamepad);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Platform_Trait_Gamepad);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Platform_Trait_MouseAndKeyboard);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Platform_Trait_Touch);

/** Declare the Input GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Character_Look);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Character_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Weapon_Shoot);

/** Declare the Ability GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_Shoot);

/** Declare the Character States GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Alive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Dead);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Jumping);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_HasWeapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Shooting);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_WeaponCooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_HasFlag);

/** Declare the Effects GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_Init)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_Death)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_InitAttributes)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_HasWeapon)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Weapon_Damage)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Weapon_Cooldown)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_HasFlag)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Character_DroppedFlag)

/** Declare the Cues GameplayTags. */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cue_Weapon_Shoot)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cue_Weapon_Hit)

// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiTags.h"

// Define the Platform trait GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Platform_Trait, "Platform.Trait");
UE_DEFINE_GAMEPLAY_TAG(Platform_Trait_DedicatedGamepad, "Platform.Trait.Input.HasDedicatedGamepad");
UE_DEFINE_GAMEPLAY_TAG(Platform_Trait_Gamepad, "Platform.Trait.Input.SupportsGamepad");
UE_DEFINE_GAMEPLAY_TAG(Platform_Trait_MouseAndKeyboard, "Platform.Trait.Input.SupportsMouseAndKeyboard");
UE_DEFINE_GAMEPLAY_TAG(Platform_Trait_Touch, "Platform.Trait.Input.SupportsTouch");

// Define the Input GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Input_Character_Look, "Input.Character.Look");
UE_DEFINE_GAMEPLAY_TAG(Input_Character_Move, "Input.Character.Move");
UE_DEFINE_GAMEPLAY_TAG(Input_Weapon_Shoot, "Input.Weapon.Shoot");

// Define the Ability GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Ability_Character_Jump, "Ability.Input.Character.Jump");
UE_DEFINE_GAMEPLAY_TAG(Ability_Weapon_Shoot, "Ability.Input.Weapon.Shoot");

// Define the Character States GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Character_State_Alive, "Character.State.Alive");
UE_DEFINE_GAMEPLAY_TAG(Character_State_Dead, "Character.State.Dead");
UE_DEFINE_GAMEPLAY_TAG(Character_State_Jumping, "Character.State.Jumping");
UE_DEFINE_GAMEPLAY_TAG(Character_State_HasWeapon, "Character.State.HasWeapon");
UE_DEFINE_GAMEPLAY_TAG(Character_State_Shooting, "Character.State.Shooting");
UE_DEFINE_GAMEPLAY_TAG(Character_State_WeaponCooldown, "Character.State.WeaponCooldown");
UE_DEFINE_GAMEPLAY_TAG(Character_State_HasFlag, "Character.State.HasFlag");

// Define the Effects GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_Init, "Effect.Character.Init");
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_Death, "Effect.Character.Death");
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_InitAttributes, "Effect.Character.InitAttributes");
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_HasWeapon, "Effect.Character.HasWeapon");
UE_DEFINE_GAMEPLAY_TAG(Effect_Weapon_Damage, "Effect.Weapon.Damage");
UE_DEFINE_GAMEPLAY_TAG(Effect_Weapon_Cooldown, "Effect.Weapon.Cooldown");
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_HasFlag, "Effect.Character.HasFlag");
UE_DEFINE_GAMEPLAY_TAG(Effect_Character_DroppedFlag, "Effect.Character.DroppedFlag");

// Define the Effects GameplayTags.
UE_DEFINE_GAMEPLAY_TAG(Cue_Weapon_Shoot, "GameplayCue.Weapon.Shoot");
UE_DEFINE_GAMEPLAY_TAG(Cue_Weapon_Hit, "GameplayCue.Weapon.Hit");

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ERPAbilityInputID : uint8
{
	None				UMETA(DisplayName = "None"),
	Confirm				UMETA(DisplayName = "Confirm"),
	Cancel				UMETA(DisplayName = "Cancel"),
	Sprint				UMETA(DisplayName = "Sprint"),
	Jump				UMETA(DisplayName = "Jump"),
	LightAttack			UMETA(DisplayName = "LightAttack"),
	HeavyAttack			UMETA(DisplayName = "HeavyAttack"),
	Block				UMETA(DisplayName = "Secondary Fire"),
	ActiveAbility		UMETA(DisplayName = "Alternate Fire"),
	Reload				UMETA(DisplayName = "Reload"),
	NextWeapon			UMETA(DisplayName = "Next Weapon"),
	PrevWeapon			UMETA(DisplayName = "Previous Weapon"),
	Interact			UMETA(DisplayName = "Interact"),
	Dodge				UMETA(DisplayName = "Dodge")
	
};

UENUM(BlueprintType)
enum class ERPHitReactDirection : uint8
{
	None	UMETA(DisplayName = "None"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	Front	UMETA(DisplayName = "Front"),
	Back	UMETA(DisplayName = "Back")
};

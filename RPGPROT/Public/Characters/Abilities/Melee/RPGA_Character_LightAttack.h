// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/Melee/RPGA_Character_MeleeBase.h"
#include "RPGA_Character_LightAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPGA_Character_LightAttack : public URPGA_Character_MeleeBase
{
	GENERATED_BODY()

public:
	URPGA_Character_LightAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

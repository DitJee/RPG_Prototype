// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/RPGameplayAbility.h"
#include "RPGA_CharacterDodge.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPGA_CharacterDodge : public URPGameplayAbility
{
	GENERATED_BODY()
	
public:
	URPGA_CharacterDodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT | Dash | Montage")
		class UAnimMontage* DodgeAnimMontage;

	class URPAT_PlayMontageAndWaitForEvents* PlayMontageAndWaitForEventsClass;
};

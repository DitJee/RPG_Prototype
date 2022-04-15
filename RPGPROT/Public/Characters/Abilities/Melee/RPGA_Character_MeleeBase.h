// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/RPGameplayAbility.h"
#include "RPGA_Character_MeleeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPGA_Character_MeleeBase : public URPGameplayAbility
{
	GENERATED_BODY()
	
public:

	URPGA_Character_MeleeBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT | Melee | Montage")
		class UAnimMontage* MeleeAnimMontage;

	UFUNCTION()
		void ExecuteEndAbility(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void ExecuteApplyEffectContainer(FGameplayTag EventTag, FGameplayEventData EventData);
};

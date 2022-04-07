// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayEffectTypes.h"

#include "CoreMinimal.h"
#include "Characters/Abilities/RPGameplayAbility.h"
#include "RPGA_CharacterSprint.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPGA_CharacterSprint : public URPGameplayAbility
{
	GENERATED_BODY()

public:
	URPGA_CharacterSprint();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPGPROT | Gameplay Effect")
		TSubclassOf<class UGameplayEffect>  GameplayEffectSprint;

	bool bEndSprint;

	FActiveGameplayEffectHandle GESprintHandle;

	class UAbilityTask_WaitInputRelease* WaitInputReleaseClass;

	FDelegateHandle WaitInputReleaseHandle;

	void OnRelease();

};

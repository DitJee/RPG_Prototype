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

	bool bEndSprint;

	FActiveGameplayEffectHandle GESprintHandle;

	class UAbilityTask_WaitInputRelease* WaitInputReleaseClass;

	class UAbilityTask_WaitDelay* WaitDelayClass;

	class UAbilityTask_NetworkSyncPoint* NetworkSyncPointClass;

	FDelegateHandle WaitInputReleaseHandle;

	UFUNCTION()
		void OnRelease(float TimeHeld);

	UFUNCTION()
		void WaitDelay();

	UFUNCTION()
		void OnWaitNetSync();

	UFUNCTION()
		void OnSync();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPGPROT | Gameplay Effect")
		TSubclassOf<class UGameplayEffect>  GameplayEffectSprint;
};

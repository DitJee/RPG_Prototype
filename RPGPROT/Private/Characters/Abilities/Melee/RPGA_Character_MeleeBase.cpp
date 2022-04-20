// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Melee/RPGA_Character_MeleeBase.h"

#include "Animation/AnimMontage.h"

#include "Characters/Abilities/AbilitiyTask/RPAT_PlayMontageAndWaitForEvents.h"

#include "Characters/RPCharacterMovementComponent.h"
#include "Characters/RPCharacterBase.h"
#include <RPGPROT/Public/Characters/Heros/RPHeroCharacter.h>

URPGA_Character_MeleeBase::URPGA_Character_MeleeBase()
{
	
	
	
}

void URPGA_Character_MeleeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	/** Commit the cost upfront */
	bool bIsSuccess = K2_CommitAbilityCost(false);

	ARPCharacterBase* Actor = Cast<ARPCharacterBase>(GetAvatarActorFromActorInfo());

	URPCharacterMovementComponent* CMC = Cast<URPCharacterMovementComponent>(Actor->GetMovementComponent());

	bool bIsInAir = CMC->IsFalling();
	
	if (bIsSuccess && !bIsInAir)
	{
		/**
			Play the Dodge montage
		*/
		URPAT_PlayMontageAndWaitForEvents* PlayMontageAndWaitForEventsClass = URPAT_PlayMontageAndWaitForEvents::PlayMontageAndWaitForEvent(
			this,
			NAME_None,
			MeleeAnimMontage,
			FGameplayTagContainer(),
			1.0f,
			NAME_None,
			true,
			1.0f
		);

		PlayMontageAndWaitForEventsClass->OnBlendOut.AddDynamic(this, &URPGA_Character_MeleeBase::ExecuteEndAbility);
		PlayMontageAndWaitForEventsClass->OnInterrupted.AddDynamic(this, &URPGA_Character_MeleeBase::ExecuteEndAbility);
		PlayMontageAndWaitForEventsClass->OnCancelled.AddDynamic(this, &URPGA_Character_MeleeBase::ExecuteEndAbility);
		PlayMontageAndWaitForEventsClass->EventReceived.AddDynamic(this, &URPGA_Character_MeleeBase::ExecuteApplyEffectContainer);
		PlayMontageAndWaitForEventsClass->ReadyForActivation();
	}
	else
	{
		K2_CancelAbility();
	}
}

void URPGA_Character_MeleeBase::ExecuteEndAbility(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URPGA_Character_MeleeBase::ExecuteApplyEffectContainer(FGameplayTag EventTag, FGameplayEventData EventData)
{

	UE_LOG(LogTemp, Warning, TEXT("Commiting melee cost"));

	/** Commit the cost upfront */
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Ability.Melee")))
	{
		K2_CommitAbilityCost(false);
	}
}


void URPGA_Character_MeleeBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


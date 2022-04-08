// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGA_CharacterDodge.h"
#include "Animation/AnimMontage.h"
#include "Characters/Abilities/AbilityTask/RPAT_PlayMontageAndWaitForEvents.h"


URPGA_CharacterDodge::URPGA_CharacterDodge()
{
	AbilityID = ERPAbilityInputID::Dodge;
	AbilityInputID = ERPAbilityInputID::Dodge;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
}

void URPGA_CharacterDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	/** Commit the cost upfront*/
	bool bIsSuccess = K2_CommitAbilityCost(false);

	if (bIsSuccess)
	{
		/**
			Play the Dodge montage
		*/
		PlayMontageAndWaitForEventsClass = URPAT_PlayMontageAndWaitForEvents::PlayMontageAndWaitForEvent(
			this,
			TEXT("None"),
			DodgeAnimMontage,
			FGameplayTagContainer(),
			1.0f,
			TEXT("None"),
			true,
			1.0f
		);

		/**
			Move the character accordingly to user's input
		*/

	}
	else
	{
		K2_CancelAbility();
	}
}

void URPGA_CharacterDodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

}

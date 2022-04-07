// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGA_CharacterSprint.h"
#include "Characters/RPCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"


URPGA_CharacterSprint::URPGA_CharacterSprint()
{
	AbilityID = ERPAbilityInputID::Sprint;
	AbilityInputID = ERPAbilityInputID::Sprint;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));

	bEndSprint = false;
}

void URPGA_CharacterSprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	bEndSprint = false;

	if (GameplayEffectSprint == NULL)
	{
		return;
	}

	UGameplayEffect* GE = Cast<UGameplayEffect>(GameplayEffectSprint.GetDefaultObject());

	if (GE)
	{
		GESprintHandle = ApplyGameplayEffectToOwner(
			Handle,
			ActorInfo,
			ActivationInfo,
			GE,
			GetAbilityLevel(),
			1
			);

		K2_AddGameplayCue(
			FGameplayTag::RequestGameplayTag(FName("GameplayCue.Ability.Sprinting")),
			FGameplayEffectContextHandle(),
			true
		);

		TWeakObjectPtr<UMovementComponent> MovementComponent = ActorInfo->MovementComponent;
		
		URPCharacterMovementComponent* MC = Cast<URPCharacterMovementComponent>(MovementComponent);

		if (MC)
		{
			MC->StartSprinting();

			/**
			* While the player is sprinting, consume the attributes
			*/
			/*WaitInputReleaseClass = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);

			if (WaitInputReleaseClass)
			{
				WaitInputReleaseClass->OnReleaseCallback();
			}*/
			
		}
	}
}

void URPGA_CharacterSprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bEndSprint = true;

	BP_RemoveGameplayEffectFromOwnerWithHandle(GESprintHandle, -1);

	TWeakObjectPtr<UMovementComponent> MovementComponent = ActorInfo->MovementComponent;

	URPCharacterMovementComponent* MC = Cast<URPCharacterMovementComponent>(MovementComponent);

	if (MC)
	{
		MC->StartSprinting();
	}

}

void URPGA_CharacterSprint::OnRelease()
{
}

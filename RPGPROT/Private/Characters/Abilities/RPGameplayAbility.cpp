// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

URPGameplayAbility::URPGameplayAbility()
{
	// instance policy is set to instanced per actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	/**
		Add blocking tag to prevent the player from activating the ability in undesired state.
		Namely when the player is dead or stunned
	*/
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void URPGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

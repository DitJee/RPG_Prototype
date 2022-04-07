// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGA_CharacterJump.h"
#include "Characters/RPCharacterBase.h"
#include "RPGPROT/RPGPROT.h"

URPGA_CharacterJump::URPGA_CharacterJump()
{
	AbilityID = ERPAbilityInputID::Jump;
	AbilityInputID = ERPAbilityInputID::Jump;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

void URPGA_CharacterJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		/**  commit the cost and cooldown */
		bool bIsSuccess = CommitAbility(Handle,ActorInfo, ActivationInfo);

		if (!bIsSuccess)
		{
			/** Cancel ability if the commitment failed*/
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		/** Jump*/
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		Character->Jump();
	}
}

bool URPGA_CharacterJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const ARPCharacterBase* Character = CastChecked<ARPCharacterBase>(
		ActorInfo->AvatarActor.Get(), 
		ECastCheckedType::NullAllowed
		);

	return Character && Character->CanJump();
}

void URPGA_CharacterJump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void URPGA_CharacterJump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		/** 
			Add this function to a list of all the functions 
			waiting for the scope lock to end so they can run. 
		*/

		WaitingToExecute.Add(
			FPostLockDelegate::CreateUObject(
				this,
				&URPGA_CharacterJump::CancelAbility,
				Handle,
				ActorInfo,
				ActivationInfo,
				bReplicateCancelAbility
			)
		);

		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

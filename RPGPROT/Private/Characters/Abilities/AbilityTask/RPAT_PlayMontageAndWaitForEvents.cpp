// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/AbilityTask/RPAT_PlayMontageAndWaitForEvents.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"

#include "Characters/Abilities/RPAbilitySystemComponent.h"

// GAS input enum
#include "RPGPROT/RPGPROT.h"

#include "GameFramework/Character.h"


URPAT_PlayMontageAndWaitForEvents::URPAT_PlayMontageAndWaitForEvents(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	Rate = 1.0f;
	bStopWhenAbilityEnds = true;
}

URPAbilitySystemComponent* URPAT_PlayMontageAndWaitForEvents::GetTargetASC()
{
	return Cast<URPAbilitySystemComponent>(AbilitySystemComponent);;
}

URPAT_PlayMontageAndWaitForEvents* URPAT_PlayMontageAndWaitForEvents::PlayMontageAndWaitForEvent(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	FGameplayTagContainer EventTags,
	float Rate,
	FName StartSection,
	bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale
)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	URPAT_PlayMontageAndWaitForEvents* MyObj = NewAbilityTask<URPAT_PlayMontageAndWaitForEvents>(
		OwningAbility, TaskInstanceName
		);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags = EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return MyObj;
}

void URPAT_PlayMontageAndWaitForEvents::Activate()
{
	/** validity check */
	if (Ability == nullptr)
	{
		return;
	}

	/** Get ASC */
	bool bPlayedMontage = false;
	URPAbilitySystemComponent* RPAbilitySystemComponent = GetTargetASC();

	if (RPAbilitySystemComponent)
	{
		/** Get ActorInfo and AnimInstance */
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance)
		{
			/** Bind event to OnGameplayEvent*/
			EventHandle = RPAbilitySystemComponent->AddGameplayEventTagContainerDelegate(
				EventTags,
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
					this,
					&URPAT_PlayMontageAndWaitForEvents::OnGameplayEvent
				)
			);

			/** Play the montage*/
			const float Duration = RPAbilitySystemComponent->PlayMontage(
				Ability,
				Ability->GetCurrentActivationInfo(),
				MontageToPlay,
				Rate,
				StartSection
			);

			if (Duration > 0.0f)
			{
				/**
					Playing a montage could potentially fire off a callback into game code 
					which could kill this ability! Early out if we are pending kill.
				*/
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				/** Bind OnCancel event */
				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(
					this, 
					&URPAT_PlayMontageAndWaitForEvents::OnAbilityCancelled
				);

				/** Bind and Set BlendingOut Delegate */
				BlendingOutDelegate.BindUObject(
					this,
					&URPAT_PlayMontageAndWaitForEvents::OnMontageBlendingOut
				);

				/** Bind and Set MontageEnded Delegate */
				MontageEndedDelegate.BindUObject(
					this,
					&URPAT_PlayMontageAndWaitForEvents::OnMontageEnded
				);

				/** Set AnimRootMotionTranslationScale*/

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());

				if (Character)
				{
					if (Character->GetLocalRole() == ROLE_Authority ||
						(Character->GetLocalRole() == ROLE_AutonomousProxy && 
							Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)
						)
					{
						Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
					}
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("%s call to PlayMontage() failed"), *FString(__FUNCTION__));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("%s : RPAbilitySystemComponent is NULL"), *FString(__FUNCTION__));
	}

	if (!bPlayedMontage)
	{
		UE_LOG(
			LogTemp, 
			Warning, 
			TEXT("%s called in Ability %s failed to play montage %s; Task Instance Name %s."), 
			*FString(__FUNCTION__),
			*Ability->GetName(), 
			*GetNameSafe(MontageToPlay), 
			*InstanceName.ToString()
		);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			ABILITY_LOG(Display, TEXT("%s: OnCancelled"), *GetName());
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();

}

void URPAT_PlayMontageAndWaitForEvents::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();

}

FString URPAT_PlayMontageAndWaitForEvents::GetDebugString() const
{
	return FString();
}

void URPAT_PlayMontageAndWaitForEvents::OnDestroy(bool AbilityEnded)
{
	/**
		 Note: Clearing montage end delegate isn't necessary 
		 since its not a multicast and will be cleared when the next montage plays.

		 (If we are destroyed, it will detect this and not do anything)

		 This delegate, however, should be cleared as it is a multicast
	*/
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);

		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	URPAbilitySystemComponent* RPAbilitySystemComponent = GetTargetASC();

	if (RPAbilitySystemComponent)
	{
		RPAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(
			EventTags,
			EventHandle
		);
	}

	Super::OnDestroy(AbilityEnded);
}

bool URPAT_PlayMontageAndWaitForEvents::StopPlayingMontage()
{
	/** Get ActorInfo and AnimInstance */
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (AnimInstance)
	{
		/**
			Check if the montage is still playing.
			Then stop the montage if the ability has been interupted
		*/
		if (AbilitySystemComponent && Ability)
		{
			if (
				AbilitySystemComponent->GetAnimatingAbility() == Ability &&
				AbilitySystemComponent->GetCurrentMontage() == MontageToPlay
				)
			{
				/**
					Unbind the delegates and stop playing the montage
				*/
				FAnimMontageInstance* AnimMontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
				
				if (AnimMontageInstance)
				{
					AnimMontageInstance->OnMontageBlendingOutStarted.Unbind();
					AnimMontageInstance->OnMontageEnded.Unbind();
				}

				AbilitySystemComponent->CurrentMontageStop();

				return true;
			}
		}
	}

	return false;
}



void URPAT_PlayMontageAndWaitForEvents::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	/** 
		check the montage and clear and reset animation
	*/
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			if (AbilitySystemComponent)
			{
				AbilitySystemComponent->ClearAnimatingAbility(Ability);
				
				/** Reset the AnimRootMotionTranslateScale*/
				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());

				if (Character->GetLocalRole() == ROLE_Authority ||
					(
					Character->GetLocalRole() == ROLE_AutonomousProxy &&
					Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted
					)
				)
				{
					Character->SetAnimRootMotionTranslationScale(1.0f);
				}
			}
		}
	}

	/**
		Broadcast the delegate accordingly
	*/

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void URPAT_PlayMontageAndWaitForEvents::OnAbilityCancelled()
{
	bool bCanStopPlaying = StopPlayingMontage();

	if (bCanStopPlaying)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void URPAT_PlayMontageAndWaitForEvents::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

void URPAT_PlayMontageAndWaitForEvents::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}

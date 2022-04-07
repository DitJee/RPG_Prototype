// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGA_CharacterSprint.h"
#include "Characters/RPCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"


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
		
		URPCharacterMovementComponent* CMC = Cast<URPCharacterMovementComponent>(MovementComponent);

		if (CMC)
		{
			CMC->StartSprinting();

			/**
			* While the player is sprinting, consume the attributes
			*/
			
			WaitInputReleaseClass = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);

			if (WaitInputReleaseClass)
			{
				/** Bind the delegate to OnRelease() */
				WaitInputReleaseClass->OnRelease.AddDynamic(this,&URPGA_CharacterSprint::OnRelease);
				WaitInputReleaseClass->ReadyForActivation();

				/**
					Predictively drain Stamina while sprinting loop. 
					If you didn't want to predict the cost, you could use a periodic GameplayEffect. 
					This does cause an RPC to the Server every loop 
					whereas the non predictive periodic GameplayEffect would not.
				*/
				WaitDelay();
			}
			
		}
	}
}

void URPGA_CharacterSprint::OnRelease(float TimeHeld)
{
	#if UE_BUILD_DEBUG
		UE_LOG(LogTemp, Warning, TEXT("TimeHeld: %f"), TimeHeld);
	#endif

	bEndSprint = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}

void URPGA_CharacterSprint::WaitDelay()
{
	WaitDelayClass = UAbilityTask_WaitDelay::WaitDelay(this, 0.05f);

	if (WaitDelayClass)
	{
		/**
			Create a new Scoped Prediction Key so that we can predict the Stamina cost.
		*/
		WaitDelayClass->OnFinish.AddDynamic(this, &URPGA_CharacterSprint::OnWaitNetSync);
		WaitDelayClass->ReadyForActivation();
	}
}

void URPGA_CharacterSprint::OnWaitNetSync()
{
	#if UE_BUILD_DEBUG
		UE_LOG(LogTemp, Warning, TEXT("OnWaitNetSync:"));
	#endif

	NetworkSyncPointClass = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyServerWait);
	
	if (NetworkSyncPointClass)
	{
		NetworkSyncPointClass->OnSync.AddDynamic(this, &URPGA_CharacterSprint::OnSync);
		NetworkSyncPointClass->ReadyForActivation();
	}
}

void URPGA_CharacterSprint::OnSync()
{
	FGameplayAbilityActorInfo ActorInfo = GetActorInfo();
	
	UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(ActorInfo.MovementComponent);

	if (CMC)
	{
		float Acceleration = CMC->GetCurrentAcceleration().Size();

		if (!CMC->IsFalling() && Acceleration > 0.0f)
		{
			bool bIsSuccess = K2_CommitAbilityCost(false);

			if (bIsSuccess)
			{
				if (!bEndSprint)
				{
					WaitDelay();
				}
			}
			else
			{
				bEndSprint = true;
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}
		}
		else
		{
			if (!bEndSprint)
			{
				WaitDelay();
			}
		}
	}
}

void URPGA_CharacterSprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	bEndSprint = true;

	BP_RemoveGameplayEffectFromOwnerWithHandle(GESprintHandle, -1);

	URPCharacterMovementComponent* CMC = Cast<URPCharacterMovementComponent>(ActorInfo->MovementComponent);

	if (CMC)
	{
		CMC->StopSprinting();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


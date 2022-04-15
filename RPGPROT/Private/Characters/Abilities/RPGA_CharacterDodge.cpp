// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPGA_CharacterDodge.h"

#include "Animation/AnimMontage.h"

#include "Characters/Abilities/AbilitiyTask/RPAT_PlayMontageAndWaitForEvents.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

#include "Characters/RPCharacterMovementComponent.h"
#include "Characters/RPCharacterBase.h"

#include "Characters/Heros/RPHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"

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

	ARPCharacterBase* Actor = Cast<ARPCharacterBase>(GetAvatarActorFromActorInfo());
	URPCharacterMovementComponent* CMC = Cast<URPCharacterMovementComponent>(Actor->GetMovementComponent());

	bool bIsInAir = CMC->IsFalling();

	{
		/**
			Play the Dodge montage
			TEXT("None"),
			DodgeAnimMontage,
			FGameplayTagContainer(),
			1.0f,
			TEXT("None"),
			true,
			1.0f
		);
		PlayMontageAndWaitForEventsClass->OnBlendOut.AddDynamic(this,    &URPGA_CharacterDodge::OnBlendOut);
		PlayMontageAndWaitForEventsClass->OnCompleted.AddDynamic(this,   &URPGA_CharacterDodge::OnCompleted);
		PlayMontageAndWaitForEventsClass->OnInterrupted.AddDynamic(this, &URPGA_CharacterDodge::OnInterrupted);
		PlayMontageAndWaitForEventsClass->OnCancelled.AddDynamic(this,   &URPGA_CharacterDodge::OnCancelled);
		PlayMontageAndWaitForEventsClass->EventReceived.AddDynamic(this, &URPGA_CharacterDodge::EventReceived);
		PlayMontageAndWaitForEventsClass->ReadyForActivation();

		/**
			Move the character accordingly to user's input
		*/

		// Calculate direction and velocity
		
		FRotator CurrentRotation = GetAvatarActorFromActorInfo()->GetActorRotation();

		if (CMC)
		{
			if (HeroCharacter)
			{
				FVector Start = HeroCharacter->GetActorLocation();
				FVector End   = Start + WorldDirection;
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

				FRotator NewRotation = FMath::RInterpTo(
					CurrentRotation,
					TargetRotation,
					GetWorld()->GetDeltaSeconds(),
					0.05
				);

				HeroCharacter->SetActorRotation(NewRotation);
			}

			UAbilityTask_ApplyRootMotionConstantForce* ApplyRootMotionConstantForceClass = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
				this,
				FName(TEXT("None")),
				WorldDirection,
				DodgeStrength,
				DodgeDuration,
				false,
				nullptr,
				ERootMotionFinishVelocityMode::SetVelocity,
				Velocity,
				0.0f,
				false
			);

			ApplyRootMotionConstantForceClass->OnFinish.AddDynamic(
				this,
				&URPGA_CharacterDodge::OnFinish
			);

			ApplyRootMotionConstantForceClass->ReadyForActivation();
		}

	}
		K2_CancelAbility();
	}
}

void URPGA_CharacterDodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
void URPGA_CharacterDodge::OnFinish()
}
{
void URPGA_CharacterDodge::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void URPGA_CharacterDodge::OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void URPGA_CharacterDodge::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void URPGA_CharacterDodge::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

}

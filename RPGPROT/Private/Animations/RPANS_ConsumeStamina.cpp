// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RPANS_ConsumeStamina.h"
#include "Characters/Heros/RPHeroCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"


URPANS_ConsumeStamina::URPANS_ConsumeStamina()
{

}

void URPANS_ConsumeStamina::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ARPHeroCharacter* HeroCharacter = Cast<ARPHeroCharacter>(MeshComp->GetOwner());

	if (HeroCharacter)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HeroCharacter,
			FGameplayTag::RequestGameplayTag(FName("Ability.Melee")),
			FGameplayEventData()
		);

	}
	
}

void URPANS_ConsumeStamina::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void URPANS_ConsumeStamina::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}

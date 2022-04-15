// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RPANS_JumpSection.h"
#include "Characters/Heros/RPHeroCharacter.h"


URPANS_JumpSection::URPANS_JumpSection()
{
}


void URPANS_JumpSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) 
{
	ARPHeroCharacter* HeroCharacter = Cast<ARPHeroCharacter>(MeshComp->GetOwner());

	if (HeroCharacter)
	{
		HeroCharacter->bEnableComboPeriod = true;
		HeroCharacter->JumpSectionNs = this;
		
	}

}

void URPANS_JumpSection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) 
{
	ARPHeroCharacter* HeroCharacter = Cast<ARPHeroCharacter>(MeshComp->GetOwner());

	if (HeroCharacter)
	{
		//HeroCharacter->JumpSectionForCombo();
	}
}

void URPANS_JumpSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) 
{
	ARPHeroCharacter* HeroCharacter = Cast<ARPHeroCharacter>(MeshComp->GetOwner());

	if (HeroCharacter)
	{
		HeroCharacter->bEnableComboPeriod = false;

	}

}

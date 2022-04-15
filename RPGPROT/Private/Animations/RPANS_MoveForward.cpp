// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RPANS_MoveForward.h"

URPANS_MoveForward::URPANS_MoveForward()
{
	bShouldFireInEditor = false;
}

void URPANS_MoveForward::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

}

void URPANS_MoveForward::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();

	if (Actor)
	{
		FVector ActorForwardVector = Actor->GetActorForwardVector();

		float OffsetAmount = FrameDeltaTime * MoveDistance;
			
		Actor->AddActorWorldOffset(
			OffsetAmount * ActorForwardVector,
			true,
			false
		);
		
	}
}

void URPANS_MoveForward::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}

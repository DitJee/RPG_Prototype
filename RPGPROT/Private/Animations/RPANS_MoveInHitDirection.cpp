// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RPANS_MoveInHitDirection.h"

URPANS_MoveInHitDirection::URPANS_MoveInHitDirection()
{
	bShouldFireInEditor = false;
}

void URPANS_MoveInHitDirection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();

	if (Actor)
	{
		// calculate move distance
		float OffsetAmount = FrameDeltaTime * MoveDistance;

		// determine the hit direction from HitDirectionEnum
		FVector HitDirection;
		switch (HitDirectionEnum)
		{
			case ERPHitReactDirection::Left:
				HitDirection = Actor->GetActorRightVector();
				break;
			case ERPHitReactDirection::Right:
				HitDirection = Actor->GetActorRightVector() * -1.0f;
				break;
			case ERPHitReactDirection::Front:
				HitDirection = Actor->GetActorForwardVector() * -1.0f;
				break;
			case ERPHitReactDirection::Back:
				HitDirection = Actor->GetActorForwardVector();
				break;
			default:
				HitDirection = Actor->GetActorForwardVector() * -1.0f;
				break;
		}
	
		Actor->AddActorWorldOffset(
			OffsetAmount * HitDirection,
			true,
			false
		);

	}
}

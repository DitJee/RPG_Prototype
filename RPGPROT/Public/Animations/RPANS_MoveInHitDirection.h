// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGPROT/RPGPROT.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RPANS_MoveInHitDirection.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPANS_MoveInHitDirection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URPANS_MoveInHitDirection();

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "RPGPROT | ANS_MoveInHitDirection | Params")
		ERPHitReactDirection HitDirectionEnum;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "RPGPROT | ANS_MoveForward | Combo")
		float MoveDistance;
};

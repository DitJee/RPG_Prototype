// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RPANS_JumpSection.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPANS_JumpSection : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	URPANS_JumpSection();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "RPGPROT | ANS_JumpSection | Combo")
		TArray<FName> JumpSections;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGPROT/RPGPROT.h"


#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RPANS_WeaponAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPANS_WeaponAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	URPANS_WeaponAttack();

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "RPGPROT|Weapon")
		ERPAbilityInputID AbilityInputID;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

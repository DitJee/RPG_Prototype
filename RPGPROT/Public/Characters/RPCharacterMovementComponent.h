// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	URPCharacterMovementComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float SprintSpeedMultiplier = 1.4f;

	uint8 RequestToStartSprinting : 1;

	virtual float GetMaxSpeed() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void StopSprinting();
};

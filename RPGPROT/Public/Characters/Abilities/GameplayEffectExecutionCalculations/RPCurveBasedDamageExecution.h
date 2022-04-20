// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPCurveBasedDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPCurveBasedDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URPCurveBasedDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

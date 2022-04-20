// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GameplayEffectExecutionCalculations/RPCurveBasedDamageExecution.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"

/**
	Declare the attributes to capture and define how we want to capture them from the Source and Target.
*/
struct RPCurveDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RPCurveDamageStatics()
	{
		/**
			Snapshot happens at time of GESpec creation.

			We're not capturing anything from the Source in this example,
			but there could be like AttackPower attributes that you might want.
		*/

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPAttributeSetBase, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPAttributeSetBase, Armor, Target, false);
	}
};

static const RPCurveDamageStatics& CurveDamageStatics()
{
	static RPCurveDamageStatics DStatics;
	return DStatics;
}

URPCurveBasedDamageExecution::URPCurveBasedDamageExecution()
{
	RelevantAttributesToCapture.Add(CurveDamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(CurveDamageStatics().ArmorDef);
}

void URPCurveBasedDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/**
		Get Target and source ASCs
	*/
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	/**
		Get Target and source actors
	*/
	AActor* TargetActor = nullptr;
	AActor* SourceActor = nullptr;

	if (TargetAbilitySystemComponent)
	{
		TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
	}

	if (SourceAbilitySystemComponent)
	{
		SourceActor = SourceAbilitySystemComponent->GetAvatarActor();
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	/** Gather the tags from the sourceand target as that can affect which buffs should be used */
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage * AttackPower / DefensePower
	//	If DefensePower is 0, it is treated as 1.0
	// --------------------------------------

	/** Get armor value*/
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		CurveDamageStatics().ArmorDef,
		EvaluationParameters,
		Armor
	);
	Armor = FMath::Max<float>(Armor, 0.0f);

	/** Get damage value*/
	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		CurveDamageStatics().DamageDef,
		EvaluationParameters,
		Damage
	);

	float AttackPower = 1.0f;
	float DefensePower = 1.0f;

	float DamageDone = Damage * AttackPower / DefensePower;
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(CurveDamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}
